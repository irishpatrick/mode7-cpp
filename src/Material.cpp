#include "Material.hpp"

Material::Material() :
    diffuseTex(0)
{

}

Material::~Material()
{

}

void Material::setDiffuseTexture(GLuint id)
{
    diffuseTex = id;
}

GLuint Material::getDiffuseTexture()
{
    return diffuseTex;
}