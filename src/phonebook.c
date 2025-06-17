#include <stdlib.h>
#include <string.h>
#include <uuid/uuid.h>

#include "contact.h"
#include "model.h"
#include "phone.h"
#include "util.h"
#include "phonebook.h"

PhoneBook *phonebook_create(void) {
    PhoneBook *phonebook = malloc(sizeof(PhoneBook));
    if (!phonebook) {
        return NULL;
    }
    phonebook->contacts = NULL;
    phonebook->contact_count = 0;
    phonebook->phonebook_id = malloc(UUID_STR_LEN * sizeof(char));

    if (generate_uuid(phonebook->phonebook_id) != UUID_GEN_SUCCESS) {
        free(phonebook->phonebook_id);
        free(phonebook);
        return NULL;
    }

    return phonebook;
}

int phonebook_destroy(PhoneBook *phonebook) {
    if (!phonebook) {
        return MODEL_EMPTY_RESOURCE;
    }

    for (int i = 0; i < phonebook->contact_count; i++) {
        const struct Contact *c = &phonebook->contacts[i];

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
    free(phonebook->phonebook_id);
    free(phonebook);

    return MODEL_OP_SUCCESS;
}

int phonebook_add_contact(PhoneBook *phonebook, struct Contact *contact) {
    if (!phonebook || !contact) {
        return MODEL_EMPTY_RESOURCE;
    }

    struct Contact *reallocated_contacts = realloc(
        phonebook->contacts,
        sizeof(struct Contact) * (phonebook->contact_count + 1)
    );

    if (!reallocated_contacts) {
        return MODEL_REALLOC_ERR;
    }

    phonebook->contacts = reallocated_contacts;
    phonebook->contacts[phonebook->contact_count] = *contact;
    phonebook->contact_count++;

    return MODEL_OP_SUCCESS;
}

int phonebook_remove_contact(PhoneBook *phonebook, const char *contact_id) {
    if (!phonebook || !contact_id) {
        return MODEL_EMPTY_RESOURCE;
    }

    int index = -1;
    for (int i = 0; i < phonebook->contact_count; i++) {
        if (strcmp(phonebook->contacts[i].contact_id, contact_id) == 0) {
            index = i;
            break;
        }
    }
    if (index == -1) {
        return MODEL_RESOURCE_NOT_FOUND;
    }

    const struct Contact *c = &phonebook->contacts[index];

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
    free(c->contact_id);

    for (int i = index; i < phonebook->contact_count - 1; i++) {
        phonebook->contacts[i] = phonebook->contacts[i + 1];
    }

    const size_t new_size = phonebook->contact_count - 1;
    struct Contact *reallocated_contacts = (new_size > 0)
                                        ? realloc(phonebook->contacts, sizeof(struct Contact) * new_size)
                                        : NULL;

    if (!reallocated_contacts && new_size > 0) {
        return MODEL_REALLOC_ERR;
    }

    phonebook->contacts = reallocated_contacts;
    phonebook->contact_count = new_size;

    return MODEL_OP_SUCCESS;
}
