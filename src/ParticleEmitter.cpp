#include "ParticleEmitter.hpp"

namespace mode7
{
    ParticleEmitter::ParticleEmitter()
    {

    }

    ParticleEmitter::~ParticleEmitter()
    {

    }

    Particle* ParticleEmitter::firstAvailable()
    {
        Particle* out;
        out = m_ready.front();
        m_ready.pop();
        return out;
    }

    void ParticleEmitter::update()
    {
        for (auto& e : m_particles)
        {
            if (!e.m_visible)
            {
                continue;
            }

            if (e.m_life <= 0)
            {
                e.m_visible = false;
                m_ready.push(&e);
            }
        }
    }

    void ParticleEmitter::draw()
    {
        for (auto& e : m_particles)
        {
            if (!e.m_visible)
            {
                continue;
            }
            e.draw(m_shader);
        }
    }
} // namespace mode7
