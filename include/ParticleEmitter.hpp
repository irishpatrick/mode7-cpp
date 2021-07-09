#ifndef PARTICLEEMITTER_HPP
#define PARTICLEEMITTER_HPP

#include "Particle.hpp"
#include "Shader.hpp"
#include "Scheduler.hpp"
#include "ParticleUpdateWorker.hpp"

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

        void init();
        void update();
        void draw();
        Particle* firstAvailable();

    private:
        Shader* m_shader;
        uint32_t n_particles;
        std::vector<Particle> m_particles;
        std::queue<Particle*> m_ready;
        Scheduler<ParticleUpdateWorker> m_updateScheduler;
    };
} // namespace mode7


#endif /* PARTICLEEMITTER_HPP */