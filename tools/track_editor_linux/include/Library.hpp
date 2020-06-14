#ifndef LIBRARY_HPP
#define LIBRARY_HPP

#include <gtk/gtk.h>
#include <track_editor.hpp>
#include <vector>
#include <memory>
#include <string>

class Library
{
public:

    Library();
    ~Library();

    void create();
    void attach();
    gboolean buttonEvent(GdkEventButton*);

    void openDir(const std::string&);

    GtkWidget* getWidget();

private:
    GtkWidget* m_widget;
    std::vector<std::unique_ptr<Part>> m_parts;
};

#endif /* LIBRARY_HPP */
