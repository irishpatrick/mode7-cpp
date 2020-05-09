#include "Animation.hpp"
#include "TexCache.hpp"
#include <iostream>
#include <nlohmann/json.hpp>
#include <fstream>

using json = nlohmann::json;

Animation::Animation() :
    mCurrentFrame(0)
{
    
}

Animation::~Animation()
{

}

void Animation::open(const std::string& fn)
{
    std::ifstream in(fn);
    if (!in)
    {
        return;
    }

    json o;
    o << in;
    in.close();

    // parse
    for (auto& e : o["frames"])
    {
        frames.push_back(TexCache::open(e["diffuse"].get<std::string>(), TexType::DIFFUSE));
    }
}

Texture Animation::getFrame(uint32_t index)
{
    return frames[index];
}