#ifndef ADDRESS_H
#define ADDRESS_H

typedef struct {
    char *address_id;
    char *address_line1;
    char *address_line2;
    char *city;
    char *state;
    char *zip_code;
} Address;

Address *address_create(void);
int address_destroy(Address *address);
int address_set_line1(Address *address, const char *line1);
int address_set_line2(Address *address, const char *line2);
int address_set_city(Address *address, const char *city);
int address_set_state(Address *address, const char *state);
int address_set_zip_code(Address *address, const char *zip_code);

#endif //ADDRESS_H
