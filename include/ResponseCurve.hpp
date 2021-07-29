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

#ifndef VCURVE_HPP
#define VCURVE_HPP

#include <string>
#include <vector>
#include "Line.hpp"

namespace mode7
{

typedef struct _Point
{
    uint32_t x;
    uint32_t y;
} Point;

class ResponseCurve
{
public:

    ResponseCurve();
    ~ResponseCurve();

    void open(const std::string&);
    float getX(float);
    float getY(float);

private:

    std::vector<Line> m_segments;
};

}

#endif /* VCURVE_HPP */
