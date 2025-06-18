#ifndef CONTACT_DETAIL_WINDOW_H
#define CONTACT_DETAIL_WINDOW_H

#include <gtk/gtk.h>
#include "contact_object.h"

#define CONTACT_DETAIL_WINDOW_TYPE (contact_detail_window_get_type())
G_DECLARE_FINAL_TYPE(ContactDetailWindow, contact_detail_window, CONTACT_DETAIL, WINDOW, GtkApplicationWindow)

GtkWidget *contact_detail_window_new(GtkApplication *app, ContactObject *contact);

#endif //CONTACT_DETAIL_WINDOW_H 