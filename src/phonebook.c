#include <stdlib.h>
#include <string.h>
#include <uuid/uuid.h>
#include <glib.h>

#include "contact.h"
#include "model.h"
#include "phone.h"
#include "util.h"
#include "phonebook.h"

PhoneBook *phonebook_create(void) {
    PhoneBook *phonebook = g_new(PhoneBook, 1);
    phonebook->phonebook_id = g_malloc(UUID_STR_LEN);
    if (generate_uuid(phonebook->phonebook_id) != UUID_GEN_SUCCESS) {
        g_free(phonebook->phonebook_id);
        g_free(phonebook);
        return NULL;
    }
    phonebook->contacts = g_ptr_array_new_with_free_func(g_object_unref);
    return phonebook;
}

void phonebook_destroy(PhoneBook *phonebook) {
    if (!phonebook) return;
    g_free(phonebook->phonebook_id);
    g_ptr_array_free(phonebook->contacts, TRUE);
    g_free(phonebook);
}

void phonebook_add_contact(PhoneBook *phonebook, ContactObject *contact) {
    g_return_if_fail(phonebook != NULL && contact != NULL);
    g_ptr_array_add(phonebook->contacts, g_object_ref(contact));
}

gboolean phonebook_remove_contact(PhoneBook *phonebook, const char *contact_id) {
    g_return_val_if_fail(phonebook != NULL && contact_id != NULL, FALSE);

    for (guint i = 0; i < phonebook->contacts->len; i++) {
        ContactObject *contact = g_ptr_array_index(phonebook->contacts, i);
        if (g_strcmp0(contact_object_get_id(contact), contact_id) == 0) {
            g_ptr_array_remove_index(phonebook->contacts, i);
            return TRUE;
        }
    }
    return FALSE;
}
