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

#include "audio/file/reader.hpp"
#include "audio/device/reader.hpp"
#include "audio/device/writer.hpp"
#include <unistd.h>
#include <wx/filedlg.h>
#include <wx/msgdlg.h>
#include <wx/choicdlg.h>
#include "aboutdialog.hpp"
#include "mainframe.hpp"
#include "audiodecoder.hpp"

namespace WaveletAnalyzer {

MainFrame::MainFrame(wxWindow *parent) : IMainFrame(parent) {
    m_Player = nullptr;
}

MainFrame::~MainFrame() {
    delete m_Player;
}

void MainFrame::OnWindowClose(wxCloseEvent &event) {
    if(CloseStream()) Destroy();
    return;
}

void MainFrame::OnMenuOpen(wxCommandEvent &event) {
    if(!CloseStream()) return;
    const auto message = wxT("Select a file");
    wxFileDialog dialog(this, message);
    dialog.ShowModal();
    auto name = dialog.GetPath();
    if(name.IsEmpty()) return;
    OpenStream((const char *)name.GetData(), false);
    return;
}

void MainFrame::OnMenuDevice(wxCommandEvent &event) {
    Audio::Device::Reader reader;
    int num = reader.GetDeviceCount();
    wxArrayString devlist;
    for(int i = 0; i < num; i++) devlist.Add(reader.GetDeviceName(i));
    const auto message = wxT("Audio capture device:");
    const auto caption = wxT("Select a device");
    wxSingleChoiceDialog dialog(this, message, caption, devlist);
    dialog.ShowModal();
    auto name = dialog.GetStringSelection();
    if(name.IsEmpty()) return;
    reader.Open(name.GetData());
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

bool MainFrame::OpenStream(const char *name, bool mode) {
    if(m_Player) return false;
    static Audio::File::Reader   reader;
    static Audio::Device::Writer writer;
    reader.Open(name);
    while(reader.Read(nullptr, 0) < 0x800) usleep(1);
    writer.SetSampleRate(reader.GetSampleRate());
    writer.SetNumChannels(reader.GetNumChannels());
    if(!writer.Open(writer.GetDefaultDeviceName())) return false;
    m_Player = new Player;
    if(!m_Player->Init(&reader, &writer)) {
        const auto message = wxT("Cannot open the audio stream.");
        const auto caption = wxT("Error");
        const auto style   = wxOK|wxICON_ERROR;
        wxMessageBox(message, caption, style, this);
        delete m_Player;
        m_Player = nullptr;
        return false;
    }
    return true;
}

bool MainFrame::CloseStream(void) {
    if(m_Player) {
        const auto message = wxT("Close the audio stream?");
        const auto caption = wxT("Confirm");
        const auto style   = wxYES_NO|wxICON_QUESTION;
        auto answer = wxMessageBox(message, caption, style, this);
        if(answer == wxNO) return false;
        delete m_Player;
        m_Player = nullptr;
    }
    return true;
}

}  // namespace WaveletAnalyzer
