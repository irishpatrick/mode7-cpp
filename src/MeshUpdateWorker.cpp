#include "MeshUpdateWorker.hpp"
#include "Mesh.hpp"

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
    Mesh* m = (Mesh*)data;
    m->update();
}

}