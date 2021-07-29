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

#ifndef DEBUGPATH_HPP
#define DEBUGPATH_HPP

#include "gl.hpp"
#include "Object.hpp"
#include "Shader.hpp"
#include <vector>

namespace mode7
{

class DebugPath : public Object
{
public:

    DebugPath();
    virtual ~DebugPath();

    void init();
    void createFromPoints(std::vector<glm::vec2>&);
    void draw();

    inline bool isReady()
    {
        return m_isReady;
    }

    inline void setColor(float r, float g, float b)
    {
        m_color = glm::vec3(r, g, b);
    }

private:

    bool m_isReady;
    glm::vec3 m_color;
    GLuint vao;
    GLuint vbo;
    GLuint num_lines;
    GLuint m_colorUniform;
    Shader shader;
};

}

#endif /* DEBUGPATH_HPP */

#endif /* _BUILD_DEBUG_TOOLS */