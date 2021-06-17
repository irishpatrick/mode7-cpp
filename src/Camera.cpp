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
