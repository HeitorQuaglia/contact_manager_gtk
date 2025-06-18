#include "contact_manager_window.h"
#include "contact_object.h"
#include <glib/gi18n.h>

struct _ContactManagerWindow {
    GtkApplicationWindow parent_instance;

    PhoneBook *phonebook;
    GListStore *contacts_store;

    GtkWidget *contacts_list_view;
    GtkWidget *delete_contact_button;
    GtkWidget *header_bar;
};

G_DEFINE_TYPE(ContactManagerWindow, contact_manager_window, GTK_TYPE_APPLICATION_WINDOW)

static void refresh_contacts_list(ContactManagerWindow *win) {
    g_list_store_remove_all(win->contacts_store);
    for (guint i = 0; i < win->phonebook->contacts->len; i++) {
        g_list_store_append(win->contacts_store, g_ptr_array_index(win->phonebook->contacts, i));
    }
}

static void on_new_contact_clicked(GtkButton *button, ContactManagerWindow *win) {
    Contact *c = contact_create();
    if (!c) return;

    c->name = g_strdup_printf("New Contact %d", win->phonebook->contacts->len + 1);
    
    ContactObject *contact_obj = contact_object_new(c);
    phonebook_add_contact(win->phonebook, contact_obj);
    g_object_unref(contact_obj); // phonebook has its own reference now

    refresh_contacts_list(win);
}

static void on_delete_contact_clicked(GtkButton *button, ContactManagerWindow *win) {
    GtkSingleSelection *selection = GTK_SINGLE_SELECTION(gtk_list_view_get_model(GTK_LIST_VIEW(win->contacts_list_view)));
    ContactObject *selected_contact = g_list_model_get_item(G_LIST_MODEL(selection), gtk_single_selection_get_selected(selection));

    if (selected_contact) {
        phonebook_remove_contact(win->phonebook, contact_object_get_id(selected_contact));
        g_object_unref(selected_contact); // release our reference from get_item
        refresh_contacts_list(win);
    }
}

static void on_selection_changed(GtkSingleSelection *selection, GParamSpec *pspec, ContactManagerWindow *win) {
    gboolean selected = gtk_single_selection_get_selected(selection) != GTK_INVALID_LIST_POSITION;
    gtk_widget_set_sensitive(win->delete_contact_button, selected);
}

static void setup_list_item(GtkSignalListItemFactory *factory, GtkListItem *list_item) {
    GtkWidget *label = gtk_label_new(NULL);
    gtk_list_item_set_child(list_item, label);
}

static void bind_list_item(GtkSignalListItemFactory *factory, GtkListItem *list_item) {
    GtkWidget *label = gtk_list_item_get_child(list_item);
    ContactObject *contact = gtk_list_item_get_item(list_item);
    
    if (contact) {
        gtk_label_set_text(GTK_LABEL(label), contact_object_get_name(contact));
    }
}

static void contact_manager_window_init(ContactManagerWindow *win) {
    gtk_widget_init_template(GTK_WIDGET(win));
}

static void contact_manager_window_class_init(ContactManagerWindowClass *klass) {
    GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);
    gtk_widget_class_set_template_from_resource(widget_class, "/org/example/contact-manager/contact_manager_window.ui");
    gtk_widget_class_bind_template_child(widget_class, ContactManagerWindow, contacts_list_view);
    gtk_widget_class_bind_template_child(widget_class, ContactManagerWindow, delete_contact_button);
    gtk_widget_class_bind_template_child(widget_class, ContactManagerWindow, header_bar);
    gtk_widget_class_bind_template_callback(widget_class, on_new_contact_clicked);
    gtk_widget_class_bind_template_callback(widget_class, on_delete_contact_clicked);
}

GtkWidget *contact_manager_window_new(GtkApplication *app, PhoneBook *phonebook) {
    ContactManagerWindow *win = g_object_new(CONTACT_MANAGER_WINDOW_TYPE, "application", app, NULL);
    win->phonebook = phonebook;

    char *title_str = g_strdup_printf(_("Contacts: %s"), phonebook->phonebook_id);
    GtkWidget *title_label = gtk_label_new(title_str);
    gtk_header_bar_set_title_widget(GTK_HEADER_BAR(win->header_bar), title_label);
    g_free(title_str);
    
    win->contacts_store = g_list_store_new(CONTACT_TYPE_OBJECT);
    refresh_contacts_list(win);

    GtkListItemFactory *factory = gtk_signal_list_item_factory_new();
    g_signal_connect(factory, "setup", G_CALLBACK(setup_list_item), NULL);
    g_signal_connect(factory, "bind", G_CALLBACK(bind_list_item), NULL);

    GtkSingleSelection *selection_model = gtk_single_selection_new(G_LIST_MODEL(win->contacts_store));
    gtk_list_view_set_model(GTK_LIST_VIEW(win->contacts_list_view), GTK_SELECTION_MODEL(selection_model));
    gtk_list_view_set_factory(GTK_LIST_VIEW(win->contacts_list_view), factory);

    g_signal_connect(selection_model, "notify::selected", G_CALLBACK(on_selection_changed), win);

    gtk_widget_set_sensitive(win->delete_contact_button, FALSE);

    return GTK_WIDGET(win);
} 