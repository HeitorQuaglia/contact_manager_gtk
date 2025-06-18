#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <adwaita.h>
#include "phonebook_manager.h"

#define MAIN_WINDOW_TYPE (main_window_get_type())
G_DECLARE_FINAL_TYPE(MainWindow, main_window, MAIN, WINDOW, AdwApplicationWindow)

void main_window_on_activate(GApplication *app, gpointer user_data);

#endif //MAIN_WINDOW_H 