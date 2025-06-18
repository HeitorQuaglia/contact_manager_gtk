#ifndef CONTACTS_VIEW_H
#define CONTACTS_VIEW_H

#include <adwaita.h>
#include "phonebook_object.h"

#define CONTACTS_VIEW_TYPE (contacts_view_get_type())
G_DECLARE_FINAL_TYPE(ContactsView, contacts_view, CONTACTS, VIEW, AdwBin);

GtkWidget *contacts_view_new(PhoneBookObject *phonebook_object);

#endif //CONTACTS_VIEW_H 