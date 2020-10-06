#ifndef SCENE_HPP
#define SCENE_HPP

#include <vector>
#include <memory>
#include "Mesh.hpp"
#include "Shader.hpp"
#include <string>
#include <sstream>

namespace mode7
{

class Scene : public Object
{
public:

    Scene();
    ~Scene();

    void addMesh(std::shared_ptr<Mesh>);
    Mesh* getMesh(unsigned int);

    void draw(Shader&);

    inline std::string query()
    {
        std::stringstream ss;
        int i = 0;
        for (auto& m : meshes)
        {
            ss << i << ": " << "..." << "\n";
            ++i;
        }

        return ss.str();
    }

private:

    std::vector<std::shared_ptr<Mesh>> meshes;
};

}

#endif /* SCENE_HPP */
