#include "address.h"
#include "model.h"
#include "util.h"
#include <stdlib.h>
#include <string.h>
#include <uuid/uuid.h>

Address *address_create(void) {
    Address *address = calloc(1, sizeof(Address));
    if (!address) {
        return NULL;
    }
    address->address_id = malloc(UUID_STR_LEN);
    if (generate_uuid(address->address_id) != UUID_GEN_SUCCESS) {
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
    free(address->street);
    free(address->number);
    free(address->city);
    free(address->state);
    free(address->zip_code);
    free(address->address_id);
    free(address);
    return MODEL_OP_SUCCESS;
}
