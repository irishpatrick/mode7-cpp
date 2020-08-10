#ifndef MODELLOADER_HPP
#define MODELLOADER_HPP

#include <string>
#include <fstream>
#include <vector>
#include <map>
#include "Mesh.hpp"
#include "Material.hpp"
#include "Group.hpp"

class ModelLoader
{
public:

    static Mesh open(const std::string&);
    static Mesh* parse(std::ifstream&);
    static Group openMulti(const std::string&);
    static std::vector<Mesh> openAll(const std::string&);

private:

    static Material openMaterial(const std::string&);
    static std::map<std::string, Material> openAllMaterials(const std::string&);
};

#endif /* MODELLOADER_HPP */