#include "contact_object.h"

struct _ContactObject {
    GObject parent_instance;
    Contact *contact;
};

enum {
    PROP_0,
    PROP_ID,
    PROP_NAME,
    N_PROPERTIES
};

static GParamSpec *obj_properties[N_PROPERTIES] = { NULL, };

G_DEFINE_TYPE(ContactObject, contact_object, G_TYPE_OBJECT)

static void contact_object_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec) {
    // Read-only properties
}

static void contact_object_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec) {
    ContactObject *self = CONTACT_OBJECT(object);
    switch (property_id) {
        case PROP_ID:
            g_value_set_string(value, self->contact->contact_id);
            break;
        case PROP_NAME:
            g_value_set_string(value, self->contact->name);
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
            break;
    }
}

static void contact_object_dispose(GObject *object) {
    ContactObject *self = CONTACT_OBJECT(object);
    if (self->contact) {
        contact_destroy(self->contact);
        self->contact = NULL;
    }
    G_OBJECT_CLASS(contact_object_parent_class)->dispose(object);
}

static void contact_object_class_init(ContactObjectClass *klass) {
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    object_class->dispose = contact_object_dispose;
    object_class->set_property = contact_object_set_property;
    object_class->get_property = contact_object_get_property;

    obj_properties[PROP_ID] = g_param_spec_string("id", "ID", "Contact ID", NULL, G_PARAM_READABLE);
    obj_properties[PROP_NAME] = g_param_spec_string("name", "Name", "Contact Name", NULL, G_PARAM_READABLE);
    g_object_class_install_properties(object_class, N_PROPERTIES, obj_properties);
}

static void contact_object_init(ContactObject *self) {
    // Initialization
}

ContactObject *contact_object_new(Contact *contact) {
    ContactObject *obj = g_object_new(CONTACT_TYPE_OBJECT, NULL);
    obj->contact = contact;
    return obj;
}

const char *contact_object_get_id(ContactObject *self) {
    g_return_val_if_fail(CONTACT_IS_OBJECT(self), NULL);
    return self->contact->contact_id;
}

const char *contact_object_get_name(ContactObject *self) {
    g_return_val_if_fail(CONTACT_IS_OBJECT(self), NULL);
    return self->contact->name;
} 