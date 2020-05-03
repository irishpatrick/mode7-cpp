#ifndef SCENE_HPP
#define SCENE_HPP

#include <vector>
#include "Mesh.hpp"

class Scene
{
public:

    Scene();
    ~Scene();

    void addMesh(Mesh*);
    Mesh getMesh(unsigned int);
    
    void update();
    void draw();

private:

    std::vector<Mesh*> meshes;
};

#endif /* SCENE_HPP */