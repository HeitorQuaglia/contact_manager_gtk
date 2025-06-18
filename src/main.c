#include <adwaita.h>

#include "main_window.h"
#include "phonebook_manager.h"

int main(int argc, char *argv[]) {
    PhoneBookManager *manager = phonebook_manager_new();
    phonebook_manager_load_all(manager);

    g_autoptr(AdwApplication) app = adw_application_new("org.example.contact-manager", G_APPLICATION_DEFAULT_FLAGS);

    g_signal_connect(app, "activate", G_CALLBACK(main_window_on_activate), manager);

    int status = g_application_run(G_APPLICATION(app), argc, argv);

    phonebook_manager_save_all(manager);
    phonebook_manager_destroy(manager);

    return status;
}
