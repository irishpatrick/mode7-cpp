#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "gl.h"
#include "Texture.hpp"
#include <vector>

class Material
{
public:

    Material();
    ~Material();

    void addMap(Texture);

    Texture getMap(unsigned int);
    unsigned int numMaps();

    int tile;

private:

    std::vector<Texture> maps;
};

#endif /* MATERIAL_HPP */