#include "ui/main_window.h"
#include <gtk/gtk.h>

static void on_quit(GtkWidget *widget, gpointer data) {
    GtkWindow *window = GTK_WINDOW(data);
    gtk_window_destroy(window);
}

void ui_main_window_new(GtkApplication *app) {
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Contact Manager");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_window_set_child(GTK_WINDOW(window), main_box);

    // Menu
    GtkWidget *header = gtk_header_bar_new();
    gtk_header_bar_set_title_widget(GTK_HEADER_BAR(header), gtk_label_new("Contact Manager"));
    gtk_window_set_titlebar(GTK_WINDOW(window), header);

    GtkWidget *quit_button = gtk_button_new_with_label("Quit");
    g_signal_connect(quit_button, "clicked", G_CALLBACK(on_quit), window);
    gtk_header_bar_pack_end(GTK_HEADER_BAR(header), quit_button);

    // Placeholder para conteúdo
    GtkWidget *label = gtk_label_new("Aqui vai a lista de contatos...");
    gtk_box_append(GTK_BOX(main_box), label);

    gtk_window_present(GTK_WINDOW(window));
}
