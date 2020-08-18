#ifndef DROPSHADOW_HPP
#define DROPSHADOW_HPP

#include "Mesh.hpp"

namespace mode7
{

class DropShadow : public Mesh
{
public:

    DropShadow();
    virtual ~DropShadow();

    void create();
    void apply(Mesh&);
    void setTracked(bool);

    virtual void update();
    virtual void draw(Shader&);

private:

    bool tracked;
};

}

#endif /* DROPSHADOW_HPP */