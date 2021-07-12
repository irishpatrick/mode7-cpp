#include "ParticleEmitter.hpp"

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>

namespace mode7
{
    ParticleEmitter::ParticleEmitter() :
        position(0.0),
        direction(0.0, 1.0, 3.0),
        n_particles(0),
        m_enabled(false),
        m_speed(2.0),
        m_defaultLife(1.0),
        m_dirSpread(0.0),
        m_velSpread(0.0),
        m_lifeSpread(0.0)
    {

    }

    ParticleEmitter::~ParticleEmitter()
    {

    }

    void ParticleEmitter::init(uint32_t np, uint32_t tid)
    {
        m_shader.open("assets/shaders/particle_v.glsl", "assets/shaders/particle_f.glsl");
        
        n_particles = np;
        m_particles.resize(n_particles);
        for (uint32_t i = 0; i < np; ++i)
        {
            m_ready.push(&m_particles[i]);

            m_particles[i].init(&m_shader, tid, glm::vec4(1.0, 1.0, 1.0, 1.0));
        }
    }

    Particle* ParticleEmitter::firstAvailable()
    {
        Particle* out;
        out = m_ready.front();
        m_ready.pop();
        return out;
    }

    glm::vec3 ParticleEmitter::randomRadius(float mag)
    {
        srand(time(NULL));

        // compute basis vectors
        glm::vec3 nprime(-direction.x, direction.x, direction.y);
        glm::vec3 p = position + nprime;
        float dist = glm::dot(p - position, direction);
        glm::vec3 delta = glm::normalize(direction) * dist;
        glm::vec3 b1 = glm::normalize(p - delta);
        glm::vec3 b2 = glm::cross(glm::normalize(direction), b1);

        float sq2 = sqrtf(2.0);
        float rx = sq2 * mag * static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        float ry = sq2 * mag * static_cast<float>(rand()) / static_cast<float>(RAND_MAX);

        return rx * b1 + ry * b2;
    }

    float ParticleEmitter::randomScalar(float mag)
    {
        return mag * static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    }

    void ParticleEmitter::update()
    {
        if (!m_enabled)
        {
            return;
        }

        uint32_t n_active = m_particles.size() - m_ready.size();
        if (n_active < n_particles)
        {
            Particle* next = m_ready.front();
            m_ready.pop();
            next->spawn(
                position,
                (m_speed + randomScalar(m_velSpread)) * glm::normalize(direction + randomRadius(m_dirSpread)),
                m_defaultLife + randomScalar(m_lifeSpread)
            );
            n_active = m_particles.size() - m_ready.size();
        }

        bool done;
        for (auto& e : m_particles)
        {
            if (!e.isVisible())
            {
                continue;
            }

            //e.m_pos = position;
            done = e.update();
            if (done)
            {
                m_ready.push(&e);
            } 
        }
    }

    void ParticleEmitter::draw()
    {
        if (!m_enabled)
        {
            return;
        }

        glEnable(GL_BLEND);
        //glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);
        glDisable(GL_DEPTH_TEST);
        for (auto& e : m_particles)
        {
            if (!e.isVisible())
            {
                continue;
            }
            e.draw();
        }
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_DEPTH_TEST);
    }
} // namespace mode7
