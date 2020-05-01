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
    float theta = -atanf(Camera::getObject().position.y / (Camera::getObject().position.z + 7.0f));
    rotate(theta, 0, 0);
    material.setDiffuseTexture(Texture::open("assets/textures/tree.png"));
    Mesh::createFromShape(Mesh::PLANE);
    scale *= 2;
    
}

void Tree::update()
{
    // set rotation to camera
    //Object::rotation = Camera::getObject().getWorldRotation();
    //Object::quat = Camera::getObject().getWorldQuat();
    
    ry = Camera::getObject().getWorldRy();

    Object::update();
}
