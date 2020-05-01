#include "Canvas.hpp"
#include <iostream>
#include <cmath>

static gboolean draw_cb(GtkWidget* widget, cairo_t* cr, gpointer data)
{
    Canvas* c = (Canvas*)data;
    c->drawEvent(cr);
    return FALSE;
}

static gboolean configure_event_cb(GtkWidget* widget, GdkEventConfigure* event, gpointer data)
{
    Canvas* c = (Canvas*)data;
    c->configureEvent();
    return TRUE;
}

static gboolean button_press_event_cb(GtkWidget* widget, GdkEventButton* event, gpointer data)
{
    Canvas* c = (Canvas*)data;
    return c->buttonEvent(event);
}

static gboolean motion_notify_event_cb(GtkWidget* widget, GdkEventMotion* event, gpointer data)
{
    Canvas* c = (Canvas*)data;
    return c->motionEvent(event);
}

static gboolean scroll_cb(GtkWidget* widget, GdkEventScroll* event, gpointer data)
{
    Canvas* c = (Canvas*)data;
    return c->scrollEvent(event);
}

Canvas::Canvas() :
    offsetX(0),
    offsetY(0),
    scale(20),
    w(0),
    h(0),
    surface(nullptr),
    widget(nullptr)
{

}

Canvas::~Canvas()
{

}

void Canvas::create()
{
    widget = gtk_drawing_area_new();
    gtk_widget_set_size_request(widget, 500, 500);
}

void Canvas::attach()
{
    g_signal_connect(widget, "draw", G_CALLBACK(draw_cb), this);
    g_signal_connect(widget, "configure-event", G_CALLBACK(configure_event_cb), this);
    g_signal_connect(widget, "motion-notify-event", G_CALLBACK(motion_notify_event_cb), this);
    g_signal_connect(widget, "button-press-event", G_CALLBACK(button_press_event_cb), this);
    g_signal_connect(widget, "scroll-event", G_CALLBACK(scroll_cb), this);

    gtk_widget_set_events(widget, gtk_widget_get_events(widget) | GDK_BUTTON_PRESS_MASK | GDK_POINTER_MOTION_MASK | GDK_SCROLL_MASK);
}

void Canvas::clear()
{
    cairo_t* cr;
    cr = cairo_create(surface);

    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_paint(cr);

    cairo_set_source_rgb(cr, 0.7, 0.7, 0.7);
    cairo_set_line_width(cr, 0.25);

    for (int i = 0; i < w / scale + 1; ++i)
    {
        cairo_move_to(cr, (int)(offsetX * scale) % scale + i * scale, 0);
        cairo_line_to(cr, (int)(offsetX * scale) % scale + i * scale, h);
    }
    for (int i = 0; i < h / scale + 1; ++i)
    {
        cairo_move_to(cr, 0, (int)(offsetY * scale) % scale + i * scale);
        cairo_line_to(cr, w, (int)(offsetY * scale) % scale + i * scale);
    }

    cairo_stroke(cr);

    cairo_set_source_rgb(cr, 1.0, 0.0, 0.0);
    cairo_set_line_width(cr, 4.0);
    cairo_rectangle(cr, offsetX * scale, offsetY * scale, 100 * scale, 100 * scale);
    cairo_stroke(cr);

    cairo_destroy(cr);
}

void Canvas::configureEvent()
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

    clear();
}

gboolean Canvas::motionEvent(GdkEventMotion* event)
{
    if (surface == nullptr)
    {
        return FALSE;
    }
    if (event->state & GDK_BUTTON1_MASK)
    {

    }
    else if (event->state & GDK_BUTTON2_MASK)
    {
        offsetX += (event->x - lastX) / (double)scale;
        offsetY += (event->y - lastY) / (double)scale;
        lastX = event->x;
        lastY = event->y;
    }
    else if (event->state & GDK_BUTTON3_MASK)
    {

    }

    gtk_widget_queue_draw(widget);

    return TRUE;
}

gboolean Canvas::scrollEvent(GdkEventScroll* event)
{
    if (event->direction == GDK_SCROLL_UP)
    {
        scale += 1;
    }
    else if (event->direction == GDK_SCROLL_DOWN)
    {
        scale -= 1;
    }

    if (scale < 4)
    {
        scale = 4;
    }

    int dx = 0;
    int dy = 0;
    //offsetX = event->x / scale - dx;
    //offsetY = event->y / scale - dy;

    gtk_widget_queue_draw(widget);

    return TRUE;
}

gboolean Canvas::buttonEvent(GdkEventButton* event)
{
    if (!surface)
    {
        return FALSE;
    }

    if (event->button == GDK_BUTTON_PRIMARY)
    {
        
    }
    else if (event->button == GDK_BUTTON_SECONDARY)
    {

    }
    else if (event->button == GDK_BUTTON_MIDDLE)
    {
        lastX = event->x;
        lastY = event->y;
    }

    gtk_widget_queue_draw(widget);

    return TRUE;
}

void Canvas::drawEvent(cairo_t* cr)
{
    clear();
    cairo_set_source_surface(cr, surface, 0, 0);
    cairo_paint(cr);
}

GtkWidget* Canvas::getWidget()
{
    return widget;
}