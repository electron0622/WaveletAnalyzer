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
    int w, h;
    wxClientDC MainDC(m_PanelMain), SubDC(m_PanelSub);
    m_PanelMain->GetClientSize(&w, &h);
    wxImage MainImage(w, h);
//    MainImage.SetData(m_pPlayer->GetMainGraph(w, h));
    m_PanelSub->GetClientSize(&w, &h);
    wxImage SubImage(w, h);
//    SubImage.SetData(m_pPlayer->GetSubGlaph(w, h));
    WaitForNextFrame(30);
    MainDC.DrawBitmap(wxBitmap(MainImage), 0, 0);
    SubDC.DrawBitmap(wxBitmap(SubImage), 0, 0);
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
