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

void Object::rotate(float x, float y, float z)
{
    rx = rx * glm::angleAxis(x, Util::xAxis());
    ry = ry * glm::angleAxis(y, Util::yAxis());
    rz = rz * glm::angleAxis(z, Util::zAxis());
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
    // update
    //quat = glm::quat(rotation);
    quat = ry * rz * rx;

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
    right = -glm::cross(front, glm::vec3(0, 1, 0));

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
    float a = glm::angle(worldQuat);
    if (a > M_PI / 2.0f)
    {
        worldRotation = -glm::vec3(M_PI) + glm::eulerAngles(worldQuat);
    }
    else if (a <= M_PI / 2.0f)
    {
        worldRotation = glm::eulerAngles(worldQuat);   
    }
    
}

BBox* Object::getBoundingBox()
{
    return &bbox;
}
