#ifndef PHONEBOOK_H
#define PHONEBOOK_H

#include <glib.h>
#include "contact_object.h"

struct Contact;

typedef struct PhoneBook {
    char *phonebook_id;
    GPtrArray *contacts;
} PhoneBook;

PhoneBook *phonebook_create(void);
void phonebook_destroy(PhoneBook *phonebook);
void phonebook_add_contact(PhoneBook *phonebook, ContactObject *contact);
gboolean phonebook_remove_contact(PhoneBook *phonebook, const char *contact_id);

#endif //PHONEBOOK_H
