#include "phonebook_object.h"

struct _PhoneBookObject {
    GObject parent_instance;
    PhoneBook *phonebook;
};

enum {
    PROP_0,
    PROP_NAME,
    N_PROPERTIES
};

static GParamSpec *obj_properties[N_PROPERTIES] = { NULL, };

G_DEFINE_TYPE(PhoneBookObject, phonebook_object, G_TYPE_OBJECT)

static void phonebook_object_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec) {
    PhoneBookObject *self = PHONEBOOK_OBJECT(object);

    switch (property_id) {
        case PROP_NAME:
            phonebook_object_set_name(self, g_value_get_string(value));
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
            break;
    }
}

static void phonebook_object_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec) {
    PhoneBookObject *self = PHONEBOOK_OBJECT(object);

    switch (property_id) {
        case PROP_NAME:
            g_value_set_string(value, self->phonebook->name);
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
            break;
    }
}

static void phonebook_object_dispose(GObject *object) {
    PhoneBookObject *self = PHONEBOOK_OBJECT(object);
    if (self->phonebook) {
        phonebook_destroy(self->phonebook);
        self->phonebook = NULL;
    }
    G_OBJECT_CLASS(phonebook_object_parent_class)->dispose(object);
}

static void phonebook_object_class_init(PhoneBookObjectClass *klass) {
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    object_class->dispose = phonebook_object_dispose;
    object_class->set_property = phonebook_object_set_property;
    object_class->get_property = phonebook_object_get_property;

    obj_properties[PROP_NAME] = g_param_spec_string("name", "Name", "Phonebook Name", NULL, G_PARAM_READWRITE);
    g_object_class_install_properties(object_class, N_PROPERTIES, obj_properties);
}

static void phonebook_object_init(PhoneBookObject *self) {}

PhoneBookObject *phonebook_object_new(PhoneBook *pb) {
    PhoneBookObject *obj = g_object_new(PHONEBOOK_TYPE_OBJECT, NULL);
    obj->phonebook = pb;
    return obj;
}

PhoneBook *phonebook_object_get_phonebook(PhoneBookObject *self) {
    g_return_val_if_fail(PHONEBOOK_IS_OBJECT(self), NULL);
    return self->phonebook;
}

void phonebook_object_set_name(PhoneBookObject *self, const char *name) {
    g_return_if_fail(PHONEBOOK_IS_OBJECT(self));
    if (g_strcmp0(self->phonebook->name, name) != 0) {
        g_free(self->phonebook->name);
        self->phonebook->name = g_strdup(name);
        g_object_notify_by_pspec(G_OBJECT(self), obj_properties[PROP_NAME]);
    }
}

void phonebook_object_add_new_contact(PhoneBookObject *self) {
    g_return_if_fail(PHONEBOOK_IS_OBJECT(self));
    Contact *c = contact_create();
    ContactObject *co = contact_object_new(c);
    g_list_store_append(self->phonebook->contacts, co);
    g_object_unref(co);
}

void phonebook_object_remove_contact(PhoneBookObject *self, ContactObject *contact) {
    g_return_if_fail(PHONEBOOK_IS_OBJECT(self));
    guint position;
    if (g_list_store_find(self->phonebook->contacts, contact, &position)) {
        g_list_store_remove(self->phonebook->contacts, position);
    }
}

GListStore *phonebook_object_get_contacts(PhoneBookObject *self) {
    g_return_val_if_fail(PHONEBOOK_IS_OBJECT(self), NULL);
    return g_object_ref(self->phonebook->contacts);
} 