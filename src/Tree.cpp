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
    shadow.scale.y = 0.3;
    shadow.material.setDiffuseTexture(Texture::open("assets/textures/drop_shadow.png"));
    shadow.createFromShape(Mesh::PLANE);
    shadow.rotate(M_PI / 2.0f, 0, 0);
    shadow.position.y = -1.0f;
    Object::addChild(shadow);

    // set texture
    material.setDiffuseTexture(Texture::open("assets/textures/tree.png"));
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
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glEnable(GL_ALPHA_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);
    shadow.draw(*alt);
    glDisable(GL_BLEND);
    glDisable(GL_ALPHA_TEST);
    glEnable(GL_DEPTH_TEST);

    Mesh::draw(s);
}
