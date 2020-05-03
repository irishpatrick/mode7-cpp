#include "Tree.hpp"
#include "Texture.hpp"
#include "Camera.hpp"

Tree::Tree() :
    Mesh()
{

}

Tree::~Tree()
{

}

void Tree::init()
{
    shadow.create();
    shadow.apply(*this);

    // set texture
    Texture t;
    t.open("assets/textures/tree.png", TexType::DIFFUSE);
    material.addMap(t);
    Mesh::createFromShape(Mesh::PLANE);
    scale *= 2;
    
}

void Tree::update()
{
    // set rotation to camera

    rx = Camera::getObject().getWorldRx();
    ry = Camera::getObject().getWorldRy();

    Object::update();
}

void Tree::draw(Shader& s)
{
    shadow.draw(*alt);

    Mesh::draw(s);
}
