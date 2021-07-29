/**
 * This file is part of mode7-cpp.
 * Copyright (C) 2021  Patrick Roche
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
**/

#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <vector>
#include <string>
#include "gl.hpp"
#include "BBox.hpp"

namespace mode7
{

class Object
{
public:

    Object();
    virtual ~Object();

    void addChild(Object*);

    void move();
    virtual void update();
    virtual void update(bool);
    virtual void update(glm::mat4);
    virtual void update(glm::mat4, bool);



    glm::mat4 getWorldMatrix();
    glm::vec3 getWorldPosition();
    glm::vec3 getWorldRotation();
    glm::vec3 getWorldScale();
    glm::vec3 getFront();
    glm::quat getWorldQuat();

    void accumulate();
    void decompose();

    void setRotation(float, float, float);
    void rotate(float, float, float);
    void setRotationX(float);
    void setRotationY(float);
    void setRotationZ(float);

    glm::quat getWorldRx();
    glm::quat getWorldRy();

    BBox* getBoundingBox();
    std::vector<Object*> getChildren();

    Object* getParent();

    glm::vec3 position;
    glm::quat rx;
    glm::quat ry;
    glm::quat rz;
    glm::vec3 scale;
    glm::vec3 velocity;

    glm::quat quat;
    glm::mat4 matrix;
    glm::mat4 inherent;

    std::string name;

protected:

    glm::vec3 worldPosition;
    glm::vec3 worldRotation;
    glm::vec3 worldScale;
    glm::quat worldQuat;
    glm::mat4 worldMatrix;

    glm::vec3 front;
    glm::vec3 right;
    glm::vec3 up;

    Object* parent;

private:

    BBox bbox;

    std::vector<Object*> children;
};

}

#endif /* OBJECT_HPP */
