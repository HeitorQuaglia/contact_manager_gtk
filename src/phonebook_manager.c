#include "phonebook_manager.h"
#include "model.h"
#include <stdlib.h>
#include <string.h>

PhoneBookManager *phonebook_manager_new(void) {
    PhoneBookManager *manager = g_new(PhoneBookManager, 1);
    manager->phonebooks = g_list_store_new(PHONEBOOK_TYPE_OBJECT);
    return manager;
}

void phonebook_manager_destroy(PhoneBookManager *manager) {
    if (!manager) return;
    g_object_unref(manager->phonebooks);
    g_free(manager);
}

void phonebook_manager_add(PhoneBookManager *manager, PhoneBookObject *pb_obj) {
    g_list_store_append(manager->phonebooks, pb_obj);
}

void phonebook_manager_add_new(PhoneBookManager *manager) {
    PhoneBook *pb = phonebook_create();
    if (pb) {
        PhoneBookObject *pb_obj = phonebook_object_new(pb);
        phonebook_manager_add(manager, pb_obj);
        g_object_unref(pb_obj);
    }
}

void phonebook_manager_remove(PhoneBookManager *manager, PhoneBookObject *pb_obj) {
    g_list_store_remove(manager->phonebooks, g_list_store_find(manager->phonebooks, pb_obj, NULL));
}

void phonebook_manager_load_all(PhoneBookManager *manager) {
    // TODO: Implement loading from a file
    g_print("TODO: Load all phonebooks from file\n");
}

void phonebook_manager_save_all(PhoneBookManager *manager) {
    // TODO: Implement saving to a file
    g_print("TODO: Save all phonebooks to file\n");
} 