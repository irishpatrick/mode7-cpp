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

#include "Box.hpp"

Box::Box() :
    m_x(0),
    m_y(0),
    m_w(0),
    m_h(0)
{

}

Box::~Box()
{

}

void Box::create(float x, float y, float w, float h)
{
    m_x = x;
    m_y = y;
    m_w = w;
    m_h = h;
}

void Box::center(float x, float y, float w, float h)
{
    m_x = x - w / 2.0f;
    m_y = y - h / 2.0f;
    m_w = w;
    m_h = h;
}

bool Box::intersects(Box other)
{
    return 
        m_x < other.m_x + other.m_w &&
        m_x + m_w > other.m_x &&
        m_y < other.m_y + other.m_h &&
        m_y + m_h > other.m_y;
}