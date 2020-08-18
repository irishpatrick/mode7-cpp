#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <vector>
#include <string>
#include <cstdint>
#include "Texture.hpp"
#include "Material.hpp"

namespace mode7
{

class Animation : public Material
{
public:

    Animation();
    virtual ~Animation();

    void open(const std::string&);
    void setCurrentFrame(uint32_t);

    virtual Texture getMap(uint32_t);
    virtual uint32_t numMaps();

private:

    uint32_t mCurrentFrame;
    std::vector<Texture> frames;
};

}

#endif /* ANIMATION_HPP */