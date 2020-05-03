#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "gl.h"
#include <string>
#include <cstdint>

typedef enum {DIFFUSE, SPECULAR, NORMAL, BUMP} textype_t;

class Texture
{
public:

    Texture();
    ~Texture();

    void open(const std::string&, textype_t);
    uint32_t getId();
    textype_t getType();

    // static methods
    static unsigned int open(const std::string&);

private:

    textype_t type;
    uint32_t id;
};

#endif /* TEXTURE_HPP */