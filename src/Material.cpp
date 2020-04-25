#include "Material.hpp"

Material::Material() :
    tile(1),
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