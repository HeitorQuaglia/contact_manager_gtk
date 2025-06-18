#ifndef PHONEBOOKS_VIEW_H
#define PHONEBOOKS_VIEW_H

#include <adwaita.h>
#include "phonebook_manager.h"

G_DECLARE_FINAL_TYPE(PhonebooksView, phonebooks_view, PHONEBOOKS, VIEW, AdwBin);

GtkWidget *phonebooks_view_new(PhoneBookManager *manager);

#endif //PHONEBOOKS_VIEW_H 