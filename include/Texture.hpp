#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "gl.h"
#include <string>
#include <cstdint>

enum class TexType {DIFFUSE, SPECULAR, NORMAL, BUMP};

class Texture
{
public:

    Texture();
    ~Texture();

    void open(const std::string&, TexType);
    uint32_t getId();
    TexType getType();

    // static methods
    static unsigned int open(const std::string&);

private:

    TexType type;
    uint32_t id;
};

#endif /* TEXTURE_HPP */