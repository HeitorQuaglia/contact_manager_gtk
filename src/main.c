#include "phonebook_manager.h"
#include "ui/phonebook_manager_window.h"
#include <gtk/gtk.h>

static void on_activate(GtkApplication *app, gpointer user_data) {
    PhoneBookManager *manager = user_data;
    GtkWidget *win = phonebook_manager_window_new(app, manager);
    gtk_widget_set_visible(win, TRUE);
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;
    PhoneBookManager *manager = phonebook_manager_new();

    app = gtk_application_new("com.contact.manager", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(on_activate), manager);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    phonebook_manager_destroy(manager);

    return status;
}
