#ifndef PHONE_H
#define PHONE_H
typedef struct {
    char *phone_id;
    char *area_code;
    char *number;
} Phone;

Phone *phone_create(void);
int phone_destroy(Phone *phone);
int phone_set_area_code(Phone *phone, const char *area_code);
int phone_set_number(Phone *phone, const char *number);
#endif //PHONE_H
