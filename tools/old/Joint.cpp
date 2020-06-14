#include "Joint.hpp"
#include "Piece.hpp"

Joint::Joint() :
    connection(nullptr),
    parent(nullptr)
{

}

Joint::~Joint()
{
    
}

void Joint::attach(Joint& other)
{
    if (other.parent == nullptr)
    {
        return;
    }

    if (other.parent->isLocked())
    {
        return;
    }

    connection = &other;
    align(*connection);
    other.parent->lock();
}

void Joint::align(Joint& other)
{
    // calc angle
    float theta;

    other.parent->setAngle(theta);
}

void Joint::flipNormal()
{
    normal = -normal;
}