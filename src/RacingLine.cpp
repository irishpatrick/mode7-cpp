#include "RacingLine.hpp"
#include <fstream>
#include <iostream>
#include <cstdio>
#include <cassert>

RacingLine::RacingLine()
{

}

RacingLine::~RacingLine()
{

}

int RacingLine::load(const std::string& fn)
{
    std::ifstream in(fn);
    if (!in)
    {
        return 1;
    }

    std::string line;
    float a, b, c, d;
    int lineNum = 0;
    while(std::getline(in, line))
    {
        ++lineNum;
        if (lineNum == 1) // skip first line
        {
            continue;
        }

        if (line[0] == '#')
        {
            continue;
        }

        sscanf(line.c_str(), "%f,%f,%f,%f", &a, &b, &c, &d);

        Line2D segment;
        segment.fromPoints(glm::vec2(a, b), glm::vec2(c, d));
        assert(segment.v().x != 0.f || segment.v().y != 0.f);
        m_lines.push_back(segment);

        Line2D top, bot;
        glm::vec2 n = segment.normal();
        assert(n.x != 0.f || n.y != 0.f);
        bot.create(segment.p(), n);
        top.create(segment.e(), n);
        float w = 30;
        glm::vec2 a, b, c, d;
        a = top.solve(w);
        b = top.solve(-w);
        c = bot.solve(-w);
        d = bot.solve(w);

        Rect r;
        r.a.fromPoints(a, b);
        r.b.fromPoints(b, c);
        r.c.fromPoints(c, d);
        r.d.fromPoints(d, a);

        m_rects.push_back(r);
    }

    in.close();

    return 0;
}

int RacingLine::getCurrentIndex(glm::vec2 position, int current)
{
    int start = current - 1;
    int index;
    //for (int i = start; i < start + 3; ++i)
    for (int i = 0; i < m_rects.size(); ++i)
    {
        index = mod(i, m_rects.size());
        Rect* r = &m_rects[index];

        float da = fabsf(r->a.distTo(position));
        float db = fabsf(r->b.distTo(position));
        float dc = fabsf(r->c.distTo(position));
        float dd = fabsf(r->d.distTo(position));

        //std::cout << "bounds(" << index << "): " << da << "," << db << "," << dc << "," << dd << std::endl;

        float l = r->a.length();
        float w = r->b.length();

        //std::cout << l << "\t" << w << std::endl;

        if ((da <= w) && (db <= l) && (dc <= w) && (dd <= l))
        {
            return index;
        }
    }

    return -1;
}