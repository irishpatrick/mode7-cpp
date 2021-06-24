#include "MeshUpdateWorker.hpp"
#include "Object.hpp"

namespace mode7
{

    MeshUpdateWorker::MeshUpdateWorker(uint32_t id) : 
        Worker(id)
    {

    }

    MeshUpdateWorker::~MeshUpdateWorker()
    {

    }

    void* MeshUpdateWorker::job(void* data)
    {
        Object* m = (Object*)data;
        m->update();
        
        return nullptr;
    }

}