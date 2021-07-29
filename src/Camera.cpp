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

#include "Camera.hpp"

namespace mode7
{

glm::mat4 Camera::view(1.0f);
glm::mat4 Camera::projection(1.0f);
Object Camera::object;

static float m_w, m_h, m_n, m_f;
static float m_dof;

void Camera::create(float w, float h, float fov, float n, float f)
{
    projection = glm::perspective(glm::radians(fov), w / h, n, f);
    m_w = w;
    m_h = h;
    m_n = n;
    m_f = f;
}

void Camera::updateView()
{
    view = glm::inverse(object.getWorldMatrix());
}

void Camera::lookAt(glm::vec3 eye, glm::vec3 target)
{
    object.matrix = glm::inverse(glm::lookAt(eye, target, glm::vec3(0, 1, 0)));
    object.accumulate();
    object.decompose();
}

glm::mat4 Camera::getProjection()
{
    return projection;
}

glm::mat4 Camera::getView()
{
    return view;
}

Object& Camera::getObject()
{
    return object;
}

void Camera::setFOV(float fov)
{
    projection = glm::perspective(glm::radians(fov), m_w / m_h, m_n, m_f);
}

void Camera::setDOF(float threshold)
{
    m_dof = threshold;
}

float Camera::getDOF()
{
    return m_dof;
}

}
