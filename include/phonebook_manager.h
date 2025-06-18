#ifndef PHONEBOOK_MANAGER_H
#define PHONEBOOK_MANAGER_H

#include <gio/gio.h>
#include "phonebook_object.h"

typedef struct {
    GListStore *phonebooks;
} PhoneBookManager;

PhoneBookManager *phonebook_manager_new(void);
void phonebook_manager_destroy(PhoneBookManager *manager);
void phonebook_manager_add(PhoneBookManager *manager, PhoneBookObject *phonebook);
void phonebook_manager_add_new(PhoneBookManager *manager);
void phonebook_manager_remove(PhoneBookManager *manager, PhoneBookObject *phonebook);
void phonebook_manager_load_all(PhoneBookManager *manager);
void phonebook_manager_save_all(PhoneBookManager *manager);

#endif //PHONEBOOK_MANAGER_H 