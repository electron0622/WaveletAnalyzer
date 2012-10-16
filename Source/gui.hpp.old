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

    bool LoadUI(const char *file_name);
    bool LoadUI(const void *data, size_t size);
    bool ShowUI(const char *object_name);

public:

    bool SetCallback(const char *object_name, const char *event_name, GtkSignalFunc func, void *data);

public:

    void Execute(void);

private:

    static void GtkInitOnce(void);
    static void GtkExecuteOnce(void);

private:

    static once_flag m_InitFlag;
    static once_flag m_ExecuteFlag;

private:

    GtkBuilder *m_Builder;

};

}  // namespace WaveletAnalyzer

#endif /* _GUI_HPP_ */
