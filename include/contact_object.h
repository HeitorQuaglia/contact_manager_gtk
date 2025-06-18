#ifndef CONTACT_OBJECT_H
#define CONTACT_OBJECT_H

#include <glib-object.h>
#include "contact.h"

#define CONTACT_TYPE_OBJECT (contact_object_get_type())
G_DECLARE_FINAL_TYPE(ContactObject, contact_object, CONTACT, OBJECT, GObject)

ContactObject *contact_object_new(Contact *contact);

const char *contact_object_get_id(ContactObject *self);
const char *contact_object_get_name(ContactObject *self);
Contact *contact_object_get_contact(ContactObject *self);

#endif //CONTACT_OBJECT_H 