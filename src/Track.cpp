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

static void next_vec2(std::vector<unsigned char>::iterator& it, glm::vec2& v)
{
    std::memcpy(&v[0], it.base(), sizeof(float));
    it += sizeof(float);

    std::memcpy(&v[1], it.base(), sizeof(float));
    it += sizeof(float);
}

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
    std::ifstream in(fn, std::ios::binary);
    if (!in)
    {
        return;
    }
    std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(in), {});

    auto it = buffer.begin();
    std::string magic(it, it + 4);
    it += 4;
    if (magic != "TDAT")
    {
        return;
    }

    int n_seg;
    std::memcpy(&n_seg, it.base(), sizeof(int));
    it += sizeof(int);

    int start;
    std::memcpy(&start, it.base(), sizeof(int));
    it += sizeof(int);

    int stride;
    std::memcpy(&stride, it.base(), sizeof(int));
    it += sizeof(int);

    Line2D cl;
    Rect tb;
    Rect rb;
    Rect wb;
    glm::vec2 a;
    glm::vec2 b;
    glm::vec2 c;
    glm::vec2 d;
    for (int i = 0; i < n_seg; ++i)
    {
        next_vec2(it, a);
        next_vec2(it, b);
        cl.create(a, b);

        next_vec2(it, a);
        next_vec2(it, b);
        next_vec2(it, c);
        next_vec2(it, d);
        tb.connect(a, b, c, d);
    
        next_vec2(it, a);
        next_vec2(it, b);
        next_vec2(it, c);
        next_vec2(it, d);
        rb.connect(a, b, c, d);

        next_vec2(it, a);
        next_vec2(it, b);
        next_vec2(it, c);
        next_vec2(it, d);
        wb.connect(a, b, c, d);
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