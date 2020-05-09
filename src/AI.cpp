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
    if (cross < -0.1)
    {
        turnLeft();
    }

    if (cross > 0.1)
    {
        turnRight();
    }

    if (dist > 20 || cross > -0.1 && cross < 0.1)
    {
        Car::gas();
    }

    if (speed > 0.2f && dist < 200 && fabs(cross) > 0.4)
    {
        Car::brake();
    }

    Car::update();
    line.update(position);
}