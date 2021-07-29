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

#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include "gl.hpp"

namespace mode7
{
    // forward declarations
    class Shader;

    class Particle
    {
    public:
        Particle();
        ~Particle();

        void init(Shader*, uint32_t, glm::vec4);
        void initGeometry();
        void setShader(Shader*);

        inline bool isVisible()
        {
            return m_life > 0.0;
        }

        inline void spawn(glm::vec3 pos, glm::vec3 vel, float life)
        {
            m_pos = pos;
            m_vel = vel;
            m_life = life;
            m_maxLife = life;
        }

        inline void setRate(float rate)
        {
            m_rate = rate;
        }

        inline void setColor(glm::vec4 rgba)
        {
            m_color = rgba;
        }

        bool update();
        void drawTriangles();
        void draw();

    private:
        glm::vec3 m_pos;
        glm::vec3 m_vel;
        glm::vec4 m_color;

        float m_life;
        float m_maxLife;
        float m_rate;

        Shader* m_shader;
        GLuint m_posShaderLoc;
        GLuint m_velShaderLoc;
        GLuint m_colorShaderLoc;
        GLuint m_timeShaderLoc;
        GLuint m_texShaderLoc;

        GLuint m_textureId;

        GLuint m_vao;

        friend class ParticleEmitter;
    };
} // namespace mode7


#endif /* PARTICLE_HPP */