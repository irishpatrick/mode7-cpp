#include "AI.hpp"
#include "gl.h"
#include <iostream>

AI::AI() :
    Car(),
    ticksLeft(0)
{

}

AI::~AI()
{

}

void AI::setRacingLine(RacingLine line)
{
    this->line = line;
}

void AI::update()
{
    glm::vec2 xz = line.getTarget();
    glm::vec3 target(xz.x, position.y, xz.y);
    glm::vec3 dir = glm::normalize(target - position);
    float dist = glm::distance(target, position);
    float cross = glm::cross(front, dir).y;
    //std::cout << dist << std::endl;
    //std::cout << "cross: " << cross << std::endl;
    //std::cout << "\rdist: " << dist << "\tcross: " << cross;
    //std::fflush(stdout);
    if (cross < -0.1f)
    {
        turnLeft();
    }

    if (cross > 0.1f)
    {
        turnRight();
    }

    uint32_t action = line.getAction();
    if (action == Point::ACCEL_PT)
    {
        ticksLeft = 0;
        //Car::gas();
    }
    else if (action == Point::COAST_PT)
    {

    }
    else if (action == Point::BRAKE_PT)
    {
        ticksLeft++;
        if (ticksLeft < 10)
        {
            Car::brake();
        }
    }

    /*if (dist > 20.f || (cross > -0.1f && cross < 0.1f))
    {
        Car::gas();
    }

    if (speed > 0.2f && dist < 200.f && fabs(cross) > 0.4f)
    {
        Car::brake();
    }*/

    Car::update();
    line.update(position);
}