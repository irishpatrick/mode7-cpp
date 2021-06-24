#ifndef MESHUPDATEWORKER_HPP
#define MESHUPDATEWORKER_HPP

#include "Worker.hpp"

#include <cstdint>

namespace mode7
{

class MeshUpdateWorker : public Worker
{
public:
    MeshUpdateWorker(uint32_t);
    virtual ~MeshUpdateWorker();

    virtual void* job(void*);
};

}

#endif /* MESHUPDATEWORKER_HPP */