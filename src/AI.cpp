#include "AI.hpp"
#include "gl.hpp"
#include <iostream>

namespace mode7
{

AI::AI() :
    Car(),
    m_actionCooldown(0),
    m_currentZone(0),
    m_lastAction(-1)
{

}

AI::~AI()
{

}

void AI::control()
{
    if (m_actionCooldown > 0)
    {
        --m_actionCooldown;
    }
    else
    {
        int cur = m_racingLine->getCurrentIndex(
            glm::vec2(position.x, position.z),
            m_currentZone);

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
        float distToNext = glm::length(
            (next.p() - glm::vec2(position.x, position.z)));
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
            glm::vec3(line.v().x, 0.f, line.v().y),
            Car::front
        );
        float cross_next = cross_next_vec.y;

        int moving_right = cross_line > 0.f;
        int moving_left = cross_line < 0.f;
        int right_of_line = distToLine > 0.f;
        int left_of_line = distToLine < 0.f;
        int dist_threshold = absDistToLine > 1.f;

        if (right_of_line && moving_right && dist_threshold)
        {
            //std::cout << "turn left" << std::endl;
            //Car::m_wheelState = LEFT;
            rotate(0.f, 0.1f, 0.f);
        }

        else if (left_of_line && moving_left && dist_threshold)
        {
            //std::cout << "turn right" << std::endl;
            //Car::m_wheelState = RIGHT;
            rotate(0.f, -0.1f, 0.f);
        }

        //if (absDistToLine > 1.f)
        if (fabsf(cross_next) > 0.01 && distToNext < 20.f)
        {
            m_lastAction = 1;
            if (velocity.z < 0.4)
            {
                m_lastAction = -1;
            }
            //velocity.z = 1.0f;
        }
        else
        {
            m_lastAction = 0;
            //velocity.z = 2.0f;
        }

        m_actionCooldown = 1;
    }

    if (m_lastAction == 0)
    {
        Car::state = ACCEL;
    }
    else if (m_lastAction == 1)
    {
        Car::state = BRAKE;
    }
}

void AI::update()
{
    if (m_racingLine)
    {
        control();
    }

    Car::updateControls();

    Object::move();

    velocity.z += m_power - (velocity.z * (0.005f + m_brake));
    velocity.x += drift - (velocity.x * 0.05f);
    //velocity.x += drift - (velocity.x * 0.005f);
    drift = 0.0f;

    Car::updateSprite();
    Car::shadow.update();
    Object::update();
    Car::sprite.update();
}

}
