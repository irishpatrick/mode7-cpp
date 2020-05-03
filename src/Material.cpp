#include "Material.hpp"

Material::Material() :
    tile(1)
{

}

Material::~Material()
{

}

void Material::addMap(Texture map)
{
    maps.push_back(map);
}

Texture Material::getMap(unsigned int index)
{
    return maps[index];
}

unsigned int Material::numMaps()
{
    return maps.size();
}