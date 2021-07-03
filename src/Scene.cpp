#include "Scene.hpp"

#include <cassert>
#include <iostream>

namespace mode7
{

Scene::Scene() : 
    Object()
{

}

Scene::~Scene()
{

}

void Scene::addMesh(std::shared_ptr<Mesh> mesh)
{
    meshes.push_back(mesh);
    addChild((Object*)meshes.back().get());
}

Mesh* Scene::getMesh(unsigned int index)
{
    return meshes[index].get();
}

void Scene::draw(Shader& s)
{
    for (auto& e : meshes)
    {
        e->draw(s);
    }
}

}