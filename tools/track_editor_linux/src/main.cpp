#include <iostream>
#include <cstdlib>
#include <gtk/gtk.h>
#include <cairo/cairo.h>
#include "Canvas.hpp"

static cairo_surface_t* surface = nullptr;
static Canvas drawing_area;
static Canvas editing_area;

static void close_window(void)
{

}

static void activate(GtkApplication* app, gpointer user_data)
{
    GtkWidget* window;
    GtkWidget* library_list;
    GtkWidget* hbox;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Track Editor");
    g_signal_connect(window, "destroy", G_CALLBACK(close_window), nullptr);
    gtk_container_set_border_width(GTK_CONTAINER(window), 8);

    //hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);
    hbox = gtk_hbox_new(TRUE, 1);
    gtk_container_add(GTK_CONTAINER(window), hbox);
    gtk_box_set_homogeneous(GTK_BOX(hbox), FALSE);
    gtk_box_set_spacing(GTK_BOX(hbox), 2);

    library_list = gtk_tree_view_new();
    gtk_widget_set_size_request(library_list, 100, 100);

    drawing_area.create();
    editing_area.create();

    gtk_box_pack_start(GTK_BOX(hbox), library_list, FALSE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbox), editing_area.getWidget(), TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbox), drawing_area.getWidget(), TRUE, TRUE, 0);

    drawing_area.attach();
    editing_area.attach();

    gtk_widget_show_all(window);
}

int main(int argc, char** argv)
{
    GtkApplication* app;
    int status;

    app = gtk_application_new(nullptr, G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), nullptr);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}