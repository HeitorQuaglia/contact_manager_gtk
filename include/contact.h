#ifndef CONTACT_H
#define CONTACT_H
#include "address.h"
#include "phone.h"
#include "phonebook.h"

typedef struct {
    char *contact_id;
    char *name;
    char *email;
    Phone *phones;
    int phone_count;
    Address *address;
    int address_count;
} Contact;

Contact *contact_create(void);
Contact *find_contact(PhoneBook *phonebook, char *contact_id);
int contact_clear_phones(Contact *contact);
int contact_clear_addresses(Contact *contact);
int contact_destroy(Contact *contact);
int contact_add_phone(Contact *contact, Phone *phone);
int contact_remove_phone(Contact *contact, const char *phone_id);
int contact_add_address(Contact *contact, Address *address);
int contact_remove_address(Contact *contact, const char *address_id);

#endif //CONTACT_H
