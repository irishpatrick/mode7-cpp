#ifndef TREE_HPP
#define TREE_HPP

#include "Mesh.hpp"
#include "DropShadow.hpp"
#include "Shader.hpp"

namespace mode7
{

class Tree : public Mesh
{
public:

    Tree();
    ~Tree();

    void init();
    virtual void update();

    virtual void draw(Shader& s);

private:

    DropShadow shadow;
};

}

#endif /* TREE_HPP */