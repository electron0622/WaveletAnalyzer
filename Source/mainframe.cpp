//============================================================================
//
//  mainframe.cpp
//
//  Copyright (C) 2012  Sato Takaaki.
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//============================================================================

#include <unistd.h>
#include <wx/dcclient.h>
#include <wx/image.h>
#include <wx/filedlg.h>
#include <wx/msgdlg.h>
#include <wx/choicdlg.h>
#include <functional>
#include <chrono>
#include "aboutdialog.hpp"
#include "mainframe.hpp"
#include "audio/file/reader.hpp"
#include "audio/device/reader.hpp"
#include "audio/device/writer.hpp"

namespace WaveletAnalyzer {

namespace chrono = std::chrono;

using std::bind;
using std::placeholders::_1;

MainFrame::MainFrame(wxWindow *parent) :
        IMainFrame(parent), m_pPlayer(nullptr), m_pAnalyzer(new Analyzer) {
    m_Shades.Init(1920, 1080);
    m_Line  .Init( 320,  240);
}

MainFrame::~MainFrame() {
    delete m_pPlayer;
    delete m_pAnalyzer;
}

void MainFrame::OnWindowClose(wxCloseEvent &event) {
    if(CloseStream()) Destroy();
    return;
}

void MainFrame::OnWindowIdle(wxIdleEvent &event) {
    event.RequestMore(true);
    if(!IsNextFrame(1)) { usleep(1); return; }
    unsigned char *data;
    int width, height;
    float  time  = 5.0f;
    float  sigma = 3.0f;
    size_t rate  = 44100;
    float  fmin  = 50.0f;
    float  fmax  = 10000.0f;
    size_t fnum  = 512;
    wxClientDC MainDC(m_PanelMain), SubDC(m_PanelSub);
    m_PanelMain->GetClientSize(&width, &height);
    m_Shades.SetRange(-time, 0.0f, fmin, fmax);
    m_pAnalyzer->Init(sigma, time, rate, fmin, fmax, fnum);
    size_t xnum = time * rate;
    size_t ynum = fnum;
    m_Shades.Draw(m_pAnalyzer->GetData(), xnum, ynum, width, height);
    data = (unsigned char *)const_cast<void *>(m_Shades.GetData());
    wxImage MainImage(m_Shades.GetWidth(), m_Shades.GetHeight(), data, true);
    m_PanelSub->GetClientSize(&width, &height);
    Plot::LineFunc mfunc = std::bind(&Analyzer::MotherWavelet, _1, sigma);
    auto col = 3.0f * sigma;
    auto row = 0.5f / sigma;
    m_Line.SetRange(-col, col, -row, row);
    m_Line.Draw(&mfunc, width, height);
    data = (unsigned char *)const_cast<void *>(m_Line.GetData());
    wxImage SubImage(m_Line.GetWidth(), m_Line.GetHeight(), data, true);
    MainDC.DrawBitmap(wxBitmap(MainImage), 0, 0);
    SubDC.DrawBitmap(wxBitmap(SubImage), 0, 0);
    return;
}

void MainFrame::OnMenuOpen(wxCommandEvent &event) {
    if(!CloseStream()) return;
    const auto message = wxT("Select a file");
    wxFileDialog dialog(this, message);
    dialog.ShowModal();
    auto name = dialog.GetPath();
    if(name.IsEmpty()) return;
    auto pReader = new Audio::File::Reader;
    auto pWriter = new Audio::Device::Writer;
    auto pInput  = io_ptr(pReader);
    auto pOutput = io_ptr(pWriter);
    pReader->SetCacheSize(0x10000);
    pWriter->SetCacheSize(0x10000);
    if(!pReader->Open((const char *)name.GetData())) {
        OpenErrorDialog();
        return;
    }
    auto t = time(nullptr);
    while(pReader->Read(nullptr, 0) == 0) {
        if(time(nullptr) - t >= 3) {
            OpenErrorDialog();
            return;
        }
        usleep(1);
    }
    pWriter->SetSampleRate(pReader->GetSampleRate());
    pWriter->SetNumChannels(pReader->GetNumChannels());
    if(!pWriter->Open(pWriter->GetDefaultDeviceName())) {
        OpenErrorDialog();
        return;
    }
    if(!OpenStream(pInput, pOutput)) {
        OpenErrorDialog();
        return;
    }
    return;
}

void MainFrame::OnMenuDevice(wxCommandEvent &event) {
    if(!CloseStream()) return;
    auto pReader = new Audio::Device::Reader;
    auto pWriter = new Audio::Device::Writer;
    auto pInput  = io_ptr(pReader);
    auto pOutput = io_ptr(pWriter);
    auto num     = pReader->GetDeviceCount();
    wxArrayString devlist;
    for(int i = 0; i < num; i++) devlist.Add(pReader->GetDeviceName(i));
    const auto message = wxT("Audio capture device:");
    const auto caption = wxT("Select a device");
    wxSingleChoiceDialog dialog(this, message, caption, devlist);
    dialog.ShowModal();
    auto name = dialog.GetStringSelection();
    if(name.IsEmpty()) return;
    if(!pReader->Open((const char *)name.GetData())) {
        OpenErrorDialog();
        return;
    }
    pWriter->SetSampleRate(pReader->GetSampleRate());
    pWriter->SetNumChannels(pReader->GetNumChannels());
    if(!pWriter->Open(pWriter->GetDefaultDeviceName())) {
        OpenErrorDialog();
        return;
    }
    if(!OpenStream(pInput, pOutput)) {
        OpenErrorDialog();
        return;
    }
    return;
}

void MainFrame::OnMenuClose(wxCommandEvent &event) {
    CloseStream();
    return;
}

void MainFrame::OnMenuExit(wxCommandEvent &event) {
    Close();
    return;
}

void MainFrame::OnMenuAbout(wxCommandEvent &event) {
    AboutDialog dialog(this);
    dialog.ShowModal();
    return;
}

void MainFrame::OnPlayButtonClick(wxCommandEvent &event) {
    if(m_pPlayer) m_pPlayer->Play();
    return;
}

void MainFrame::OnStopButtonClick(wxCommandEvent &event) {
    if(m_pPlayer) m_pPlayer->Stop();
    return;
}

void MainFrame::OpenErrorDialog(void) {
    const auto message = wxT("Cannot open the audio stream.");
    const auto caption = wxT("Error");
    const auto style   = wxOK|wxICON_ERROR;
    wxMessageBox(message, caption, style, this);
    return;
}

bool MainFrame::OpenStream(io_ptr &pReader, io_ptr &pWriter) {
    if(!m_pPlayer) m_pPlayer = new Player;
    if(!m_pPlayer->Init(pReader, pWriter, m_pAnalyzer)) {
        delete m_pPlayer;
        m_pPlayer = nullptr;
        return false;
    }
    return true;
}

bool MainFrame::CloseStream(void) {
    if(m_pPlayer) {
        const auto message = wxT("Close the audio stream?");
        const auto caption = wxT("Confirm");
        const auto style   = wxYES_NO|wxICON_QUESTION;
        auto answer = wxMessageBox(message, caption, style, this);
        if(answer == wxNO) return false;
        delete m_pPlayer;
        m_pPlayer = nullptr;
    }
    return true;
}

bool MainFrame::IsNextFrame(size_t freq) {
    static auto t0 = chrono::system_clock::now();
    auto dt = chrono::nanoseconds(1000000000 / freq);
    auto t  = chrono::system_clock::now();
    if(t - t0 < dt) return false;
    t0 = t;
    return true;
}

}  // namespace WaveletAnalyzer
