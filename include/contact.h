#ifndef CONTACT_H
#define CONTACT_H
#include "address.h"
#include "phone.h"

struct PhoneBook;

typedef struct Contact {
    char *contact_id;
    char *name;
    char *email;
    Phone *phones;
    int phone_count;
    Address *address;
    int address_count;
} Contact;

Contact *contact_create(void);
int contact_destroy(Contact *contact);

#endif //CONTACT_H
