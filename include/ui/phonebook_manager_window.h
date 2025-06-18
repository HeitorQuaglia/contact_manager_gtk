#ifndef PHONEBOOK_MANAGER_WINDOW_H
#define PHONEBOOK_MANAGER_WINDOW_H

#include <gtk/gtk.h>
#include "phonebook_manager.h"

#define PHONEBOOK_MANAGER_WINDOW_TYPE (phonebook_manager_window_get_type())
G_DECLARE_FINAL_TYPE(PhonebookManagerWindow, phonebook_manager_window, PHONEBOOK_MANAGER, WINDOW, GtkApplicationWindow)

GtkWidget *phonebook_manager_window_new(GtkApplication *app, PhoneBookManager *manager);

#endif //PHONEBOOK_MANAGER_WINDOW_H 