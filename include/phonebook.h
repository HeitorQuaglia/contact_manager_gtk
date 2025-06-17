#ifndef PHONEBOOK_H
#define PHONEBOOK_H

#include "contact.h"

typedef struct {
    char *phonebook_id;
    Contact *contacts;
    int contact_count;
} PhoneBook;

PhoneBook *phonebook_create(void);
int phonebook_destroy(PhoneBook *phonebook);
int phonebook_add_contact(PhoneBook *phonebook, Contact *contact);
int phonebook_remove_contact(PhoneBook *phonebook, const char *contact_id);

#endif //PHONEBOOK_H
