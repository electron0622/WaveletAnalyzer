//============================================================================
//
//  mainframe.hpp
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

#ifndef _MAINFRAME_HPP_
#define _MAINFRAME_HPP_

#include "frame.h"
#include "player.hpp"

namespace WaveletAnalyzer {

class MainFrame : public IMainFrame {

public:
    MainFrame(wxWindow *parent);
    ~MainFrame();

private:
    void OnMenuOpen(wxCommandEvent &event);
    void OnMenuDevice(wxCommandEvent &event);
    void OnMenuClose(wxCommandEvent &event);
    void OnMenuExit(wxCommandEvent &event);

private:
    void OnMenuAbout(wxCommandEvent &event);

private:
    Player   *m_Player;
    Recorder *m_Recorder;

};

}  // namespace WaveletAnalyzer

#endif /* _MAINFRAME_HPP_ */
