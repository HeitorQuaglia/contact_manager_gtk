#include "phonebook_manager.h"
#include "model.h"
#include <stdlib.h>
#include <string.h>

PhoneBookManager *phonebook_manager_new(void) {
    PhoneBookManager *manager = malloc(sizeof(PhoneBookManager));
    if (!manager) {
        return NULL;
    }
    manager->phonebooks = NULL;
    manager->count = 0;
    return manager;
}

void phonebook_manager_destroy(PhoneBookManager *manager) {
    if (!manager) {
        return;
    }
    for (int i = 0; i < manager->count; i++) {
        phonebook_destroy(manager->phonebooks[i]);
    }
    free(manager->phonebooks);
    free(manager);
}

int phonebook_manager_add(PhoneBookManager *manager, PhoneBook *phonebook) {
    if (!manager || !phonebook) {
        return MODEL_EMPTY_RESOURCE;
    }

    PhoneBook **reallocated_phonebooks = realloc(manager->phonebooks, sizeof(PhoneBook *) * (manager->count + 1));
    if (!reallocated_phonebooks) {
        return MODEL_REALLOC_ERR;
    }

    manager->phonebooks = reallocated_phonebooks;
    manager->phonebooks[manager->count] = phonebook;
    manager->count++;

    return MODEL_OP_SUCCESS;
}

int phonebook_manager_remove(PhoneBookManager *manager, const char *phonebook_id) {
    if (!manager || !phonebook_id) {
        return MODEL_EMPTY_RESOURCE;
    }

    int index = -1;
    for (int i = 0; i < manager->count; i++) {
        if (strcmp(manager->phonebooks[i]->phonebook_id, phonebook_id) == 0) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        return MODEL_RESOURCE_NOT_FOUND;
    }

    phonebook_destroy(manager->phonebooks[index]);

    for (int i = index; i < manager->count - 1; i++) {
        manager->phonebooks[i] = manager->phonebooks[i + 1];
    }

    manager->count--;
    if (manager->count == 0) {
        free(manager->phonebooks);
        manager->phonebooks = NULL;
    } else {
        PhoneBook **reallocated_phonebooks = realloc(manager->phonebooks, sizeof(PhoneBook *) * manager->count);
        if (!reallocated_phonebooks) {
            // This is problematic, the manager is in an inconsistent state.
            // For this app, we'll accept the memory leak of the old pointer.
            return MODEL_REALLOC_ERR;
        }
        manager->phonebooks = reallocated_phonebooks;
    }

    return MODEL_OP_SUCCESS;
}

PhoneBook *phonebook_manager_find(PhoneBookManager *manager, const char *phonebook_id) {
    if (!manager || !phonebook_id) {
        return NULL;
    }

    for (int i = 0; i < manager->count; i++) {
        if (strcmp(manager->phonebooks[i]->phonebook_id, phonebook_id) == 0) {
            return manager->phonebooks[i];
        }
    }

    return NULL;
} 