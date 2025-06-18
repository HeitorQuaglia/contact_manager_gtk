#ifndef CONTACT_MANAGER_WINDOW_H
#define CONTACT_MANAGER_WINDOW_H

#include <gtk/gtk.h>
#include "phonebook.h"

#define CONTACT_MANAGER_WINDOW_TYPE (contact_manager_window_get_type())
G_DECLARE_FINAL_TYPE(ContactManagerWindow, contact_manager_window, CONTACT_MANAGER, WINDOW, GtkApplicationWindow)

GtkWidget *contact_manager_window_new(GtkApplication *app, PhoneBook *phonebook);

#endif //CONTACT_MANAGER_WINDOW_H 