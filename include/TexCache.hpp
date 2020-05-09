#ifndef TEXCACHE_HPP
#define TEXCACHE_HPP

#include <string>
#include "Texture.hpp"

class TexCache
{
public:

    static Texture open(const std::string&, TexType);

};

#endif /* TEXCACHE_HPP */