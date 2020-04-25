#include "Line.hpp"

Line::Line() :
    m(0.0f),
    b(0.0f)
{

}

Line::~Line()
{

}

void Line::create(float m, float b)
{
    this->m = m;
    this->b = b;
}

void Line::create(float x1, float y1, float x2, float y2)
{
    m = (y2 - y1) / (x2 - x1);
    b = y1 - (m * x1);
}

float Line::solve(float x)
{
    return m * x + b;
}

float Line::solveInv(float y)
{
    return (y - b) / m;
}