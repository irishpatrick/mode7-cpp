#include <iostream>
#include <cstdlib>
#include <gtk/gtk.h>
#include <cairo/cairo.h>

static cairo_surface_t* surface = nullptr;

static int w;
static int h;
static int scale = 20;

static void clear_surface()
{
    cairo_t* cr;
    cr = cairo_create(surface);

    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_paint(cr);

    cairo_set_source_rgb(cr, 0.7, 0.7, 0.7);
    cairo_set_line_width(cr, 0.25);

    for (int i = 0; i < w / scale; ++i)
    {
        cairo_move_to(cr, i * scale, 0);
        cairo_line_to(cr, i * scale, h);
    }
    for (int i = 0; i < h / scale; ++i)
    {
        cairo_move_to(cr, 0, i * scale);
        cairo_line_to(cr, w, i * scale);
    }

    cairo_stroke(cr);

    cairo_destroy(cr);
}

static gboolean configure_event_cb(GtkWidget *widget, GdkEventConfigure *event, gpointer data)
{

    w = gtk_widget_get_allocated_width(widget);
    h = gtk_widget_get_allocated_height(widget);
    if (surface)
    {
        cairo_surface_destroy(surface);
    }
    surface = gdk_window_create_similar_surface(
        gtk_widget_get_window(widget), 
        CAIRO_CONTENT_COLOR, 
        gtk_widget_get_allocated_width(widget), 
        gtk_widget_get_allocated_height(widget));

    clear_surface();

    return TRUE;
}

static gboolean draw_cb(GtkWidget* widget, cairo_t* cr, gpointer data)
{
    cairo_set_source_surface(cr, surface, 0, 0);
    cairo_paint(cr);
    
    return FALSE;
}

static void draw_brush(GtkWidget* widget, gdouble x, gdouble y)
{
    cairo_t *cr;

    cr = cairo_create(surface);

    cairo_rectangle(cr, x - 3, y - 3, 6, 6);
    cairo_fill(cr);

    cairo_destroy(cr);

    gtk_widget_queue_draw_area(widget, x - 3, y - 3, 6, 6);
}

static gboolean button_press_event_cb(GtkWidget* widget, GdkEventButton* event, gpointer data)
{
    if (surface == NULL)
    {
        return FALSE;
    }

    if (event->button == GDK_BUTTON_PRIMARY)
    {
        draw_brush (widget, event->x, event->y);
    }
    else if (event->button == GDK_BUTTON_SECONDARY)
    {
        clear_surface();
        gtk_widget_queue_draw(widget);
    }

    return TRUE;
}

static gboolean motion_notify_event_cb (GtkWidget* widget, GdkEventMotion *event, gpointer data)
{
    if (surface == NULL)
    {
        return FALSE;
    }
    if (event->state & GDK_BUTTON1_MASK)
    {
        draw_brush (widget, event->x, event->y);
    }

    return TRUE;
}

static void close_window(void)
{
    if (surface)
    {
        cairo_surface_destroy(surface);
    }
}

static void activate(GtkApplication* app, gpointer user_data)
{
    GtkWidget* window;
    GtkWidget* frame;
    GtkWidget* drawing_area;
    GtkWidget* editing_area;
    GtkWidget* library_list;
    GtkWidget* hbox;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Title");
    g_signal_connect(window, "destroy", G_CALLBACK(close_window), nullptr);
    gtk_container_set_border_width(GTK_CONTAINER(window), 8);

    hbox = gtk_hbox_new(TRUE, 1);
    gtk_container_add(GTK_CONTAINER(window), hbox);
    gtk_box_set_homogeneous(GTK_BOX(hbox), FALSE);
    gtk_box_set_spacing(GTK_BOX(hbox), 2);

    library_list = gtk_tree_view_new();
    gtk_widget_set_size_request(library_list, 100, 100);

    drawing_area = gtk_drawing_area_new();
    gtk_widget_set_size_request(drawing_area, 100, 100);

    editing_area = gtk_drawing_area_new();
    gtk_widget_set_size_request(editing_area, 100, 100);

    gtk_box_pack_start(GTK_BOX(hbox), library_list, FALSE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbox), editing_area, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbox), drawing_area, TRUE, TRUE, 0);

    g_signal_connect (drawing_area, "draw", G_CALLBACK(draw_cb), nullptr);
    g_signal_connect (drawing_area,"configure-event", G_CALLBACK(configure_event_cb), nullptr);
    g_signal_connect (drawing_area, "motion-notify-event", G_CALLBACK(motion_notify_event_cb), nullptr);
    g_signal_connect (drawing_area, "button-press-event", G_CALLBACK(button_press_event_cb), nullptr);

    g_signal_connect (editing_area, "draw", G_CALLBACK(draw_cb), nullptr);
    g_signal_connect (editing_area,"configure-event", G_CALLBACK(configure_event_cb), nullptr);
    g_signal_connect (editing_area, "motion-notify-event", G_CALLBACK(motion_notify_event_cb), nullptr);
    g_signal_connect (editing_area, "button-press-event", G_CALLBACK(button_press_event_cb), nullptr);

    gtk_widget_set_events(editing_area, gtk_widget_get_events (drawing_area) | GDK_BUTTON_PRESS_MASK | GDK_POINTER_MOTION_MASK);
    gtk_widget_set_events(drawing_area, gtk_widget_get_events (drawing_area) | GDK_BUTTON_PRESS_MASK | GDK_POINTER_MOTION_MASK);

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