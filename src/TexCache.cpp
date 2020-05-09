#include "TexCache.hpp"
#include <map>
#include <iostream>

static std::map<std::string, Texture> cache;

Texture TexCache::open(const std::string& fn, TexType type)
{
    if (cache.find(fn) == cache.end())
    {
        Texture tex;
        tex.open(fn, type);
        cache[fn] = tex;
        return tex;
    }

    return cache[fn];
}
