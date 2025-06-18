#ifndef PHONE_H
#define PHONE_H

typedef struct {
    char *phone_id;
    char *number;
} Phone;

Phone *phone_create(void);
int phone_destroy(Phone *phone);

#endif //PHONE_H
