#ifndef MODELLOADER_HPP
#define MODELLOADER_HPP

#include "Scene.hpp"
#include <string>
#include <vector>
#include <assimp/scene.h>

class ModelLoader
{
public:

    static std::vector<Mesh> open(const std::string&);
    static void processNode(std::vector<Mesh>&, aiNode*, const aiScene*);
    static Mesh processMesh(aiMesh*, const aiScene*);
    static std::vector<unsigned int> loadTextures(aiMaterial*, aiTextureType, const std::string&);

private:


};

#endif /* MODELLOADER_HPP */