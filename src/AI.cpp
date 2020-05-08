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
    Car::gas();
    //Car::turnLeft();
    //position -= 1.1f * front;
    glm::vec2 xz = line.getTarget();
    glm::vec3 target(xz.x, position.y, xz.y);
    glm::vec3 dir = glm::normalize(target - position);
    float dist = glm::distance(target, position);
    std::cout << dist << std::endl;
    //float theta = glm::angle(front, dir);
    //std::cout << glm::to_string(dir) << std::endl;
    //std::cout << "theta: " << theta * 180.f / M_PI << std::endl;
    //std::cout << "cross: " << glm::to_string(glm::cross(front, dir)) << std::endl;

    float cross = glm::cross(front, dir).y;
    //std::cout << "cross: " << cross << std::endl;
    if (cross < -0.2)
    {
        turnLeft();
    }

    if (cross > 0.2)
    {
        turnRight();
    }

    if (cross > -0.1 && cross < 0.1)
    {
        Car::gas();
    }

    if (dist < 200 && fabs(cross) > 0.8)
    {
        Car::brake();
    }

    Car::update();
    line.update(position);
}