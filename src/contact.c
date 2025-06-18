#include "contact.h"

#include <stdlib.h>
#include <string.h>
#include <uuid/uuid.h>

#include "util.h"
#include "model.h"

Contact *contact_create(void) {
    Contact *contact = calloc(1, sizeof(Contact));
    if (!contact) {
        return NULL;
    }

    contact->contact_id = malloc(UUID_STR_LEN);
    if (generate_uuid(contact->contact_id) != UUID_GEN_SUCCESS) {
        free(contact->contact_id);
        free(contact);
        return NULL;
    }
    return contact;
}

static int contact_clear_phones(Contact *contact) {
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

static int contact_clear_addresses(Contact *contact) {
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
