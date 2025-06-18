#ifndef PHONEBOOK_OBJECT_H
#define PHONEBOOK_OBJECT_H

#include <glib-object.h>
#include <gio/gio.h>
#include "phonebook.h"

#define PHONEBOOK_TYPE_OBJECT (phonebook_object_get_type())
G_DECLARE_FINAL_TYPE(PhoneBookObject, phonebook_object, PHONEBOOK, OBJECT, GObject)

PhoneBookObject *phonebook_object_new(PhoneBook *phonebook);

PhoneBook *phonebook_object_get_phonebook(PhoneBookObject *self);
void phonebook_object_set_name(PhoneBookObject *self, const char *name);

void phonebook_object_add_new_contact(PhoneBookObject *self);
void phonebook_object_remove_contact(PhoneBookObject *self, ContactObject *contact);
GListStore *phonebook_object_get_contacts(PhoneBookObject *self);

#endif //PHONEBOOK_OBJECT_H 