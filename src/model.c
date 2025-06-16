#include "model.h"
#include <stdlib.h>

PhoneBook *phonebook_create(void) {
    PhoneBook *phonebook = malloc(sizeof(PhoneBook));
    if (!phonebook) {
        return NULL;
    }
    phonebook->contacts = NULL;
    phonebook->contact_count = 0;
    phonebook->phonebook_id = 0;
    return phonebook;
}

int phonebook_destroy(PhoneBook *phonebook) {
    if (!phonebook) {
        return MODEL_EMPTY_RESOURCE;
    }

    for (int i = 0; i < phonebook->contact_count; i++) {
        Contact *c = &phonebook->contacts[i];

        for (int j = 0; j < c->phone_count; j++) {
            phone_destroy(&c->phones[j]);
        }
        free(c->phones);

        for (int j = 0; j < c->address_count; j++) {
            address_destroy(&c->address[j]);
        }
        free(c->address);

        free(c->name);
        free(c->email);
    }

    free(phonebook->contacts);
    free(phonebook);

    return MODEL_OP_SUCCESS;
}

int phonebook_add_contact(PhoneBook *phonebook, Contact *contact) {
    if (!phonebook || !contact) {
        return MODEL_EMPTY_RESOURCE;
    }

    Contact *reallocated_contacts = realloc(
        phonebook->contacts,
        sizeof(Contact) * (phonebook->contact_count + 1)
    );

    if (!reallocated_contacts) {
        return MODEL_REALLOC_ERR;
    }

    phonebook->contacts = reallocated_contacts;
    phonebook->contacts[phonebook->contact_count] = *contact;
    phonebook->contact_count++;

    return MODEL_OP_SUCCESS;
}

int phonebook_remove_contact(PhoneBook *phonebook, int index) {
    if (!phonebook || index < 0 || index >= phonebook->contact_count) {
        return MODEL_OP_ERR;
    }

    Contact *c = &phonebook->contacts[index];

    for (int j = 0; j < c->phone_count; j++) {
        phone_destroy(&c->phones[j]);
    }
    free(c->phones);

    for (int j = 0; j < c->address_count; j++) {
        address_destroy(&c->address[j]);
    }
    free(c->address);

    free(c->name);
    free(c->email);

    for (int i = index; i < phonebook->contact_count - 1; i++) {
        phonebook->contacts[i] = phonebook->contacts[i + 1];
    }

    size_t new_size = phonebook->contact_count - 1;
    Contact *reallocated_contacts = (new_size > 0)
        ? realloc(phonebook->contacts, sizeof(Contact) * new_size)
        : NULL;

    if (!reallocated_contacts && new_size > 0) {
        return MODEL_REALLOC_ERR;
    }

    phonebook->contacts = reallocated_contacts;
    phonebook->contact_count--;

    return MODEL_OP_SUCCESS;
}
