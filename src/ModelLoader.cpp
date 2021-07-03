#include "ModelLoader.hpp"
#include "Mesh.hpp"
#include "Texture.hpp"
#include "Util.hpp"
#include "TexCache.hpp"
#include <memory>
#include <map>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

namespace mode7
{

std::shared_ptr<Scene> ModelLoader::openShared(const std::string& fn)
{
    std::vector<std::shared_ptr<Mesh>> meshes;
    std::shared_ptr<Scene> s = std::make_shared<Scene>();
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(fn, aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "error::assimp::" << importer.GetErrorString() << std::endl;
        return s;
    }

    processNode(meshes, scene->mRootNode, scene, aiMatrix4x4());

    for (auto& e : meshes)
    {
        s->addMesh(e);
    }
    
    return s;
}

std::unique_ptr<Scene> ModelLoader::openUnique(const std::string& fn)
{
    std::vector<std::shared_ptr<Mesh>> meshes;
    std::unique_ptr<Scene> s(new Scene());
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(fn, aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "error::assimp::" << importer.GetErrorString() << std::endl;
        return s;
    }

    processNode(meshes, scene->mRootNode, scene, aiMatrix4x4());

    for (auto& e : meshes)
    {
        s->addMesh(e);
    }
    
    return s;
}

void ModelLoader::processNode(std::vector<std::shared_ptr<Mesh>>& vec, aiNode* node, const aiScene* scene, aiMatrix4x4 transform)
{
    std::cout << "process node: " << node->mName.C_Str() << std::endl;
    for (unsigned int i = 0; i < node->mNumMeshes; ++i)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        vec.push_back(processMesh(mesh, scene, transform * node->mTransformation));
    }

    for (unsigned int i = 0; i < node->mNumChildren; ++i)
    {
        processNode(vec, node->mChildren[i], scene, transform * node->mTransformation);
    }
}

std::shared_ptr<Mesh> ModelLoader::processMesh(aiMesh* mesh, const aiScene* scene, aiMatrix4x4 transform)
{
    std::cout << "\tprocess mesh: " << mesh->mName.C_Str() << std::endl;
    std::shared_ptr<Mesh> out = std::make_shared<Mesh>();
    Material mat;

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
    {
        Vertex v;

        v.position = glm::vec3(
            mesh->mVertices[i].x,
            mesh->mVertices[i].y,
            mesh->mVertices[i].z
        );

        v.normal = glm::vec3(
            mesh->mNormals[i].x,
            mesh->mNormals[i].y,
            mesh->mNormals[i].z
        );

        if (mesh->mTextureCoords[0])
        {
            v.uv = glm::vec2(
                mesh->mTextureCoords[0][i].x,
                mesh->mTextureCoords[0][i].y
            );
        }
        else
        {
            v.uv = glm::vec2(0.0f);
        }


        vertices.push_back(v);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; ++j)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Texture*> d = loadTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        for (auto& e : d)
        {
            mat.addMap(e);
        }
    }

    

    out->inherent = Util::fromAi(transform);
    out->setMaterial(mat);
    out->createFromArrays(vertices, indices);

    return out;
}

std::vector<Texture*> ModelLoader::loadTextures(aiMaterial* mat, aiTextureType type, const std::string& name)
{
    TexType tt;
    if (name == "texture_diffuse")
    {
        tt = TexType::DIFFUSE;
    }
    else if (name == "texture_specular")
    {
        tt = TexType::SPECULAR;
    }
    std::vector<Texture*> out;

    for (unsigned int i = 0; i < mat->GetTextureCount(type); ++i)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        std::string fn = "assets/textures/" + std::string(str.C_Str());
        out.push_back(TexCache::open(fn, tt));
    }
    return out;
}

}
