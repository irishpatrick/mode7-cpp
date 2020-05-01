#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <vector>
#include "gl.h"
#include "BBox.hpp"

class Object
{
public:

    Object();
    virtual ~Object();

    void addChild(Object&);
    void addChild(Object*);
    virtual void update();

    glm::mat4 getWorldMatrix();

    glm::vec3 getWorldPosition();
    glm::vec3 getWorldRotation();
    glm::vec3 getWorldScale();
    glm::vec3 getFront();
    glm::quat getWorldQuat();

    BBox* getBoundingBox();

    void accumulate();
    void decompose();

    void setRotation(float, float, float);
    void rotate(float, float, float);
    void setRotationX(float);
    void setRotationY(float);
    void setRotationZ(float);

    glm::quat getWorldRx();
    glm::quat getWorldRy();

    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    glm::quat quat;
    glm::quat rx;
    glm::quat ry;
    glm::quat rz;

    glm::mat4 matrix;

protected:

    glm::vec3 worldPosition;
    glm::vec3 worldRotation;
    glm::vec3 worldScale;
    glm::quat worldQuat;

    glm::vec3 front;
    glm::vec3 right;

    glm::mat4 worldMatrix;

private:

    BBox bbox;

    Object* parent;
    std::vector<Object*> children;
};

#endif /* OBJECT_HPP */
