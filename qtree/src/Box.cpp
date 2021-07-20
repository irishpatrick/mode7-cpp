#include "Box.hpp"

Box::Box() :
    m_x(0),
    m_y(0),
    m_w(0),
    m_h(0)
{

}

Box::~Box()
{

}

void Box::create(float x, float y, float w, float h)
{
    m_x = x;
    m_y = y;
    m_w = w;
    m_h = h;
}

void Box::center(float x, float y, float w, float h)
{
    m_x = x - w / 2.0f;
    m_y = y - h / 2.0f;
    m_w = w;
    m_h = h;
}

bool Box::intersects(Box other)
{
    return 
        m_x < other.m_x + other.m_w &&
        m_x + m_w > other.m_x &&
        m_y < other.m_y + other.m_h &&
        m_y + m_h > other.m_y;
}