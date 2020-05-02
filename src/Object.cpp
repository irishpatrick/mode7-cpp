#include "Object.hpp"
#include "Util.hpp"

Object::Object() :
    parent(nullptr),
    position(0.0f),
    rotation(0.0f),
    scale(1.0f),
    quat(),
    rx(0, 1, 0, 0),
    ry(0, 0, 1, 0),
    rz(0, 0, 0, 1),
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

std::vector<Object*> Object::getChildren()
{
    return children;
}

void Object::rotate(float x, float y, float z)
{
    rx = rx * glm::angleAxis(x, Util::xAxis());
    ry = ry * glm::angleAxis(y, Util::yAxis());
    rz = rz * glm::angleAxis(z, Util::zAxis());
}

void Object::setRotationX(float x)
{
    rx = glm::angleAxis(x, Util::xAxis());

}

void Object::setRotationY(float y)
{
    ry = glm::angleAxis(y, Util::yAxis());
}

void Object::setRotationZ(float z)
{
    rz = glm::angleAxis(z, Util::zAxis());
}

void Object::setRotation(float x, float y, float z)
{
    rx = glm::angleAxis(x, Util::xAxis());
    ry = glm::angleAxis(y, Util::yAxis());
    rz = glm::angleAxis(z, Util::zAxis());
}

glm::quat Object::getWorldRx()
{
    if (parent != nullptr)
    {
        return rx * parent->getWorldRx();
    }

    return rx;
}

glm::quat Object::getWorldRy()
{
    if (parent != nullptr)
    {
        return ry * parent->ry;
    }

    return ry;
}

void Object::addChild(Object& o)
{
    o.parent = this;
    o.rx = o.rx * glm::inverse(rx);
    o.ry = o.ry * glm::inverse(ry);
    o.rz = o.rz * glm::inverse(rz);
    children.push_back(&o);
}

void Object::addChild(Object* o)
{
    o->parent = this;
    children.push_back(o);
}

glm::vec3 Object::getFront()
{
    return front;
}

glm::quat Object::getWorldQuat()
{
    return quat;
}

void Object::update()
{
    quat = ry * rz * rx;

    glm::mat4 t = glm::translate(glm::mat4(1.0f), position);
    glm::mat4 r = glm::toMat4(quat);
    glm::mat4 s = glm::scale(glm::mat4(1.0f), scale);

    matrix = t * r * s;

    accumulate();
    decompose();

    // etc
    front = worldQuat * Util::zAxis();
    right = -glm::cross(front, Util::yAxis());

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
}

BBox* Object::getBoundingBox()
{
    return &bbox;
}
