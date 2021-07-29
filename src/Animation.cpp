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

#include "Animation.hpp"
#include "TexCache.hpp"
#include <iostream>
#include <nlohmann/json.hpp>
#include <fstream>

using json = nlohmann::json;

namespace mode7
{

Animation::Animation() :
    mCurrentFrame(0)
{
    
}

Animation::~Animation()
{

}

void Animation::open(const std::string& fn)
{
    std::cout << "[Animation] open " << fn << std::endl;
    std::ifstream in(fn);
    if (!in)
    {
        std::cout << "cannot open " << fn << std::endl;
        return;
    }

    json o;
    in >> o;
    in.close();

    // parse
    for (auto& e : o["frames"])
    {
        std::cout << "[Animation] add " << e["diffuse"].get<std::string>() << std::endl;
        frames.push_back(TexCache::open(e["diffuse"].get<std::string>(), TexType::DIFFUSE));
    }
}

void Animation::setCurrentFrame(uint32_t index)
{
    mCurrentFrame = index;
}

Texture* Animation::getMap(uint32_t)
{
    return frames[mCurrentFrame];
}

uint32_t Animation::numMaps()
{
    return 1;
}

}