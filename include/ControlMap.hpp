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

#ifndef CONTROLMAP_HPP
#define CONTROLMAP_HPP

#include <string>
#include <vector>

namespace mode7
{

class ControlMap
{
public:
    ControlMap();
    ~ControlMap();

    void open(const std::string&);
    float calculate(float, float);

private:
    int to_index(int, int);

    float m_width;
    float m_height;
    int m_nrows;
    int m_ncols;
    std::vector<float> m_map;
};

}

#endif /* CONTROLMAP_HPP */