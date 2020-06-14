#include "Library.hpp"

static gboolean button_press_event_cb(GtkWidget* widget, GdkEventButton* event, gpointer data)
{
    Library* c = (Library*)data;
    return c->buttonEvent(event);
}

Library::Library() :
    m_widget(nullptr)
{

}

Library::~Library()
{

}

void Library::create()
{
    m_widget = gtk_tree_view_new();
}

void Library::attach()
{
    g_signal_connect(m_widget, "button-press-event", G_CALLBACK(button_press_event_cb), this);
}

gboolean Library::buttonEvent(GdkEventButton* event)
{
    return TRUE;
}
