#ifndef TREE_HPP
#define TREE_HPP

#include "Mesh.hpp"

class Tree : public Mesh
{
public:

    Tree();
    ~Tree();

    void init();
    virtual void update();

    virtual void draw(Shader& s);

private:

    Mesh shadow;
};

#endif /* TREE_HPP */