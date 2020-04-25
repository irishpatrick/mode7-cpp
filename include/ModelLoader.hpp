#ifndef MODELLOADER_HPP
#define MODELLOADER_HPP

#include <string>
#include "Mesh.hpp"
#include "Material.hpp"

class ModelLoader
{
public:

    static Mesh open(const std::string&);

private:

    static Material openMaterial(const std::string&);
};

#endif /* MODELLOADER_HPP */