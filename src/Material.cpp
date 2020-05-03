#include "Material.hpp"

Material::Material() :
    tile(1)
{

}

Material::~Material()
{

}

void Material::addDiffuseMap(unsigned int id)
{
    diffuseMaps.push_back(id);
}

unsigned int Material::getDiffuseMap(unsigned int index)
{
    return diffuseMaps[index];
}

unsigned int Material::numDiffuseMaps()
{
    return diffuseMaps.size();
}