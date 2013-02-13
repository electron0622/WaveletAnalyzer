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
#include "plot/shades.hpp"
#include "plot/line.hpp"
#include "player.hpp"
#include "analyzer.hpp"

namespace WaveletAnalyzer {

class MainFrame : public IMainFrame {

public:
    MainFrame(wxWindow *parent);
    ~MainFrame();

private:
    void OnWindowClose(wxCloseEvent &event);
    void OnWindowIdle(wxIdleEvent &event);

private:
    void OnMenuOpen(wxCommandEvent &event);
    void OnMenuDevice(wxCommandEvent &event);
    void OnMenuClose(wxCommandEvent &event);
    void OnMenuExit(wxCommandEvent &event);

private:
    void OnMenuAbout(wxCommandEvent &event);

private:
    void OnPanelMainMotion(wxMouseEvent &event);

private:
    void OnPlayButtonClick(wxCommandEvent &event);
    void OnStopButtonClick(wxCommandEvent &event);

private:
    void OpenErrorDialog(void);

private:
    bool OpenStream(io_ptr &pReader, io_ptr &pWriter);
    bool CloseStream(void);

private:
    bool IsNextFrame(size_t freq);

private:
    void DrawXYZ(const wxPoint &p);

private:
    Plot::Shades m_Shades;
    Plot::Line   m_Line;
    Player      *m_pPlayer;
    Analyzer    *m_pAnalyzer;

};

}  // namespace WaveletAnalyzer

#endif /* _MAINFRAME_HPP_ */
