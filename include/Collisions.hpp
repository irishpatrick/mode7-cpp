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

#ifndef COLLISIONS_HPP
#define COLLISIONS_HPP

#include <vector>
#include "Object.hpp"
#include "gl.hpp"

namespace mode7
{

class Collisions
{
public:

    Collisions();
    ~Collisions();

    void addObject(Object*, float);
    void update();

private:

    std::vector<std::pair<Object*, float>> m_objs;
};

}

#endif /* COLLISIONS_HPP */
