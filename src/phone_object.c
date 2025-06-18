#include "phone_object.h"

struct _PhoneObject {
    GObject parent_instance;
    Phone *phone;
};

enum {
    PROP_0,
    PROP_ID,
    PROP_NUMBER,
    N_PROPERTIES
};

static GParamSpec *obj_properties[N_PROPERTIES] = { NULL, };

G_DEFINE_TYPE(PhoneObject, phone_object, G_TYPE_OBJECT)

static void phone_object_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec) {
    PhoneObject *self = PHONE_OBJECT(object);
    switch (property_id) {
        case PROP_ID:
            g_value_set_string(value, self->phone->phone_id);
            break;
        case PROP_NUMBER:
            g_value_set_string(value, self->phone->number);
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
            break;
    }
}

static void phone_object_dispose(GObject *object) {
    PhoneObject *self = PHONE_OBJECT(object);
    if (self->phone) {
        phone_destroy(self->phone);
        self->phone = NULL;
    }
    G_OBJECT_CLASS(phone_object_parent_class)->dispose(object);
}

static void phone_object_class_init(PhoneObjectClass *klass) {
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    object_class->dispose = phone_object_dispose;
    object_class->get_property = phone_object_get_property;

    obj_properties[PROP_ID] = g_param_spec_string("id", "ID", "Phone ID", NULL, G_PARAM_READABLE);
    obj_properties[PROP_NUMBER] = g_param_spec_string("number", "Number", "Phone Number", NULL, G_PARAM_READABLE);
    g_object_class_install_properties(object_class, N_PROPERTIES, obj_properties);
}

static void phone_object_init(PhoneObject *self) {}

PhoneObject *phone_object_new(Phone *phone) {
    PhoneObject *obj = g_object_new(PHONE_TYPE_OBJECT, NULL);
    obj->phone = phone;
    return obj;
}

const char *phone_object_get_id(PhoneObject *self) {
    g_return_val_if_fail(PHONE_IS_OBJECT(self), NULL);
    return self->phone->phone_id;
}

const char *phone_object_get_number(PhoneObject *self) {
    g_return_val_if_fail(PHONE_IS_OBJECT(self), NULL);
    return self->phone->number;
} 