#include "contact.h"

#include <stdlib.h>
#include <string.h>
#include <uuid/uuid.h>
#include "util.h"
#include "model.h"

Contact *contact_create(void) {
    Contact *contact = g_new(Contact, 1);

    contact->contact_id = malloc(UUID_STR_LEN);
    if (generate_uuid(contact->contact_id) != UUID_GEN_SUCCESS) {
        free(contact->contact_id);
        g_free(contact);
        return NULL;
    }
    contact->name = NULL;
    contact->email = NULL;
    contact->phones = g_ptr_array_new_with_free_func(g_object_unref);
    contact->addresses = g_ptr_array_new_with_free_func(g_object_unref);

    return contact;
}

int contact_destroy(Contact *contact) {
    if (!contact) {
        return MODEL_EMPTY_RESOURCE;
    }
    g_free(contact->name);
    g_free(contact->email);
    g_free(contact->contact_id);
    g_ptr_array_free(contact->phones, TRUE);
    g_ptr_array_free(contact->addresses, TRUE);
    g_free(contact);

    return MODEL_OP_SUCCESS;
}

void contact_add_phone(Contact *contact, PhoneObject *phone) {
    g_return_if_fail(contact != NULL && phone != NULL);
    g_ptr_array_add(contact->phones, g_object_ref(phone));
}

gboolean contact_remove_phone(Contact *contact, const char *phone_id) {
    g_return_val_if_fail(contact != NULL && phone_id != NULL, FALSE);
    for (guint i = 0; i < contact->phones->len; i++) {
        PhoneObject *phone = g_ptr_array_index(contact->phones, i);
        if (g_strcmp0(phone_object_get_id(phone), phone_id) == 0) {
            g_ptr_array_remove_index(contact->phones, i);
            return TRUE;
        }
    }
    return FALSE;
}

void contact_add_address(Contact *contact, AddressObject *address) {
    g_return_if_fail(contact != NULL && address != NULL);
    g_ptr_array_add(contact->addresses, g_object_ref(address));
}

gboolean contact_remove_address(Contact *contact, const char *address_id) {
    g_return_val_if_fail(contact != NULL && address_id != NULL, FALSE);
    for (guint i = 0; i < contact->addresses->len; i++) {
        AddressObject *address = g_ptr_array_index(contact->addresses, i);
        if (g_strcmp0(address_object_get_id(address), address_id) == 0) {
            g_ptr_array_remove_index(contact->addresses, i);
            return TRUE;
        }
    }
    return FALSE;
}
