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

#ifndef LINE2D_HPP
#define LINE2D_HPP

#include "gl.hpp"

#include <string>
#include <sstream>

namespace mode7
{

class Line2D
{
public:

    inline Line2D() {}
    inline ~Line2D() {}

    inline std::string str()
    {
        std::stringstream ss;
        ss << glm::to_string(m_p) << " --> " << glm::to_string(m_e);
        return ss.str();
    }

    inline void create(glm::vec2 a, glm::vec2 b)
    {
        m_p = a;
        m_e = a;
        m_v = glm::normalize(b);
    }

    inline void fromPoints(glm::vec2 a, glm::vec2 b)
    {
        m_v = glm::normalize(b - a);
        m_p = a;
        m_e = b;
    }

    inline glm::vec2 normal()
    {
        return glm::vec2(-m_v.y, m_v.x);
    }

    inline glm::vec2 solve(float t)
    {
        return m_p + (t * m_v);
    }

    inline float distTo(glm::vec2 pt)
    {
        return glm::dot((pt - m_p), normal());
    }

    inline float length()
    {
        return glm::length(m_e - m_p);
    }

    inline glm::vec2 p()
    {
        return m_p;
    }

    inline glm::vec2 e()
    {
        return m_e;
    }

    inline glm::vec2 v()
    {
        return m_v;
    }

private:

    glm::vec2 m_p;
    glm::vec2 m_e;
    glm::vec2 m_v;
};

}

#endif /* LINE2D_HPP */
