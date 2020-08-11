#include "AI.hpp"
#include "gl.h"
#include <iostream>

AI::AI() :
    Car(),
    m_currentZone(0),
    m_racingLine(nullptr)
{

}

AI::~AI()
{

}

void AI::setRacingLine(RacingLine* line)
{
    m_racingLine = line;
}

void AI::control()
{
    int cur = m_racingLine->getCurrentIndex(Car::position, m_currentZone);
    if (cur > 0)
    {
        m_currentZone = cur;
    }
    Line2D line = m_racingLine->getLine(m_currentZone);
    Line2D next = m_racingLine->getNext(m_currentZone);

    float distToLine = line.distTo(Car::position);
    float distToNext = (next.p() - glm::vec2(position.x, position.z)).length();
}

void AI::update()
{
    if (m_racingLine)
    {
        control();
    }
    Car::update();
}