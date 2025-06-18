#ifndef PHONEBOOK_MANAGER_H
#define PHONEBOOK_MANAGER_H

#include "phonebook.h"

typedef struct {
    PhoneBook **phonebooks;
    int count;
} PhoneBookManager;

PhoneBookManager *phonebook_manager_new(void);
void phonebook_manager_destroy(PhoneBookManager *manager);
int phonebook_manager_add(PhoneBookManager *manager, PhoneBook *phonebook);
int phonebook_manager_remove(PhoneBookManager *manager, const char *phonebook_id);
PhoneBook *phonebook_manager_find(PhoneBookManager *manager, const char *phonebook_id);

#endif //PHONEBOOK_MANAGER_H 