#include "address.h"

#include <stdlib.h>
#include <string.h>
#include <uuid/uuid.h>

#include "model.h"
#include "util.h"

Address *address_create(void) {
    Address *address = malloc(sizeof(Address));
    if (!address) {
        return NULL;
    }

    address->address_line1 = NULL;
    address->address_line2 = NULL;
    address->city = NULL;
    address->state = NULL;
    address->zip_code = NULL;

    address->address_id = malloc(UUID_STR_LEN);
    if (!address->address_id || generate_uuid(address->address_id) != UUID_GEN_SUCCESS) {
        free(address->address_id);
        free(address);
        return NULL;
    }

    return address;
}

int address_destroy(Address *address) {
    if (!address) {
        return MODEL_EMPTY_RESOURCE;
    }

    free(address->address_line1);
    free(address->address_line2);
    free(address->city);
    free(address->state);
    free(address->zip_code);
    free(address->address_id);

    return MODEL_OP_SUCCESS;
}

int address_set_line1(Address *address, const char *line1) {
    if (!address || !line1) {
        return MODEL_EMPTY_RESOURCE;
    }

    char *new_line1 = strdup(line1);
    if (!new_line1) {
        return MODEL_REALLOC_ERR;
    }

    free(address->address_line1);
    address->address_line1 = new_line1;

    return MODEL_OP_SUCCESS;
}

int address_set_line2(Address *address, const char *line2) {
    if (!address || !line2) {
        return MODEL_EMPTY_RESOURCE;
    }

    char *new_line2 = strdup(line2);
    if (!new_line2) {
        return MODEL_REALLOC_ERR;
    }

    free(address->address_line2);
    address->address_line2 = new_line2;

    return MODEL_OP_SUCCESS;
}

int address_set_city(Address *address, const char *city) {
    if (!address || !city) {
        return MODEL_EMPTY_RESOURCE;
    }

    char *new_city = strdup(city);
    if (!new_city) {
        return MODEL_REALLOC_ERR;
    }

    free(address->city);
    address->city = new_city;

    return MODEL_OP_SUCCESS;
}

int address_set_state(Address *address, const char *state) {
    if (!address || !state) {
        return MODEL_EMPTY_RESOURCE;
    }

    char *new_state = strdup(state);
    if (!new_state) {
        return MODEL_REALLOC_ERR;
    }

    free(address->state);
    address->state = new_state;

    return MODEL_OP_SUCCESS;
}

int address_set_zip_code(Address *address, const char *zip_code) {
    if (!address || !zip_code) {
        return MODEL_EMPTY_RESOURCE;
    }

    char *new_zip = strdup(zip_code);
    if (!new_zip) {
        return MODEL_REALLOC_ERR;
    }

    free(address->zip_code);
    address->zip_code = new_zip;

    return MODEL_OP_SUCCESS;
}
