#include "phone.h"
#include "model.h"
#include "util.h"
#include <stdlib.h>
#include <string.h>
#include <uuid/uuid.h>

Phone *phone_create(void) {
    Phone *phone = calloc(1, sizeof(Phone));
    if (!phone) {
        return NULL;
    }
    phone->phone_id = malloc(UUID_STR_LEN);
    if (generate_uuid(phone->phone_id) != UUID_GEN_SUCCESS) {
        free(phone->phone_id);
        free(phone);
        return NULL;
    }
    return phone;
}

int phone_destroy(Phone *phone) {
    if (!phone) {
        return MODEL_EMPTY_RESOURCE;
    }
    free(phone->number);
    free(phone->phone_id);
    free(phone);
    return MODEL_OP_SUCCESS;
}
