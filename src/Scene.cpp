#include "Scene.hpp"

Scene::Scene()
{

}

Scene::~Scene()
{

}

void Scene::addMesh(Mesh* mesh)
{
    meshes.push_back(mesh);
}

Mesh Scene::getMesh(unsigned int index)
{
    return *meshes[index];
}