#include "phonebook_manager_window.h"

struct _PhonebookManagerWindow {
    GtkApplicationWindow parent_instance;

    PhoneBookManager *manager;
    GtkStringList *string_list;
    GtkWidget *list_view;
    GtkWidget *delete_button;
};

G_DEFINE_TYPE(PhonebookManagerWindow, phonebook_manager_window, GTK_TYPE_APPLICATION_WINDOW)

static void refresh_phonebook_list(PhonebookManagerWindow *win) {
    guint n_items = g_list_model_get_n_items(G_LIST_MODEL(win->string_list));
    gtk_string_list_splice(win->string_list, 0, n_items, NULL);

    for (int i = 0; i < win->manager->count; i++) {
        gtk_string_list_append(win->string_list, win->manager->phonebooks[i]->phonebook_id);
    }
}

static void on_create_button_clicked(GtkButton *button, PhonebookManagerWindow *win) {
    PhoneBook *pb = phonebook_create();
    if (pb) {
        phonebook_manager_add(win->manager, pb);
        refresh_phonebook_list(win);
    } else {
        // Handle error, maybe show a dialog
    }
}

static void on_delete_button_clicked(GtkButton *button, PhonebookManagerWindow *win) {
    GtkSingleSelection *selection = GTK_SINGLE_SELECTION(gtk_list_view_get_model(GTK_LIST_VIEW(win->list_view)));
    guint position = gtk_single_selection_get_selected(selection);

    if (position != GTK_INVALID_LIST_POSITION) {
        const char *id_to_delete = gtk_string_list_get_string(win->string_list, position);
        phonebook_manager_remove(win->manager, id_to_delete);
        refresh_phonebook_list(win);
    }
}

static void on_selection_changed(GtkSingleSelection *selection, GParamSpec *pspec, PhonebookManagerWindow *win) {
    gboolean selected = gtk_single_selection_get_selected(selection) != GTK_INVALID_LIST_POSITION;
    gtk_widget_set_sensitive(win->delete_button, selected);
}

static void phonebook_manager_window_init(PhonebookManagerWindow *win) {
    gtk_widget_init_template(GTK_WIDGET(win));
}

static void setup_list_item(GtkSignalListItemFactory *factory, GtkListItem *list_item) {
    GtkWidget *label = gtk_label_new(NULL);
    gtk_list_item_set_child(list_item, label);
}

static void bind_list_item(GtkSignalListItemFactory *factory, GtkListItem *list_item) {
    GtkWidget *label = gtk_list_item_get_child(list_item);
    GtkStringObject *string_object = gtk_list_item_get_item(list_item);
    gtk_label_set_text(GTK_LABEL(label), gtk_string_object_get_string(string_object));
}

static void phonebook_manager_window_class_init(PhonebookManagerWindowClass *klass) {
    GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);

    gtk_widget_class_set_template_from_resource(widget_class, "/org/example/contact-manager/phonebook_manager_window.ui");
    gtk_widget_class_bind_template_callback(widget_class, on_create_button_clicked);
    gtk_widget_class_bind_template_callback(widget_class, on_delete_button_clicked);
    gtk_widget_class_bind_template_child(widget_class, PhonebookManagerWindow, delete_button);
}

GtkWidget *phonebook_manager_window_new(GtkApplication *app, PhoneBookManager *manager) {
    PhonebookManagerWindow *win = g_object_new(PHONEBOOK_MANAGER_WINDOW_TYPE, "application", app, NULL);

    win->manager = manager;

    // Setup String List and ListView
    win->string_list = gtk_string_list_new(NULL);

    GtkListItemFactory *factory = gtk_signal_list_item_factory_new();
    g_signal_connect(factory, "setup", G_CALLBACK(setup_list_item), NULL);
    g_signal_connect(factory, "bind", G_CALLBACK(bind_list_item), NULL);

    GtkSingleSelection *selection_model = gtk_single_selection_new(G_LIST_MODEL(win->string_list));
    win->list_view = gtk_list_view_new(GTK_SELECTION_MODEL(selection_model), factory);

    g_signal_connect(selection_model, "notify::selected", G_CALLBACK(on_selection_changed), win);

    // Add list view to the scrolled window from the UI file
    GtkWidget *scrolled_window = gtk_widget_get_first_child(gtk_widget_get_last_child(GTK_WIDGET(win))); // This is fragile. Assume Box -> ScrolledWindow
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_window), win->list_view);

    refresh_phonebook_list(win);
    gtk_widget_set_sensitive(win->delete_button, FALSE); // Initially insensitive

    return GTK_WIDGET(win);
} 