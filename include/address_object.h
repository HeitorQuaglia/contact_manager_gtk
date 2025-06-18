#ifndef ADDRESS_OBJECT_H
#define ADDRESS_OBJECT_H

#include <glib-object.h>
#include "address.h"

#define ADDRESS_TYPE_OBJECT (address_object_get_type())
G_DECLARE_FINAL_TYPE(AddressObject, address_object, ADDRESS, OBJECT, GObject)

AddressObject *address_object_new(Address *address);

const char *address_object_get_id(AddressObject *self);
const char *address_object_get_street(AddressObject *self);

#endif //ADDRESS_OBJECT_H 