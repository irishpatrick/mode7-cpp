#ifndef MESHUPDATEWORKER_HPP
#define MESHUPDATEWORKER_HPP

#include "Worker.hpp"

namespace mode7
{

class MeshUpdateWorker : public Worker
{
public:
    MeshUpdateWorker();
    virtual ~MeshUpdateWorker();

    virtual void* job(void*);
};

}

#endif /* MESHUPDATEWORKER_HPP */