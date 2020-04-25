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
    // set texture
    material.setDiffuseTexture(Texture::open("assets/textures/tree.png"));
    Mesh::createFromShape(Mesh::PLANE);
    scale *= 2;
    
}

void Tree::update()
{
    // set rotation to camera
    Object::rotation = Camera::getObject().getWorldRotation();

    Object::update();
}
