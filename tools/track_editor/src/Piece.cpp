#include "Piece.hpp"

Piece::Piece() :
    rotation(0.0f),
    rotation_locked(false)
{

}

Piece::~Piece()
{
    
}

void Piece::addJoint(uint32_t a, uint32_t b)
{
    Joint j;
    glm::vec2 m = shape[b] - shape[a];
    j.normal = glm::normalize(glm::vec2(-m[1], m[0]));
    j.offset = (glm::length(m) / 2.0f) * m + shape[a];
    joints.push_back(j);
}

void Piece::lock()
{
    rotation_locked = true;
}

void Piece::unlock()
{
    rotation_locked = false;
}

bool Piece::isLocked()
{
    return rotation_locked;
}

void Piece::setAngle(float theta)
{
    rotation = theta;
}