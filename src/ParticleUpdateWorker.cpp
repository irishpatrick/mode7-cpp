#include "ParticleUpdateWorker.hpp"
#include "Particle.hpp"

namespace mode7
{

    ParticleUpdateWorker::ParticleUpdateWorker(uint32_t id) : 
        Worker(id)
    {

    }

    ParticleUpdateWorker::~ParticleUpdateWorker()
    {

    }

    void* ParticleUpdateWorker::job(void* data)
    {
        Particle* p = (Particle*)data;
        p->update();
        
        return nullptr;
    }

}