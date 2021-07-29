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

#ifndef LINE_HPP
#define LINE_HPP

#include <string>
#include <sstream>

namespace mode7
{

class Line
{
public:

    Line();
    ~Line();

    void create(float, float);
    void create(float, float, float, float);

    float solve(float);
    float solveInv(float);

    inline std::string str()
    {
        std::stringstream ss;
        ss << "(" << x0 << "," << y0 << ") ";
        ss << "(" << x1 << "," << y1 << ") ";
        ss << "y = " << m << " * x + " << b;
        return ss.str();
    }

    float m;
    float b;

    float x0, y0;
    float x1, y1;
};

}

#endif /* LINE_HPP */
