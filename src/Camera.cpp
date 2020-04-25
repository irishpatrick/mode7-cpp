#include "Camera.hpp"

glm::mat4 Camera::view(1.0f);
glm::mat4 Camera::projection(1.0f);
Object Camera::object;

void Camera::create(float w, float h, float fov, float n, float f)
{
    projection = glm::perspective(glm::radians(fov), w / h, n, f);
}

void Camera::update()
{
    object.update();
    view = glm::inverse(object.getWorldMatrix());
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
