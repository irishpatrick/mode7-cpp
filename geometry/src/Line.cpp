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

#include "Line.hpp"

namespace mode7
{

Line::Line() :
    m(0.0f),
    b(0.0f)
{

}

Line::~Line()
{

}

void Line::create(float m, float b)
{
    this->m = m;
    this->b = b;
}

void Line::create(float x1, float y1, float x2, float y2)
{
    this->x0 = x1;
    this->x1 = x2;
    this->y0 = y1;
    this->y1 = y2;
    m = (y2 - y1) / (x2 - x1);
    b = y1 - (m * x1);
}

float Line::solve(float x)
{
    return m * x + b;
}

float Line::solveInv(float y)
{
    return (y - b) / m;
}

}
