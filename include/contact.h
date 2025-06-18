#ifndef CONTACT_H
#define CONTACT_H

#include <glib.h>
#include "phone_object.h"
#include "address_object.h"

typedef struct Contact {
    char *contact_id;
    char *name;
    char *email;
    GPtrArray *phones;
    GPtrArray *addresses;
} Contact;

Contact *contact_create(void);
int contact_destroy(Contact *contact);

void contact_add_phone(Contact *contact, PhoneObject *phone);
gboolean contact_remove_phone(Contact *contact, const char *phone_id);

void contact_add_address(Contact *contact, AddressObject *address);
gboolean contact_remove_address(Contact *contact, const char *address_id);

#endif //CONTACT_H
