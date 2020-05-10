#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <vector>
#include <string>
#include <cstdint>
#include "Texture.hpp"
#include "Material.hpp"

class Animation : public Material
{
public:

    Animation();
    virtual ~Animation();

    void open(const std::string&);

    virtual Texture getMap(uint32_t);

private:

    uint32_t mCurrentFrame;
    std::vector<Texture> frames;
};

#endif /* ANIMATION_HPP */