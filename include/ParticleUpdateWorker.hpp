#ifndef PARTICLEUPDATEWORKER_HPP
#define PARTICLEUPDATEWORKER_HPP

#include "Worker.hpp"

#include <cstdint>

namespace mode7
{

    class ParticleUpdateWorker : public Worker
    {
    public:
        ParticleUpdateWorker(uint32_t);
        virtual ~ParticleUpdateWorker();

        virtual void* job(void*);
    };

}

#endif /* PARTICLEUPDATEWORKER_HPP */