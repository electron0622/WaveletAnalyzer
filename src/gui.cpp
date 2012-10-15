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
#include <stdexcept>
#include "gui.hpp"

namespace WaveletAnalyzer {

using std::exception;

GUI::GUI() {
    call_once(m_InitFlag, GtkInitOnce);
    m_Builder = gtk_builder_new();
    if(!m_Builder) throw exception(); // "gtk_builder_new returned nullptr."
}

GUI::~GUI() {
}

bool GUI::LoadUI(const char *file_name) {
    guint success;
    success = gtk_builder_add_from_file(m_Builder, file_name, nullptr);
    if(!success) return false;
    return true;
}

bool GUI::LoadUI(const void *data, size_t size) {
    guint success;
    const char *str = static_cast<const char *>(data);
    success = gtk_builder_add_from_string(m_Builder, str, size, nullptr);
    if(!success) return false;
    return true;
}

bool GUI::SetCallback(const char *object_name, const char *event_name, GtkSignalFunc func, void *data) {
    if(!object_name || !event_name || !func) return false;
    GtkObject *object = GTK_OBJECT(gtk_builder_get_object(m_Builder, object_name));
    if(!object) return false;
    gtk_signal_connect(object, event_name, func, data);
    return true;
}

bool GUI::ShowUI(const char *object_name) {
    if(!object_name) return false;
    GtkWidget *widget = GTK_WIDGET(gtk_builder_get_object(m_Builder, object_name));
    if(!widget) return false;
    gtk_widget_show(widget);
    return true;
}

void GUI::Execute(void) {
    call_once(m_ExecuteFlag, GtkExecuteOnce);
    return;
}

void GUI::GtkInitOnce(void) {
    gtk_set_locale();
    gtk_init(&__argc, &__argv);
    return;
}

void GUI::GtkExecuteOnce(void) {
    gtk_main();
    return;
}

once_flag GUI::m_InitFlag;
once_flag GUI::m_ExecuteFlag;

}  // namespace WaveletAnalyzer
