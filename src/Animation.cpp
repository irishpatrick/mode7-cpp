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
    std::cout << "[Animation] open " << fn << std::endl;
    std::ifstream in(fn);
    if (!in)
    {
        std::cout << "cannot open " << fn << std::endl;
        return;
    }

    json o;
    o << in;
    in.close();

    // parse
    for (auto& e : o["frames"])
    {
        std::cout << "[Animation] add " << e["diffuse"].get<std::string>() << std::endl;
        frames.push_back(TexCache::open(e["diffuse"].get<std::string>(), TexType::DIFFUSE));
    }
}

void Animation::setCurrentFrame(uint32_t index)
{
    mCurrentFrame = index;
}

Texture Animation::getMap(uint32_t)
{
    return frames[mCurrentFrame];
}

uint32_t Animation::numMaps()
{
    return 1;
}