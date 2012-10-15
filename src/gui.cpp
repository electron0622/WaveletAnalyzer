//============================================================================
//
//  gui.cpp
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

#include <stdlib.h>
#include "gui.hpp"

namespace WaveletAnalyzer {

GUI::GUI() {
    call_once(m_InitFlag, GtkInitOnce);
}

GUI::~GUI() {
}

bool GUI::BuildUI(const char *filename, const char *objectname) {
    guint success;
    m_Builder = gtk_builder_new();
    if(!m_Builder) return false;
    success = gtk_builder_add_from_file(m_Builder, filename, nullptr);
    if(!success) return false;
    m_Window = GTK_WIDGET(gtk_builder_get_object(m_Builder, objectname));
    if(!m_Window) return false;
    gtk_builder_connect_signals(m_Builder, NULL);
    SetCallback(GTK_SIGNAL_FUNC(gtk_main_quit), nullptr, "destroy");
    return true;
}

bool GUI::BuildUI(const void *data, size_t size, const char *objectname) {
    guint success;
    m_Builder = gtk_builder_new();
    if(!m_Builder) return false;
    success = gtk_builder_add_from_string(m_Builder, (const char *)data, size, nullptr);
    if(!success) return false;
    m_Window = GTK_WIDGET(gtk_builder_get_object(m_Builder, objectname));
    if(!m_Window) return false;
    gtk_builder_connect_signals(m_Builder, NULL);
    SetCallback(GTK_SIGNAL_FUNC(gtk_main_quit), nullptr, "destroy");
    return true;
}

bool GUI::SetCallback(GtkSignalFunc func, void *data, const char *eventname) {
    if(!func || !eventname) return false;
    gtk_signal_connect(GTK_OBJECT(m_Window), eventname, func, data);
    return true;
}

void GUI::ExecuteUI(void) {
    gtk_widget_show(m_Window);
    gtk_main();
    return;
}

void GUI::GtkInitOnce(void) {
    gtk_set_locale();
    gtk_init(&__argc, &__argv);
    return;
}

once_flag GUI::m_InitFlag;

}  // namespace WaveletAnalyzer
