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

#ifndef MODELLOADER_HPP
#define MODELLOADER_HPP

#include "Scene.hpp"
#include "Texture.hpp"

#include <string>
#include <vector>
#include <assimp/scene.h>
#include <memory>

namespace mode7
{

class ModelLoader
{
public:

    static std::unique_ptr<Scene> openUnique(const std::string&);
    static std::shared_ptr<Scene> openShared(const std::string&);
    static void processNode(std::vector<std::shared_ptr<Mesh>>&, aiNode*, const aiScene*, aiMatrix4x4);
    static std::shared_ptr<Mesh> processMesh(aiMesh*, const aiScene*, aiMatrix4x4);
    static std::vector<Texture*> loadTextures(aiMaterial*, aiTextureType, const std::string&);

private:

};

}

#endif /* MODELLOADER_HPP */