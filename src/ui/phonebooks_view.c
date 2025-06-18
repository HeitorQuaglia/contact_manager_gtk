#include "ui/phonebooks_view.h"
#include "phonebook_object.h"
#include <libintl.h>

#ifndef _
#define _(String) gettext(String)
#endif

struct _PhonebooksView {
    AdwBin parent_instance;

    PhoneBookManager *manager;
    GtkWidget *list_view;
    GtkWidget *delete_button;
    GtkWidget *edit_button;
};

G_DEFINE_TYPE(PhonebooksView, phonebooks_view, ADW_TYPE_BIN);

enum {
    SIGNAL_PHONEBOOK_ACTIVATED,
    LAST_SIGNAL
};

static guint phonebooks_view_signals[LAST_SIGNAL] = { 0 };

// Forward declaration for on_edit_response
static void on_edit_response(GtkDialog *dialog, gint response_id, PhoneBookObject *pb_obj);

static void on_add_button_clicked(GtkButton *button, PhonebooksView *view) {
    phonebook_manager_add_new(view->manager);
}

static void on_delete_button_clicked(GtkButton *button, PhonebooksView *view) {
    GtkSingleSelection *selection = GTK_SINGLE_SELECTION(gtk_list_view_get_model(GTK_LIST_VIEW(view->list_view)));
    guint selected_pos = gtk_single_selection_get_selected(selection);

    if (selected_pos != GTK_INVALID_LIST_POSITION) {
        PhoneBookObject *selected_pb = g_list_model_get_item(gtk_single_selection_get_model(selection), selected_pos);
        if (selected_pb) {
            phonebook_manager_remove(view->manager, selected_pb);
            g_object_unref(selected_pb);
        }
    }
}

static void on_edit_button_clicked(GtkButton *button, PhonebooksView *view) {
    GtkSingleSelection *selection = GTK_SINGLE_SELECTION(gtk_list_view_get_model(GTK_LIST_VIEW(view->list_view)));
    guint selected_pos = gtk_single_selection_get_selected(selection);

    if (selected_pos != GTK_INVALID_LIST_POSITION) {
        PhoneBookObject *selected_pb = g_list_model_get_item(gtk_single_selection_get_model(selection), selected_pos);
        if (selected_pb) {
            GtkWidget *dialog = gtk_dialog_new_with_buttons("Edit Phonebook Name",
                                                             GTK_WINDOW(gtk_widget_get_root(GTK_WIDGET(view))),
                                                             GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                                             _("_Cancel"), GTK_RESPONSE_CANCEL,
                                                             _("_Save"), GTK_RESPONSE_ACCEPT,
                                                             NULL);
            GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
            GtkWidget *entry = gtk_entry_new();
            char *current_name = NULL;
            g_object_get(selected_pb, "name", &current_name, NULL);
            gtk_editable_set_text(GTK_EDITABLE(entry), current_name);
            g_free(current_name);
            gtk_widget_set_margin_start(GTK_WIDGET(entry), 12);
            gtk_widget_set_margin_end(GTK_WIDGET(entry), 12);
            gtk_widget_set_margin_top(GTK_WIDGET(entry), 12);
            gtk_widget_set_margin_bottom(GTK_WIDGET(entry), 12);
            gtk_box_append(GTK_BOX(content_area), entry);

            g_object_ref(selected_pb);
            g_signal_connect(dialog, "response", G_CALLBACK(on_edit_response), selected_pb);
            gtk_widget_show(dialog);
            g_object_unref(selected_pb);
        }
    }
}

static void on_edit_response(GtkDialog *dialog, gint response_id, PhoneBookObject *pb_obj) {
    if (response_id == GTK_RESPONSE_ACCEPT) {
        GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
        GtkWidget *entry = gtk_widget_get_first_child(content_area);
        const char *new_name = gtk_editable_get_text(GTK_EDITABLE(entry));
        if (new_name && *new_name) {
            g_object_set(pb_obj, "name", new_name, NULL);
        }
    }
    g_object_unref(pb_obj);
    gtk_window_destroy(GTK_WINDOW(dialog));
}

static void on_selection_changed(GtkSingleSelection *selection, GParamSpec *pspec, PhonebooksView *view) {
    gboolean selected = gtk_single_selection_get_selected(selection) != GTK_INVALID_LIST_POSITION;
    gtk_widget_set_sensitive(view->delete_button, selected);
    gtk_widget_set_sensitive(view->edit_button, selected);
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

static void on_phonebook_list_activated(GtkListView *list_view, guint position, gpointer user_data) {
    PhonebooksView *view = user_data;
    PhoneBookObject *pb_obj = g_list_model_get_item(gtk_single_selection_get_model(GTK_SINGLE_SELECTION(gtk_list_view_get_model(list_view))), position);
    if (pb_obj) {
        g_signal_emit(view, phonebooks_view_signals[SIGNAL_PHONEBOOK_ACTIVATED], 0, pb_obj);
        g_object_unref(pb_obj);
    }
}

static void phonebooks_view_init(PhonebooksView *view) {
    gtk_widget_init_template(GTK_WIDGET(view));
    gtk_widget_set_sensitive(view->delete_button, FALSE);
    gtk_widget_set_sensitive(view->edit_button, FALSE);
}

static void phonebooks_view_class_init(PhonebooksViewClass *klass) {
    GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);
    gtk_widget_class_set_template_from_resource(widget_class, "/org/example/contact-manager/phonebooks_view.ui");

    phonebooks_view_signals[SIGNAL_PHONEBOOK_ACTIVATED] = g_signal_new(
        "phonebook-activated",
        G_TYPE_FROM_CLASS(klass),
        G_SIGNAL_RUN_LAST,
        0,
        NULL, NULL,
        g_cclosure_marshal_generic,
        G_TYPE_NONE,
        1,
        PHONEBOOK_TYPE_OBJECT
    );

    gtk_widget_class_bind_template_child(widget_class, PhonebooksView, list_view);
    gtk_widget_class_bind_template_child(widget_class, PhonebooksView, delete_button);
    gtk_widget_class_bind_template_child(widget_class, PhonebooksView, edit_button);

    gtk_widget_class_bind_template_callback(widget_class, on_add_button_clicked);
    gtk_widget_class_bind_template_callback(widget_class, on_delete_button_clicked);
    gtk_widget_class_bind_template_callback(widget_class, on_edit_button_clicked);
}

GtkWidget *phonebooks_view_new(PhoneBookManager *manager) {
    PhonebooksView *view = g_object_new(phonebooks_view_get_type(), NULL);
    view->manager = manager;

    GtkListItemFactory *factory = gtk_signal_list_item_factory_new();
    g_signal_connect(factory, "setup", G_CALLBACK(setup_list_item), NULL);
    g_signal_connect(factory, "bind", G_CALLBACK(bind_list_item), NULL);

    GtkSingleSelection *selection_model = gtk_single_selection_new(G_LIST_MODEL(view->manager->phonebooks));
    gtk_list_view_set_model(GTK_LIST_VIEW(view->list_view), GTK_SELECTION_MODEL(selection_model));
    gtk_list_view_set_factory(GTK_LIST_VIEW(view->list_view), factory);

    g_signal_connect(selection_model, "notify::selected", G_CALLBACK(on_selection_changed), view);
    g_signal_connect(view->list_view, "activate", G_CALLBACK(on_phonebook_list_activated), view);
    
    return GTK_WIDGET(view);
} 