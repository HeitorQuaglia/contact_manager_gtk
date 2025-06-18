#include "ui/contacts_view.h"
#include "contact_object.h"

struct _ContactsView {
    AdwBin parent_instance;

    PhoneBookObject *phonebook_object;
    GListStore *contacts_store;

    GtkWidget *contacts_list_view;
    GtkWidget *delete_contact_button;
};

G_DEFINE_TYPE(ContactsView, contacts_view, ADW_TYPE_BIN);

static void on_add_contact_clicked(GtkButton *button, ContactsView *view) {
    phonebook_object_add_new_contact(view->phonebook_object);
}

static void on_delete_contact_clicked(GtkButton *button, ContactsView *view) {
    GtkSingleSelection *selection = GTK_SINGLE_SELECTION(gtk_list_view_get_model(GTK_LIST_VIEW(view->contacts_list_view)));
    guint selected_pos = gtk_single_selection_get_selected(selection);

    if (selected_pos != GTK_INVALID_LIST_POSITION) {
        ContactObject *selected_contact = g_list_model_get_item(gtk_single_selection_get_model(selection), selected_pos);
        if (selected_contact) {
            phonebook_object_remove_contact(view->phonebook_object, selected_contact);
            g_object_unref(selected_contact);
        }
    }
}

static void on_selection_changed(GtkSingleSelection *selection, GParamSpec *pspec, ContactsView *view) {
    gboolean selected = gtk_single_selection_get_selected(selection) != GTK_INVALID_LIST_POSITION;
    gtk_widget_set_sensitive(view->delete_contact_button, selected);
}

static void setup_list_item(GtkSignalListItemFactory *factory, GtkListItem *list_item) {
    GtkWidget *label = gtk_label_new(NULL);
    gtk_list_item_set_child(list_item, label);
}

static void bind_list_item(GtkSignalListItemFactory *factory, GtkListItem *list_item) {
    GtkWidget *label = gtk_list_item_get_child(list_item);
    GObject *item = gtk_list_item_get_item(list_item);
    g_object_bind_property(item, "name", label, "label", G_BINDING_SYNC_CREATE);
}

static void contacts_view_init(ContactsView *view) {
    gtk_widget_init_template(GTK_WIDGET(view));
    gtk_widget_set_sensitive(view->delete_contact_button, FALSE);
}

static void contacts_view_class_init(ContactsViewClass *klass) {
    GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);
    gtk_widget_class_set_template_from_resource(widget_class, "/org/example/contact-manager/contacts_view.ui");

    gtk_widget_class_bind_template_child(widget_class, ContactsView, contacts_list_view);
    gtk_widget_class_bind_template_child(widget_class, ContactsView, delete_contact_button);

    gtk_widget_class_bind_template_callback(widget_class, on_add_contact_clicked);
    gtk_widget_class_bind_template_callback(widget_class, on_delete_contact_clicked);
}

GtkWidget *contacts_view_new(PhoneBookObject *phonebook_object) {
    ContactsView *view = g_object_new(CONTACTS_VIEW_TYPE, NULL);
    view->phonebook_object = g_object_ref(phonebook_object);
    view->contacts_store = phonebook_object_get_contacts(phonebook_object);

    GtkListItemFactory *factory = gtk_signal_list_item_factory_new();
    g_signal_connect(factory, "setup", G_CALLBACK(setup_list_item), NULL);
    g_signal_connect(factory, "bind", G_CALLBACK(bind_list_item), NULL);

    GtkSingleSelection *selection_model = gtk_single_selection_new(G_LIST_MODEL(view->contacts_store));
    gtk_list_view_set_model(GTK_LIST_VIEW(view->contacts_list_view), GTK_SELECTION_MODEL(selection_model));
    gtk_list_view_set_factory(GTK_LIST_VIEW(view->contacts_list_view), factory);
    g_object_unref(view->contacts_store);

    g_signal_connect(selection_model, "notify::selected", G_CALLBACK(on_selection_changed), view);
    
    return GTK_WIDGET(view);
} 