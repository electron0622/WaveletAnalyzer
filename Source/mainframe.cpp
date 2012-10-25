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

#include <wx/filedlg.h>
#include <wx/msgdlg.h>
#include "aboutdialog.hpp"
#include "mainframe.hpp"

namespace WaveletAnalyzer {

MainFrame::MainFrame(wxWindow *parent) : IMainFrame(parent) {
    m_Player = nullptr;
}

MainFrame::~MainFrame() {
    delete m_Player;
}

void MainFrame::OnMenuOpen(wxCommandEvent &event) {
    if(!CloseStream()) return;
    wxFileDialog dialog(this);
    dialog.ShowModal();
    wxString path = dialog.GetPath();
    if(path.IsEmpty()) return;
    OpenStream(path.c_str());
    return;
}

void MainFrame::OnMenuDevice(wxCommandEvent &event) {
    return;
}

void MainFrame::OnMenuClose(wxCommandEvent &event) {
    CloseStream();
    return;
}

void MainFrame::OnMenuExit(wxCommandEvent &event) {
    if(CloseStream()) Close();
    return;
}

void MainFrame::OnMenuAbout(wxCommandEvent &event) {
    AboutDialog dialog(this);
    dialog.ShowModal();
    return;
}

bool MainFrame::OpenStream(const char *path) {
    m_Player = new Player;
    if(!m_Player->Init(path)) {
        const wxString message = wxT("Cannot open the audio stream.");
        const wxString caption = wxT("Error");
        const int      style   = wxOK|wxICON_ERROR;
        wxMessageBox(message, caption, style, this);
        delete m_Player;
        m_Player = nullptr;
        return false;
    }
    return true;
}

bool MainFrame::CloseStream(void) {
    if(m_Player) {
        const wxString message = wxT("Close the audio stream?");
        const wxString caption = wxT("Confirm");
        const int      style   = wxYES_NO|wxICON_QUESTION;
        int answer = wxMessageBox(message, caption, style, this);
        if(answer == wxNO) return false;
        delete m_Player;
        m_Player = nullptr;
    }
    return true;
}

}  // namespace WaveletAnalyzer
