#include "Collisions.hpp"
#include "Car.hpp"
#include <iostream>
#include <cstdint>

namespace mode7
{

Collisions::Collisions()
{

}

Collisions::~Collisions()
{

}

void Collisions::addObject(Object* m, float radius)
{
    m_objs.push_back(std::pair<Object*, float>(m, radius));
}

void Collisions::update()
{
    for (uint32_t i = 0; i < m_objs.size() - 1; ++i)
    {
        for (uint32_t j = i + 1; j < m_objs.size(); ++j)
        {
            auto& a = m_objs[i];
            auto& b = m_objs[j];

            glm::vec3 diff = a.first->position - b.first->position;
            diff.y = 0.f;
            float dist = glm::length(diff);

            if (dist <= a.second)
            {
                Car* c = dynamic_cast<Car*>(a.first);
                if (c != nullptr)
                {
                    c->stun();
                }
                //a.first->velocity = -glm::normalize(diff) * (1.0f / (2.f * dist));
                //a.first->velocity.z += -a.first->velocity.z * 0.2f;
                a.first->velocity.x += -a.first->velocity.z * -a.first->velocity.z * 0.2f;
                //a.first->velocity.x = -0.1f;
            }

            if (dist <= b.second)
            {
                //b.first->velocity += glm::normalize(diff) * 0.1f;
            }
        }
    }
}

}
