#ifndef ADDRESS_H
#define ADDRESS_H

typedef struct {
    char *address_id;
    char *street;
    char *number;
    char *city;
    char *state;
    char *zip_code;
} Address;

Address *address_create(void);
int address_destroy(Address *address);

#endif //ADDRESS_H
