#include "ui/main_window.h"
#include <gtk/gtk.h>

static void on_activate(GtkApplication *app, gpointer user_data) {
    ui_main_window_new(app);
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("com.contact.manager", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
