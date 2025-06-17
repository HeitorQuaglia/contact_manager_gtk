#include "contact.h"

#include <stdlib.h>
#include <string.h>
#include <uuid/uuid.h>

#include "address.h"
#include "phone.h"
#include "util.h"
#include "phonebook.h"
#include "model.h"

Contact *contact_create(void) {
    Contact *contact = malloc(sizeof(Contact));
    if (!contact) {
        return NULL;
    }
    contact->name = NULL;
    contact->email = NULL;
    contact->phones = NULL;
    contact->phone_count = 0;
    contact->address = NULL;
    contact->address_count = 0;
    contact->contact_id = malloc(UUID_STR_LEN * sizeof(char));
    if (generate_uuid(contact->contact_id)) {
        free(contact->contact_id);
        free(contact);
        return NULL;
    }
    return contact;
}

Contact *find_contact(struct PhoneBook *phonebook, const char *contact_id) {
    if (!phonebook || !contact_id) {
        return NULL;
    }

    for (int i = 0; i < phonebook->contact_count; i++) {
        if (strcmp(phonebook->contacts[i].contact_id, contact_id) == 0) {
            return &phonebook->contacts[i];
        }
    }

    return NULL;
}

int contact_clear_phones(Contact *contact) {
    if (!contact) {
        return MODEL_EMPTY_RESOURCE;
    }

    for (int i = 0; i < contact->phone_count; i++) {
        phone_destroy(&contact->phones[i]);
    }
    free(contact->phones);
    contact->phones = NULL;
    contact->phone_count = 0;

    return MODEL_OP_SUCCESS;
}

int contact_clear_addresses(Contact *contact) {
    if (!contact) {
        return MODEL_EMPTY_RESOURCE;
    }

    for (int i = 0; i < contact->address_count; i++) {
        address_destroy(&contact->address[i]);
    }
    free(contact->address);
    contact->address = NULL;
    contact->address_count = 0;

    return MODEL_OP_SUCCESS;
}

int contact_destroy(Contact *contact) {
    if (!contact) {
        return MODEL_EMPTY_RESOURCE;
    }

    contact_clear_phones(contact);
    contact_clear_addresses(contact);

    free(contact->name);
    free(contact->email);
    free(contact->contact_id);
    free(contact);

    return MODEL_OP_SUCCESS;
}

int contact_add_phone(Contact *contact, Phone *phone) {
    if (!contact || !phone) {
        return MODEL_EMPTY_RESOURCE;
    }

    Phone *reallocated_phones = realloc(
        contact->phones,
        sizeof(Phone) * (contact->phone_count + 1)
    );

    if (!reallocated_phones) {
        return MODEL_REALLOC_ERR;
    }

    contact->phones = reallocated_phones;
    contact->phones[contact->phone_count] = *phone;
    contact->phone_count++;

    return MODEL_OP_SUCCESS;
}

int contact_remove_phone(Contact *contact, const char *phone_id) {
    if (!contact || !phone_id) {
        return MODEL_EMPTY_RESOURCE;
    }

    int index = -1;
    for (int i = 0; i < contact->phone_count; i++) {
        if (strcmp(contact->phones[i].phone_id, phone_id) == 0) {
            index = i;
            break;
        }
    }
    if (index == -1) {
        return MODEL_RESOURCE_NOT_FOUND;
    }

    phone_destroy(&contact->phones[index]);

    for (int i = index; i < contact->phone_count - 1; i++) {
        contact->phones[i] = contact->phones[i + 1];
    }

    const int new_count = contact->phone_count - 1;
    Phone *reallocated_phones = (new_count > 0)
                                    ? realloc(contact->phones, sizeof(Phone) * new_count)
                                    : NULL;

    if (!reallocated_phones && new_count > 0) {
        return MODEL_REALLOC_ERR;
    }

    contact->phones = reallocated_phones;
    contact->phone_count = new_count;

    return MODEL_OP_SUCCESS;
}

int contact_add_address(Contact *contact, Address *address) {
    if (!contact || !address) {
        return MODEL_EMPTY_RESOURCE;
    }

    Address *reallocated_address = realloc(
        contact->address,
        sizeof(Address) * (contact->address_count + 1)
    );

    if (!reallocated_address) {
        return MODEL_REALLOC_ERR;
    }

    contact->address = reallocated_address;
    contact->address[contact->address_count] = *address;
    contact->address_count++;

    return MODEL_OP_SUCCESS;
}

int contact_remove_address(Contact *contact, const char *address_id) {
    if (!contact || !address_id) {
        return MODEL_EMPTY_RESOURCE;
    }

    int index = -1;
    for (int i = 0; i < contact->address_count; i++) {
        if (strcmp(contact->address[i].address_id, address_id) == 0) {
            index = i;
            break;
        }
    }
    if (index == -1) {
        return MODEL_RESOURCE_NOT_FOUND;
    }
    address_destroy(&contact->address[index]);
    for (int i = index; i < contact->address_count - 1; i++) {
        contact->address[i] = contact->address[i + 1];
    }

    const int new_count = contact->address_count - 1;
    Address *reallocated_address = (new_count > 0)
                                       ? realloc(contact->address, sizeof(Address) * new_count)
                                       : NULL;

    if (!reallocated_address && new_count > 0) {
        return MODEL_REALLOC_ERR;
    }

    contact->address = reallocated_address;
    contact->address_count = new_count;

    return MODEL_OP_SUCCESS;
}
