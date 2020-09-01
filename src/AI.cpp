#include "AI.hpp"
#include "gl.h"
#include <iostream>

namespace mode7
{

AI::AI() :
    Car(),
    m_currentZone(0)
{

}

AI::~AI()
{

}

void AI::control()
{
    int cur = m_racingLine->getCurrentIndex(glm::vec2(position.x, position.z), m_currentZone);
    if (cur >= 0)
    {
        m_currentZone = cur;
    }
    else
    {
    }

    Line2D line = m_racingLine->getLine(m_currentZone);
    Line2D next = m_racingLine->getNext(m_currentZone);
    
    float distToLine = line.distTo(glm::vec2(position.x, position.z));
    float distToNext = (next.p() - glm::vec2(position.x, position.z)).length();
    float absDistToLine = fabsf(distToLine);
    float dot_line = glm::dot(
        line.v(), 
        glm::vec2(Car::front.x, Car::front.z)
    );
    glm::vec3 cross_line_vec = glm::cross(
        glm::vec3(line.v().x, 0.f, line.v().y),
        Car::front
    );
    float cross_line = cross_line_vec.y;
    float dot_next = glm::dot(
        next.normal(),
        glm::vec2(Car::front.x, Car::front.z)
    );
    glm::vec3 cross_next_vec = glm::cross(
        glm::vec3(line.normal().x, 0.f, line.normal().y),
        Car::front
    );
    float cross_next = cross_next_vec.y;

    //std::cout << front.x << "," << front.z << "\t" << line.v().x << "," << line.v().y << std::endl;

    int moving_right = cross_line > 0.f;
    int moving_left = cross_line < 0.f;
    int right_of_line = distToLine > 0.f;
    int left_of_line = distToLine < 0.f;
    int dist_threshold = absDistToLine > 1.f;

    if (cur >= 0 && false)
    {
        /*std::cout <<
            getWorldPosition().x << "," <<
            getWorldPosition().z << "\t" <<

            distToLine << "," <<
            distToNext << "," <<
            dot_line << "," <<
            cross_line << "," <<
            dot_next << "," <<
            cross_next << "\t" <<
            
            moving_right << "," <<
            moving_left << "," <<
            right_of_line << "," <<
            left_of_line << "," <<
            dist_threshold << "," <<
            std::endl;*/
    }

    //velocity.z = -0.3f * (0.01f + 0.05f * dot_next);
    velocity.z = 1.5f; 

    if (right_of_line && moving_right && dist_threshold)
    {
        //Car::turnLeft();
        //std::cout << "turn left" << std::endl;
        rotate(0.f, 0.1f, 0.f);
    }

    else if (left_of_line && moving_left && dist_threshold)
    {
        //Car::turnRight();
        //std::cout << "turn right" << std::endl;
        rotate(0.f, -0.1f, 0.f);
    }
}

void AI::update()
{
    if (m_racingLine)
    {
        control();
    }
    
    Object::move();
    Car::updateSprite();
    Car::shadow.update();
    Object::update();
    Car::sprite.update();
}

}
