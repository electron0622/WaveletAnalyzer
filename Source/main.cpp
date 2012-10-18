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
#include <wx/encconv.h>
#include "main.hpp"
#include "mainframe.hpp"

namespace WaveletAnalyzer {

using std::exception;
using std::bad_alloc;

IMPLEMENT_APP(MainApp)

bool MainApp::OnInit(void) {
    try {
        MainFrame *frame = new MainFrame(nullptr);
        if(!frame) throw bad_alloc();
        frame->Show(true);
        SetTopWindow(frame);
    }
    catch(exception &e) {
        wxString ErrMsg(e.what(), wxConvUTF8);
        wxMessageBox(ErrMsg, wxT("Error"), wxOK|wxICON_ERROR);
        return false;
    }
    catch(...) {
        wxString ErrMsg(wxT("An unknown error has occurred."));
        wxMessageBox(ErrMsg, wxT("Error"), wxOK|wxICON_ERROR);
        return false;
    }
    return true;
}

}  // namespace WaveletAnalyzer
