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

private:


};

#endif /* TREE_HPP */