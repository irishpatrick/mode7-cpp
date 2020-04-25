#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "gl.h"
#include <string>

class Texture
{
public:

    static GLuint open(const std::string&);
};

#endif /* TEXTURE_HPP */