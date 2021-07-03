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