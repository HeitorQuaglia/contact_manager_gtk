#ifndef MODEL_H
#define MODEL_H

typedef struct {
    int address_id;
    char *adress_line1;
    char *adress_line2;
    char *city;
    char *state;
    char *zip_code;
} Address;

typedef struct {
    int phone_id;
    char *area_code;
    char *number;
} Phone;

typedef struct {
    int contact_id;
    char *name;
    char *email;
    Phone *phones;
    int phone_count;
    Address *address;
    int address_count;
} Contact;

typedef struct {
    int phonebook_id;
    Contact *contacts;
    int contact_count;
} PhoneBook;

#define MODEL_EMPTY_RESOURCE -4
#define MODEL_DEALLOC_ERR -3
#define MODEL_REALLOC_ERR -2
#define MODEL_OP_ERR -1
#define MODEL_OP_SUCCESS 0

/* PhoneBook functions */
PhoneBook *phonebook_create(void);
int phonebook_destroy(PhoneBook *phonebook);
int phonebook_add_contact(PhoneBook *phonebook, Contact *contact);
int phonebook_remove_contact(PhoneBook *phonebook, int contact_id);

/* Contact functions */
Contact *contact_create(void);
Contact *find_contact(PhoneBook *phonebook, int contact_id);
int contact_clear_phones(Contact *contact);
int contact_clear_addresses(Contact *contact);
int contact_destroy(Contact *contact);
int contact_add_phone(Contact *contact, Phone *phone);
int contact_remove_phone(Contact *contact, int phone_id);
int contact_add_address(Contact *contact, Address *address);
int contact_remove_address(Contact *contact, int address_id);

/* Phone functions */
Phone *phone_create(void);
int phone_destroy(Phone *phone);
int phone_set_area_code(Phone *phone, const char *area_code);
int phone_set_number(Phone *phone, const char *number);

/* Address functions */
Address *address_create(void);
int address_destroy(Address *address);
int address_set_line1(Address *address, const char *line1);
int address_set_line2(Address *address, const char *line2);
int address_set_city(Address *address, const char *city);
int address_set_state(Address *address, const char *state);
int address_set_zip_code(Address *address, const char *zip_code);

#endif //MODEL_H
