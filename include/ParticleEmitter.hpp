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

#ifndef PARTICLEEMITTER_HPP
#define PARTICLEEMITTER_HPP

#include "Particle.hpp"
#include "Shader.hpp"
#include "Scheduler.hpp"
#include "ParticleUpdateWorker.hpp"

#include "gl.hpp"
#include <cstdint>
#include <vector>
#include <queue>

namespace mode7
{
    class ParticleEmitter
    {
    public:
        ParticleEmitter();
        ~ParticleEmitter();

        void init(uint32_t, uint32_t);
        void update();
        void draw();

        inline void setDirectionSpread(float val)
        {
            m_dirSpread = val;
        }

        inline void setVelocitySpread(float val)
        {
            m_velSpread = val;
        }

        inline void setLifeSpread(float val)
        {
            m_lifeSpread = val;
        }

        inline void setSpeed(float val)
        {
            m_speed = val;
        }

        inline void enable()
        {
            m_enabled = true;
        }

        inline void disable()
        {
            m_enabled = false;
        }

        glm::vec3 randomRadius(float);
        float randomScalar(float);

        Particle* firstAvailable();

        glm::vec3 position;
        glm::vec3 direction;

    private:
        Shader m_shader;
        uint32_t n_particles;
        bool m_enabled;
        float m_speed;
        float m_defaultLife;
        float m_dirSpread;
        float m_velSpread;
        float m_lifeSpread;
        std::vector<Particle> m_particles;
        std::queue<Particle*> m_ready;
        Scheduler<ParticleUpdateWorker> m_updateScheduler;
    };
} // namespace mode7


#endif /* PARTICLEEMITTER_HPP */