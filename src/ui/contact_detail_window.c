#include "contact_detail_window.h"
#include "phone_object.h"
#include "address_object.h"
#include "contact.h" // For contact_add/remove functions
#include <glib/gi18n.h>

struct _ContactDetailWindow {
    GtkApplicationWindow parent_instance;

    ContactObject *contact;
    GListStore *phones_store;
    GListStore *addresses_store;

    GtkWidget *header_bar;
    GtkWidget *phones_list_view;
    GtkWidget *addresses_list_view;
    GtkWidget *delete_phone_button;
    GtkWidget *delete_address_button;
    GtkWidget *new_phone_button;
    GtkWidget *new_address_button;
};

G_DEFINE_TYPE(ContactDetailWindow, contact_detail_window, GTK_TYPE_APPLICATION_WINDOW)

static void refresh_lists(ContactDetailWindow *win) {
    Contact *contact_data = contact_object_get_contact(win->contact);
    g_list_store_remove_all(win->phones_store);
    for (guint i = 0; i < contact_data->phones->len; i++) {
        g_list_store_append(win->phones_store, g_ptr_array_index(contact_data->phones, i));
    }

    g_list_store_remove_all(win->addresses_store);
    for (guint i = 0; i < contact_data->addresses->len; i++) {
        g_list_store_append(win->addresses_store, g_ptr_array_index(contact_data->addresses, i));
    }
}

// --- Phone Callbacks ---
static void on_new_phone_clicked(GtkButton *button, ContactDetailWindow *win) {
    Phone *p = phone_create();
    p->number = g_strdup_printf("555-%04d", g_random_int_range(0, 9999));
    PhoneObject *p_obj = phone_object_new(p);
    contact_add_phone(contact_object_get_contact(win->contact), p_obj);
    g_object_unref(p_obj);
    refresh_lists(win);
}

static void on_delete_phone_clicked(GtkButton *button, ContactDetailWindow *win) {
    GtkSingleSelection *selection = GTK_SINGLE_SELECTION(gtk_list_view_get_model(GTK_LIST_VIEW(win->phones_list_view)));
    PhoneObject *selected = g_list_model_get_item(G_LIST_MODEL(selection), gtk_single_selection_get_selected(selection));
    if (selected) {
        contact_remove_phone(contact_object_get_contact(win->contact), phone_object_get_id(selected));
        g_object_unref(selected);
        refresh_lists(win);
    }
}

// --- Address Callbacks ---
static void on_new_address_clicked(GtkButton *button, ContactDetailWindow *win) {
    Address *a = address_create();
    a->street = g_strdup_printf("%d Main St", g_random_int_range(100, 1000));
    AddressObject *a_obj = address_object_new(a);
    contact_add_address(contact_object_get_contact(win->contact), a_obj);
    g_object_unref(a_obj);
    refresh_lists(win);
}

static void on_delete_address_clicked(GtkButton *button, ContactDetailWindow *win) {
    GtkSingleSelection *selection = GTK_SINGLE_SELECTION(gtk_list_view_get_model(GTK_LIST_VIEW(win->addresses_list_view)));
    AddressObject *selected = g_list_model_get_item(G_LIST_MODEL(selection), gtk_single_selection_get_selected(selection));
    if (selected) {
        contact_remove_address(contact_object_get_contact(win->contact), address_object_get_id(selected));
        g_object_unref(selected);
        refresh_lists(win);
    }
}

// --- List View Setup ---
static void setup_list_item(GtkSignalListItemFactory *factory, GtkListItem *list_item) {
    GtkWidget *label = gtk_label_new(NULL);
    gtk_list_item_set_child(list_item, label);
}

static void bind_phone_item(GtkSignalListItemFactory *f, GtkListItem *li) {
    GtkWidget *label = gtk_list_item_get_child(li);
    PhoneObject *phone = gtk_list_item_get_item(li);
    gtk_label_set_text(GTK_LABEL(label), phone ? phone_object_get_number(phone) : "");
}

static void bind_address_item(GtkSignalListItemFactory *f, GtkListItem *li) {
    GtkWidget *label = gtk_list_item_get_child(li);
    AddressObject *address = gtk_list_item_get_item(li);
    gtk_label_set_text(GTK_LABEL(label), address ? address_object_get_street(address) : "");
}

