#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <vector>
#include <string>
#include <cstdint>
#include "Texture.hpp"

class Animation
{
public:

    Animation();
    ~Animation();

    void open(const std::string&);
    Texture getFrame(uint32_t);

private:

    uint32_t mCurrentFrame;
    std::vector<Texture> frames;
};

#endif /* ANIMATION_HPP */