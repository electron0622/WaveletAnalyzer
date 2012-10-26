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
#include <wx/choicdlg.h>
#include "aboutdialog.hpp"
#include "mainframe.hpp"

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
    wxFileDialog dialog(this);
    dialog.ShowModal();
    auto path = dialog.GetPath();
    if(path.IsEmpty()) return;
    OpenStream((const char *)path.GetData());
    return;
}

void MainFrame::OnMenuDevice(wxCommandEvent &event) {
    wxArrayString arraystr;
    arraystr.Add(wxT("test"));
    wxPoint p;
    wxSingleChoiceDialog dialog(this, wxT("Choose device"), wxT("Choose device"), arraystr);
    dialog.ShowModal();
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

bool MainFrame::OpenStream(const char *path) {
    if(m_Player) return false;
    m_Player = new Player;
    if(!m_Player->Init(path)) {
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