// --- Window Lifecycle ---
static void contact_detail_window_init(ContactDetailWindow *win) {
    gtk_widget_init_template(GTK_WIDGET(win));
}

static void contact_detail_window_dispose(GObject *object) {
    ContactDetailWindow *win = CONTACT_DETAIL_WINDOW(object);
    g_clear_object(&win->contact);
    G_OBJECT_CLASS(contact_detail_window_parent_class)->dispose(object);
}

static void contact_detail_window_class_init(ContactDetailWindowClass *klass) {
    GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);
    GObjectClass *object_class = G_OBJECT_CLASS(klass);

    object_class->dispose = contact_detail_window_dispose;

    gtk_widget_class_set_template_from_resource(widget_class, "/org/example/contact-manager/contact_detail_window.ui");
    gtk_widget_class_bind_template_child(widget_class, ContactDetailWindow, header_bar);
    gtk_widget_class_bind_template_child(widget_class, ContactDetailWindow, phones_list_view);
    gtk_widget_class_bind_template_child(widget_class, ContactDetailWindow, addresses_list_view);
    gtk_widget_class_bind_template_child(widget_class, ContactDetailWindow, new_phone_button);
    gtk_widget_class_bind_template_child(widget_class, ContactDetailWindow, delete_phone_button);
    gtk_widget_class_bind_template_child(widget_class, ContactDetailWindow, new_address_button);
    gtk_widget_class_bind_template_child(widget_class, ContactDetailWindow, delete_address_button);
}

GtkWidget *contact_detail_window_new(GtkApplication *app, ContactObject *contact) {
    ContactDetailWindow *win = g_object_new(CONTACT_DETAIL_WINDOW_TYPE, "application", app, NULL);
    win->contact = g_object_ref(contact);

    // Setup Header
    char *title = g_strdup_printf(_("Details for %s"), contact_object_get_name(contact));
    GtkWidget *title_label = gtk_label_new(title);
    gtk_header_bar_set_title_widget(GTK_HEADER_BAR(win->header_bar), title_label);
    g_free(title);

    // Setup Stores
    win->phones_store = g_list_store_new(PHONE_TYPE_OBJECT);
    win->addresses_store = g_list_store_new(ADDRESS_TYPE_OBJECT);
    refresh_lists(win);

    // Setup Phone List
    GtkListItemFactory *phone_factory = gtk_signal_list_item_factory_new();
    g_signal_connect(phone_factory, "setup", G_CALLBACK(setup_list_item), NULL);
    g_signal_connect(phone_factory, "bind", G_CALLBACK(bind_phone_item), NULL);
    GtkSingleSelection *phone_selection = gtk_single_selection_new(G_LIST_MODEL(win->phones_store));
    gtk_list_view_set_model(GTK_LIST_VIEW(win->phones_list_view), GTK_SELECTION_MODEL(phone_selection));
    gtk_list_view_set_factory(GTK_LIST_VIEW(win->phones_list_view), phone_factory);
    
    g_signal_connect(win->new_phone_button, "clicked", G_CALLBACK(on_new_phone_clicked), win);
    g_signal_connect(win->delete_phone_button, "clicked", G_CALLBACK(on_delete_phone_clicked), win);

    // Setup Address List
    GtkListItemFactory *address_factory = gtk_signal_list_item_factory_new();
    g_signal_connect(address_factory, "setup", G_CALLBACK(setup_list_item), NULL);
    g_signal_connect(address_factory, "bind", G_CALLBACK(bind_address_item), NULL);
    GtkSingleSelection *address_selection = gtk_single_selection_new(G_LIST_MODEL(win->addresses_store));
    gtk_list_view_set_model(GTK_LIST_VIEW(win->addresses_list_view), GTK_SELECTION_MODEL(address_selection));
    gtk_list_view_set_factory(GTK_LIST_VIEW(win->addresses_list_view), address_factory);
    
    g_signal_connect(win->new_address_button, "clicked", G_CALLBACK(on_new_address_clicked), win);
    g_signal_connect(win->delete_address_button, "clicked", G_CALLBACK(on_delete_address_clicked), win);
    
    return GTK_WIDGET(win);
} 