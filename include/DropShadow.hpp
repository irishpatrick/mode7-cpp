#ifndef DROPSHADOW_HPP
#define DROPSHADOW_HPP

#include "Mesh.hpp"

class DropShadow : public Mesh
{
public:

    DropShadow();
    virtual ~DropShadow();

    void create();
    void apply(Mesh&);

    virtual void draw(Shader&);
};

#endif /* DROPSHADOW_HPP */