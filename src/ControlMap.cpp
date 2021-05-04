#include "ControlMap.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdint>
#include <boost/algorithm/string.hpp>

namespace mode7
{

ControlMap::ControlMap()
{

}

ControlMap::~ControlMap()
{

}

void ControlMap::open(const std::string& fn)
{
    // csv format
    std::ifstream in(fn, std::ios::binary);
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
}

float ControlMap::calculate(float x, float y)
{
    float window_x = m_width / (float)m_ncols;
    float window_y = m_height / (float)m_nrows;
    int wx = x / window_x;
    int wy = y / window_y;

    float x1 = (wx + 0) * window_x;
    float y1 = (wy + 0) * window_y;
    float x2 = (wx + 1) * window_x;
    float y2 = (wy + 1) * window_y;

    float q11 = m_map[to_index(wx + 0, wy + 0)];
    float q12 = m_map[to_index(wx + 0, wy + 1)];
    float q21 = m_map[to_index(wx + 1, wy + 0)];
    float q22 = m_map[to_index(wx + 1, wy + 1)];

    float fxy1 = (x2 - x) / (x2 - x1) * q11 + (x - x1) / (x2 - x1) * q21;
    float fxy2 = (x2 - x) / (x2 - x1) * q12 + (x - x1) / (x2 - x1) * q22;

    return (y2 - y) / (y2 - y1) * fxy1 + (y - y1) / (y2 - y1) * fxy2;   
}

int ControlMap::to_index(int x, int y)
{
    return x + m_ncols * y;
}

} // namespace mode7