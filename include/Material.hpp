#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "gl.h"
#include <vector>

class Material
{
public:

    Material();
    ~Material();

    void addDiffuseMap(unsigned int);
    unsigned int getDiffuseMap(unsigned int);
    unsigned int numDiffuseMaps();

    int tile;

private:

    std::vector<unsigned int> diffuseMaps;
    std::vector<unsigned int> specularMaps;
    unsigned int diffuseTex;
};

#endif /* MATERIAL_HPP */