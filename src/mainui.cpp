//============================================================================
//
//  mainui.cpp
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

#include <functional>
#include "mainui.hpp"

namespace WaveletAnalyzer {

using std::bind;

MainUI::MainUI() {
}

MainUI::~MainUI() {
}

void MainUI::Execute(void) {
    if(!Init()) return;
    return;
}

void test(GtkWidget *button, void *user_data){
    GtkWidget *dialog;
    dialog = gtk_message_dialog_new(
            nullptr,
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_OK,
            "wagui.glade is not found or is broken."
    );
    gtk_window_set_title(GTK_WINDOW(dialog), "File not found.");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

bool MainUI::Init(void) {
    if(!LoadUI("wagui.xml")) {
        GtkWidget *dialog;
        dialog = gtk_message_dialog_new(
                nullptr,
                GTK_DIALOG_MODAL,
                GTK_MESSAGE_ERROR,
                GTK_BUTTONS_OK,
                "wagui.xml is not found."
        );
        gtk_window_set_title(GTK_WINDOW(dialog), "File not found.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return false;
    }
    std::function<void (void)> f;
    f = bind(&MainUI::FileOpen, this);
    SetCallback("wamain", "destroy", GTK_SIGNAL_FUNC(gtk_main_quit), nullptr);
    SetCallback("menu_open", "button_press_event", GTK_SIGNAL_FUNC(test), nullptr);
    ShowUI("wamain");
    GUI::Execute();
    return true;
}

void MainUI::FileOpen(void) {
    return;
}

}  // namespace WaveletAnalyzer
