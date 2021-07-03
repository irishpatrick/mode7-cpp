#include "TexCache.hpp"
#include <map>
#include <iostream>
#include <memory>

namespace mode7
{

static std::map<std::string, std::shared_ptr<Texture>> cache;

Texture* TexCache::open(const std::string& fn, TexType type)
{
    if (cache.find(fn) == cache.end())
    {
        std::shared_ptr<Texture> tex = std::make_shared<Texture>();
        tex->open(fn, type);
        cache[fn] = tex;
        return tex.get();
    }

    return cache[fn].get();
}

}