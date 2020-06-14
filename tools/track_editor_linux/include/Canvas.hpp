#ifndef CANVAS_HPP
#define CANVAS_HPP

#include <gtk/gtk.h>
#include <track_editor.hpp>

class Canvas
{
public:

    Canvas();
    ~Canvas();

    void create();
    void attach();
    void clear();
    void drawPart(Part*);

    void configureEvent();
    gboolean scrollEvent(GdkEventScroll*);
    gboolean motionEvent(GdkEventMotion*);
    gboolean buttonEvent(GdkEventButton*);
    void drawEvent(cairo_t*);

    GtkWidget* getWidget();

private:

    double offsetX;
    double offsetY;
    int lastX;
    int lastY;
    int scale;
    int w;
    int h;
    cairo_surface_t* surface;
    GtkWidget* widget;
};

#endif /* CANVAS_HPP */
