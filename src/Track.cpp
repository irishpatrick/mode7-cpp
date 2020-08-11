#include "Track.hpp"
#include "Tree.hpp"
#include "Object.hpp"
#include "Mesh.hpp"
#include "Texture.hpp"
#include <nlohmann/json.hpp>
#include <fstream>

using json = nlohmann::json;

static Mesh mesh;
static std::vector<Tree> trees;

void Track::open(const std::string& fn)
{
    mesh.createFromShape(Mesh::PLANE);
    mesh.scale = glm::vec3(300.0f);
    //mesh.material.addDiffuseMap(Texture::open("assets/textures/track.png"));
    mesh.setRotation(M_PI / 2.0f, 0, 0);

    std::ifstream in(fn);
    if (!in)
    {
        std::cout << "cannot open " << fn << std::endl;
        return;
    }

    json o;
    in >> o;
    in.close();
}

void Track::update()
{
    mesh.update();
}

void Track::draw(Shader& s)
{
    mesh.draw(s);
    for (auto& e : trees)
    {
        e.draw(s);
    }
}

void Track::destroy()
{
}
