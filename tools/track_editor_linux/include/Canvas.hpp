#ifndef CANVAS_HPP
#define CANVAS_HPP

#include <gtk/gtk.h>

class Canvas
{
public:

    Canvas();
    ~Canvas();

    GtkWidget* getWidget();

private:

    GtkWidget* widget;
};

#endif /* CANVAS_HPP */