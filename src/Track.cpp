#include "Track.hpp"
#include "Tree.hpp"
#include "Object.hpp"
#include "Mesh.hpp"
#include "Texture.hpp"
#include "ModelLoader.hpp"

#include <nlohmann/json.hpp>
#include <fstream>

namespace mode7
{

using json = nlohmann::json;

Track::Track()
{

}

Track::~Track()
{

}

void Track::open(const std::string& fn)
{
    m_scene = std::move(ModelLoader::openUnique(fn));
}

void Track::attachData(const std::string& fn)
{
    std::ifstream in(fn);
    if (!in)
    {
        return;
    }
}

void Track::transformData(glm::vec3 translate, glm::vec3 rotate, glm::vec3 scale)
{
}

Scene* Track::getScene()
{
    return m_scene.get();
}

void Track::update()
{
}

void Track::draw(Shader& s)
{
}

void Track::destroy()
{
}

}