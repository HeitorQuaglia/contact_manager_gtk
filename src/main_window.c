#include "main_window.h"
#include "ui/phonebooks_view.h"
#include "ui/contacts_view.h"

struct _MainWindow {
    AdwApplicationWindow parent_instance;

    AdwViewStack *view_stack;
    AdwHeaderBar *header_bar;
};

G_DEFINE_TYPE(MainWindow, main_window, ADW_TYPE_APPLICATION_WINDOW);

static void main_window_init(MainWindow *win) {
    gtk_widget_init_template(GTK_WIDGET(win));
}

static void main_window_class_init(MainWindowClass *klass) {
    GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);
    gtk_widget_class_set_template_from_resource(widget_class, "/org/example/contact-manager/main_window.ui");
    gtk_widget_class_bind_template_child(widget_class, MainWindow, view_stack);
    gtk_widget_class_bind_template_child(widget_class, MainWindow, header_bar);
}

static void on_phonebook_activated(PhonebooksView *view, PhoneBookObject *pb_obj, MainWindow *win) {
    char *pb_name = NULL;
    g_object_get(pb_obj, "name", &pb_name, NULL);
    g_autofree char *title = g_strconcat("Contacts: ", pb_name, NULL);
    g_free(pb_name);

    GtkWidget *contacts_page = contacts_view_new(pb_obj);
    adw_view_stack_add_titled(win->view_stack, contacts_page, title, title);
    adw_view_stack_set_visible_child_name(win->view_stack, title);
}

void main_window_on_activate(GApplication *app, gpointer user_data) {
    PhoneBookManager *manager = user_data;
    
    MainWindow *win = g_object_new(MAIN_WINDOW_TYPE, "application", app, NULL);
    
    GtkWidget *phonebooks_page = phonebooks_view_new(manager);
    adw_view_stack_add_titled(win->view_stack, phonebooks_page, "phonebooks", "Phonebooks");
    
    g_signal_connect(phonebooks_page, "phonebook-activated", G_CALLBACK(on_phonebook_activated), win);
    
    gtk_window_present(GTK_WINDOW(win));
} 