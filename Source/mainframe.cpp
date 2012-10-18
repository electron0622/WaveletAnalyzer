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

#include <stdexcept>
#include "mainframe.hpp"
#include "aboutdialog.hpp"

namespace WaveletAnalyzer {

MainFrame::MainFrame(wxWindow *parent) : IMainFrame(parent) {
//    new int[0xF0000000];
}

MainFrame::~MainFrame() {
}

void MainFrame::OnMenuOpen(wxCommandEvent &event) {
    return;
}

void MainFrame::OnMenuDevice(wxCommandEvent &event) {
    return;
}

void MainFrame::OnMenuClose(wxCommandEvent &event) {
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

}  // namespace WaveletAnalyzer
