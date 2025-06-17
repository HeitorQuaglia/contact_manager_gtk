#include "phone.h"

#include <stdlib.h>
#include <string.h>
#include <uuid/uuid.h>

#include "model.h"
#include "util.h"

Phone *phone_create(void) {
    Phone *phone = malloc(sizeof(Phone));
    if (!phone) {
        return NULL;
    }

    phone->area_code = NULL;
    phone->number = NULL;

    phone->phone_id = malloc(UUID_STR_LEN);
    if (!phone->phone_id || generate_uuid(phone->phone_id) != UUID_GEN_SUCCESS) {
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

    free(phone->area_code);
    free(phone->number);
    free(phone->phone_id);

    return MODEL_OP_SUCCESS;
}

int phone_set_area_code(Phone *phone, const char *area_code) {
    if (!phone || !area_code) {
        return MODEL_EMPTY_RESOURCE;
    }

    char *new_area_code = strdup(area_code);
    if (!new_area_code) {
        return MODEL_REALLOC_ERR;
    }

    free(phone->area_code);
    phone->area_code = new_area_code;

    return MODEL_OP_SUCCESS;
}

int phone_set_number(Phone *phone, const char *number) {
    if (!phone || !number) {
        return MODEL_EMPTY_RESOURCE;
    }

    char *new_number = strdup(number);
    if (!new_number) {
        return MODEL_REALLOC_ERR;
    }

    free(phone->number);
    phone->number = new_number;

    return MODEL_OP_SUCCESS;
}
