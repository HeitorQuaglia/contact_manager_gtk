#ifndef PHONE_OBJECT_H
#define PHONE_OBJECT_H

#include <glib-object.h>
#include "phone.h"

#define PHONE_TYPE_OBJECT (phone_object_get_type())
G_DECLARE_FINAL_TYPE(PhoneObject, phone_object, PHONE, OBJECT, GObject)

PhoneObject *phone_object_new(Phone *phone);

const char *phone_object_get_id(PhoneObject *self);
const char *phone_object_get_number(PhoneObject *self);

#endif //PHONE_OBJECT_H 