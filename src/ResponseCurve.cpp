#include "ResponseCurve.hpp"
#include <iostream>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <utility>
#include <iostream>

namespace mode7
{

ResponseCurve::ResponseCurve()
{
}

ResponseCurve::~ResponseCurve()
{
}

void ResponseCurve::open(const std::string& fn)
{
    std::ifstream in(fn);
    if (!in)
    {
        std::cout << "cannot open " << fn << std::endl;
        return;
    }
    std::string line;
    std::vector<std::string> parts;
    int32_t px = 0;
    int32_t py = 0;
    bool first = true;
    Line l;

    while (std::getline(in, line))
    {
        boost::split(parts, line, boost::is_any_of(","));
        if (parts.size() < 2)
        {
            std::cout << "bad line" << std::endl;
        }
        else
        {
            int32_t x, y;
            x = boost::lexical_cast<int32_t>(parts[0]);
            y = boost::lexical_cast<int32_t>(parts[1]);
            if (x == px)
            {
                px = x;
                py = y;
                if (first) continue;
            }
            l.create(px, py, x, y);
            m_segments.push_back(l);
            first = false;
            px = x;
            py = y;
        }
    }
}

float ResponseCurve::getY(float x)
{
    Line* l;
    for (int i = m_segments.size() - 1; i >= 0; --i)
    {
        l = &m_segments[i];

        //if (x >= (float)p->x)
        if (x >= l->x0 && x <= l->x1)
        {
            return l->solve(x);
        }
    }

    return 0.f;
}

float ResponseCurve::getX(float y)
{
    Line* l;
    for (int i = m_segments.size() - 1; i >= 0; --i)
    {
        l = &m_segments[i];

        if (y >= l->y0 && y <= l->y1)
        {
            return l->solveInv(y);
        }
    }

    return 0.f;
}

}
