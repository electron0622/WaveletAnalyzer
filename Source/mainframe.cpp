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
#include <chrono>
#include "aboutdialog.hpp"
#include "mainframe.hpp"
#include "audio/file/reader.hpp"
#include "audio/device/reader.hpp"
#include "audio/device/writer.hpp"
#include "plot/line.hpp"

namespace WaveletAnalyzer {

namespace chrono = std::chrono;

MainFrame::MainFrame(wxWindow *parent) :
        IMainFrame(parent), m_pPlayer(nullptr) {
}

MainFrame::~MainFrame() {
    delete m_pPlayer;
}

void MainFrame::OnWindowClose(wxCloseEvent &event) {
    if(CloseStream()) Destroy();
    return;
}

void MainFrame::OnWindowIdle(wxIdleEvent &event) {
    static Plot::Line pl;
    static auto once = false;
    if(!once) { pl.Init(1920, 1080); once = true; };
    int w, h;
    wxClientDC MainDC(m_PanelMain), SubDC(m_PanelSub);
    m_PanelMain->GetClientSize(&w, &h);
    constexpr float sigma = 0.333f;
    Plot::LineFunc mfunc = [](float x) {
        constexpr float one_over_sqrt_two_pi = 1.0 / std::sqrt(2.0 * M_PI);
        return (one_over_sqrt_two_pi / sigma) * std::exp(complex<float>(x * x * (-1.0f / (2.0f * sigma * sigma)), (float)(2.0f * M_PI) * x));
    };
    pl.SetRange(-3.0f * sigma, 3.0f * sigma, -1.5f, 1.5f);
    pl.Draw(&mfunc, w, h);
    wxImage MainImage(pl.GetWidth(), pl.GetHeight(), (unsigned char *)const_cast<void *>(pl.GetData()), true);
    m_PanelSub->GetClientSize(&w, &h);
    wxImage SubImage(w, h);
    WaitForNextFrame(30);
    wxBitmap MainBitmap(MainImage);
    wxBitmap SubBitmap(SubImage);
    MainDC.DrawBitmap(MainBitmap, 0, 0);
    SubDC.DrawBitmap(SubBitmap, 0, 0);
    event.RequestMore(true);
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
    if(!m_pPlayer->Init(pReader, pWriter)) {
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

void MainFrame::WaitForNextFrame(size_t freq) {
    static auto t0 = chrono::system_clock::now();
    auto dt = chrono::nanoseconds(1000000000 / freq);
    while(true) {
        auto t = chrono::system_clock::now();
        if(t - t0 >= dt) {
            t0 = t;
            break;
        }
        usleep(1);
    }
    return;
}

}  // namespace WaveletAnalyzer
