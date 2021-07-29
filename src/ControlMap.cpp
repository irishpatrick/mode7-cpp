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

#include "ControlMap.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdint>
#include <cmath>
#include <boost/algorithm/string.hpp>

namespace mode7
{

ControlMap::ControlMap() :
    m_width(1.0f),
    m_height(1.0f),
    m_nrows(11),
    m_ncols(11)
{

}

ControlMap::~ControlMap()
{

}

void ControlMap::open(const std::string& fn)
{
    std::cout << "[ControlMap] open " << fn << std::endl;
    // csv format
    std::ifstream in(fn, std::ios::binary);
    if (!in)
    {
        std::cout << "[ControlMap] failed to open " << fn << std::endl;
        return;
    }

    std::string line;
    std::vector<std::string> cols;
    int line_num = 0;
    while (std::getline(in, line))
    {
        boost::split(cols, line, boost::is_any_of(","));
        
        if (line_num == 0)
        {
        }
        else
        {
            m_ncols = cols.size() - 1;
            for (uint32_t i = 1; i < cols.size(); ++i)
            {
                m_map.push_back(atof(cols[i].c_str()));
            }
        }

        if (cols.size() > 1) ++line_num;
    }

    m_nrows = line_num - 1;

    assert(m_ncols > 0);
    assert(m_nrows > 0);
    assert(m_map.size() > 0);
}

float ControlMap::calculate(float x, float y)
{
    if (m_map.size() <= 0)
    {
        return 0.0f;
    }

    float window_x = m_width / (float)(m_ncols - 1);
    float window_y = m_height / (float)(m_nrows - 1);
    unsigned int wx = x / window_x;
    unsigned int wy = y / window_y;

    wx = (int)fmin(wx, m_ncols - 1);
    wy = (int)fmin(wy, m_nrows - 1);

    float x1 = (wx + 0) * window_x;
    float y1 = (wy + 0) * window_y;
    float x2 = (wx + 1) * window_x;
    float y2 = (wy + 1) * window_y;

    int ax = (int)fmin(wx + 0, m_ncols - 1);
    int ay = (int)fmin(wy + 0, m_nrows - 1);
    int bx = (int)fmin(wx + 1, m_ncols - 1);
    int by = (int)fmin(wy + 1, m_nrows - 1);

    float q11 = m_map[to_index(ax, ay)];
    float q12 = m_map[to_index(ax, by)];
    float q21 = m_map[to_index(bx, ay)];
    float q22 = m_map[to_index(bx, by)];

    float fxy1 = (x2 - x) / (x2 - x1) * q11 + (x - x1) / (x2 - x1) * q21;
    float fxy2 = (x2 - x) / (x2 - x1) * q12 + (x - x1) / (x2 - x1) * q22;

    return (y2 - y) / (y2 - y1) * fxy1 + (y - y1) / (y2 - y1) * fxy2;   
}

int ControlMap::to_index(int x, int y)
{
    return x + m_ncols * y;
}

} // namespace mode7