/**
 * This file is part of mode7-cpp.
 * Copyright (C) 2021  Patrick Roche
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
**/

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
