#include "DropShadow.hpp"
#include "Texture.hpp"
#include "TexCache.hpp"
#include "gl.hpp"
#include "Camera.hpp"

namespace mode7
{

DropShadow::DropShadow() :
    Mesh(),
    tracked(false)
{

}

DropShadow::~DropShadow()
{

}

void DropShadow::create()
{
    material.addMap(TexCache::open("assets/textures/drop_shadow.png", TexType::DIFFUSE));

    Mesh::createFromShape(Mesh::PLANE);

    scale.x = 1.2f;
    scale.y = 0.35f;

    rotate(M_PI / 2.0f, 0, 0);
}

void DropShadow::apply(Mesh& parent)
{
    parent.addChild(this);
    position.y = -1.0f; // temporary
}

void DropShadow::setTracked(bool val)
{
    tracked = val;
}

void DropShadow::update()
{
    if (!tracked)
    {
        ry = Camera::getObject().getWorldRy();
    }

    Mesh::update();
}

void DropShadow::draw(Shader& s)
{
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glEnable(GL_ALPHA_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);

    Mesh::draw(s);
    //s.use();
    //s.setMaterial(material);
    //s.setModel(*this);
    //drawTriangles();

    glDisable(GL_BLEND);
    glDisable(GL_ALPHA_TEST);
    glEnable(GL_DEPTH_TEST);
}

}
