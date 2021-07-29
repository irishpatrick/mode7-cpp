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

#ifndef UTIL_HPP
#define UTIL_HPP

#include <assimp/Importer.hpp>
#include "gl.hpp"
#include "Line2D.hpp"

#include <vector>

using namespace mode7;

class Util
{
public:

    static void seed();
    static int randint(int, int);
    static glm::vec3 xAxis();
    static glm::vec3 yAxis();
    static glm::vec3 zAxis();
    static bool sat(std::vector<Line2D*>&, std::vector<Line2D*>&);
    static float lerp(float, float, float);
    static float map(float, float, float, float, float);
    static glm::mat4 fromAi(const aiMatrix4x4&);
    static std::pair<int, int> getMonitorRes();
    static int getMonitorScale();

    static inline float constrain(float val, float min, float max)
    {
        if (val <= min) val = min;
        if (val >= max) val = max;
        return val;
    }

private:


};

#endif /* UTIL_HPP */
