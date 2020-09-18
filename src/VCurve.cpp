#include "VCurve.hpp"
#include <iostream>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <utility>

namespace mode7
{

VCurve::VCurve()
{
}

VCurve::~VCurve()
{
}

void VCurve::open(const std::string& fn)
{
    std::ifstream in(fn);
    if (!in)
    {
        return;
    }
    std::string line;
    std::vector<std::string> parts;
    uint32_t px = 0;
    uint32_t py = 0;
    while (std::getline(in, line))
    {
        boost::split(parts, line, boost::is_any_of(","));
        if (parts.size() < 2)
        {
            std::cout << "bad line" << std::endl;
        }
        else
        {
            Line l;
            Point p;
            uint32_t x, y;
            x = boost::lexical_cast<uint32_t>(parts[0]);
            y = boost::lexical_cast<uint32_t>(parts[1]);
            l.create(px, py, x, y);
            p.x = px;
            p.y = py;
            m_segments.push_back(std::pair<Point, Line>(p, l));
        }
    }
}

float VCurve::getY(float x)
{
    Point* p;
    Line* l;
    for (auto& e : m_segments)
    {
        p = &e.first;
        l = &e.second;

        if (x >= (float)p->x)
        {
            return l->solve(x);
        }
    }

    return 0.f;
}

float VCurve::getX(float y)
{
    Point* p;
    Line* l;
    for(auto& e : m_segments)
    {
        p = &e.first;
        l = &e.second;

        if (y >= (float)p->y)
        {
            return l->solveInv(y);
        }
    }

    return 0.f;
}

}
