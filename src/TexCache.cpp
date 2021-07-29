/**
 * This file is part of mode7-cpp.
 * Copyright (C) 2021  Patrick Roche
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
**/

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