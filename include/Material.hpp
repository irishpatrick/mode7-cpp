#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "gl.h"
#include "Texture.hpp"
#include <vector>

class Material
{
public:

    Material();
    virtual ~Material();

    void addMap(Texture);

    virtual Texture getMap(unsigned int);
    unsigned int numMaps();

    int tile;

protected:

    std::vector<Texture> maps;
};

#endif /* MATERIAL_HPP */