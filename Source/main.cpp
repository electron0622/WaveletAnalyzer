//============================================================================
//
//  main.cpp
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
#include <wx/msgdlg.h>
#include "main.hpp"
#include "mainframe.hpp"

namespace WaveletAnalyzer {

using std::exception;

IMPLEMENT_APP(MainApp)

bool MainApp::OnInit(void) {
    try {
        MainFrame *frame = new MainFrame(nullptr);
        frame->Show(true);
        SetTopWindow(frame);
    }
    catch(exception &e) {
        const auto message = e.what();
        const auto caption = wxT("Error");
        const auto style   = wxOK|wxICON_ERROR;
        wxMessageBox(message, caption, style);
        return false;
    }
    catch(...) {
        const auto message = wxT("An unknown error has occurred.");
        const auto caption = wxT("Error");
        const auto style   = wxOK|wxICON_ERROR;
        wxMessageBox(message, caption, style);
        return false;
    }
    return true;
}

}  // namespace WaveletAnalyzer
