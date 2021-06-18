#include "MeshUpdateWorker.hpp"
#include "Mesh.hpp"

namespace mode7
{

MeshUpdateWorker::MeshUpdateWorker() : 
    Worker()
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