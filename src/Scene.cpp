/**
 * This file is part of mode7-cpp.
 * Copyright (C) 2021  Patrick Roche
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
**/

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