#ifndef PHONEBOOK_H
#define PHONEBOOK_H

struct Contact;

typedef struct PhoneBook {
    char *phonebook_id;
    struct Contact *contacts;
    int contact_count;
} PhoneBook;

PhoneBook *phonebook_create(void);
int phonebook_destroy(PhoneBook *phonebook);
int phonebook_add_contact(PhoneBook *phonebook, struct Contact *contact);
int phonebook_remove_contact(PhoneBook *phonebook, const char *contact_id);

#endif //PHONEBOOK_H
