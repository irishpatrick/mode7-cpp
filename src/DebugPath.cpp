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

#ifdef _BUILD_DEBUG_TOOLS

#include "DebugPath.hpp"

#include <iostream>

namespace mode7
{

DebugPath::DebugPath() :
    Object(),
    m_isReady(false),
    vao(-1),
    vbo(-1),
    num_lines(0),
    m_color(1.0, 0.0, 1.0)
{

}

DebugPath::~DebugPath()
{

}

void DebugPath::init()
{
    shader.open("assets/shaders/debugpath_v.glsl", "assets/shaders/debugpath_f.glsl");
    m_colorUniform = glGetUniformLocation(shader.pid(), "color");
}

void DebugPath::createFromPoints(std::vector<glm::vec2>& points)
{
    m_isReady = true;
    std::vector<glm::vec3> converted;
    converted.reserve(points.size());
    for (auto& e : points)
    {
        converted.push_back(glm::vec3(e.x, 0.f, e.y));
    }
    num_lines = converted.size();

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, converted.size() * sizeof(glm::vec3), &converted[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0, 3,
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float),
        (void*)0
    );
}

void DebugPath::draw()
{
    if (m_isReady)
    {
        shader.use();
        glUniform3fv(m_colorUniform, 1, &m_color[0]);
        shader.setModel(*this);

        glBindVertexArray(vao);
        glDrawArrays(GL_LINES, 0, num_lines);
    }
}

}

#endif /* _BUILD_DEBUG_TOOLS */