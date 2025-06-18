#include "address_object.h"

struct _AddressObject {
    GObject parent_instance;
    Address *address;
};

enum {
    PROP_0,
    PROP_ID,
    PROP_STREET,
    N_PROPERTIES
};

static GParamSpec *obj_properties[N_PROPERTIES] = { NULL, };

G_DEFINE_TYPE(AddressObject, address_object, G_TYPE_OBJECT)

static void address_object_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec) {
    AddressObject *self = ADDRESS_OBJECT(object);
    switch (property_id) {
        case PROP_ID:
            g_value_set_string(value, self->address->address_id);
            break;
        case PROP_STREET:
            g_value_set_string(value, self->address->street);
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
            break;
    }
}

static void address_object_dispose(GObject *object) {
    AddressObject *self = ADDRESS_OBJECT(object);
    if (self->address) {
        address_destroy(self->address);
        self->address = NULL;
    }
    G_OBJECT_CLASS(address_object_parent_class)->dispose(object);
}

static void address_object_class_init(AddressObjectClass *klass) {
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    object_class->dispose = address_object_dispose;
    object_class->get_property = address_object_get_property;

    obj_properties[PROP_ID] = g_param_spec_string("id", "ID", "Address ID", NULL, G_PARAM_READABLE);
    obj_properties[PROP_STREET] = g_param_spec_string("street", "Street", "Street Name", NULL, G_PARAM_READABLE);
    g_object_class_install_properties(object_class, N_PROPERTIES, obj_properties);
}

static void address_object_init(AddressObject *self) {}

AddressObject *address_object_new(Address *address) {
    AddressObject *obj = g_object_new(ADDRESS_TYPE_OBJECT, NULL);
    obj->address = address;
    return obj;
}

const char *address_object_get_id(AddressObject *self) {
    g_return_val_if_fail(ADDRESS_IS_OBJECT(self), NULL);
    return self->address->address_id;
}

const char *address_object_get_street(AddressObject *self) {
    g_return_val_if_fail(ADDRESS_IS_OBJECT(self), NULL);
    return self->address->street;
} 