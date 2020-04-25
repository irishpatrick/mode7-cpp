#include "Object.hpp"
#include "Util.hpp"

Object::Object() :
    parent(nullptr),
    position(0.0f),
    rotation(0.0f),
    scale(1.0f),
    quat(),
    worldPosition(0.0f),
    worldRotation(0.0f),
    worldScale(1.0f),
    worldQuat(),
    matrix(1.0f),
    worldMatrix(1.0f)
{

}

Object::~Object()
{
}

void Object::addChild(Object& o)
{
    o.parent = this;
    children.push_back(&o);
}

glm::vec3 Object::getFront()
{
    return front;
}

void Object::update()
{
    // update
    quat = glm::quat(rotation);

    glm::mat4 t = glm::translate(glm::mat4(1.0f), position);
    glm::mat4 r = glm::toMat4(quat);
    glm::mat4 s = glm::scale(glm::mat4(1.0f), scale);

    matrix = t * r * s;

    accumulate();
    decompose();

    // etc
    //glm::mat4 inv = glm::inverse(worldMatrix);
    //front = glm::normalize(glm::vec3(inv[2]));
    front = worldQuat * Util::zAxis();

    // update children
    for (auto& e : children)
    {
        e->update();
    }
}

glm::mat4 Object::getWorldMatrix()
{
    return worldMatrix;
}

glm::vec3 Object::getWorldPosition()
{
    return worldPosition;
}

glm::vec3 Object::getWorldRotation()
{
    return worldRotation;
}

glm::vec3 Object::getWorldScale()
{
    return worldScale;
}

void Object::accumulate()
{
    if (parent != nullptr)
    {
        worldMatrix = parent->worldMatrix * matrix;
    }
    else
    {
        worldMatrix = matrix;
    }
}

void Object::decompose()
{
    glm::vec3 skew;
    glm::vec4 persp;
    glm::decompose(worldMatrix, worldScale, worldQuat, worldPosition, skew, persp);
    //worldQuat = glm::conjugate(worldQuat);
    worldRotation = glm::eulerAngles(worldQuat);
}

BBox* Object::getBoundingBox()
{
    return &bbox;
}
