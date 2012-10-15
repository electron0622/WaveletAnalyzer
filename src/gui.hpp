//============================================================================
//
//  gui.hpp
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

#ifndef _GUI_HPP_
#define _GUI_HPP_

#include <gtk/gtk.h>
#include <mutex>

namespace WaveletAnalyzer {

using std::once_flag;
using std::call_once;

class GUI {

public:

    GUI();
    virtual ~GUI();

public:

    bool BuildUI(const char *filename, const char *objectname);
    bool BuildUI(const void *data, size_t size, const char *objectname);

public:

    bool SetCallback(GtkSignalFunc func, void *data, const char *eventname);

public:

    void ExecuteUI(void);

private:

    static void GtkInitOnce(void);

private:

    static once_flag m_InitFlag;

private:

    GtkBuilder *m_Builder;
    GtkWidget  *m_Window;

};

}  // namespace WaveletAnalyzer

#endif /* _GUI_HPP_ */
