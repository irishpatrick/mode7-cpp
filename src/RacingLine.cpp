#include "RacingLine.hpp"
#include <fstream>
#include <iostream>
#include <cstdio>

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

        sscanf(line.c_str(), "%f,%f,%f,%f", &a, &b, &c, &d);
        Line2D segment;
        segment.fromPoints({a, b}, {c, d});
        m_lines.push_back(segment);

        Line2D top, bot;
        glm::vec2 n = segment.normal();
        bot.create(segment.p(), n);
        top.create(segment.e(), n);
        float w = 10;
        Rect r;
        r.a = top;
        r.b.fromPoints(top.solve(-w), top.solve(w));
        r.c = bot;
        r.d.fromPoints(bot.solve(-w), bot.solve(w));
        m_rects.push_back(r);
    }

    in.close();

    return 0;
}

int RacingLine::getCurrentIndex(glm::vec2 position, int current)
{
    int start = current - 1;
    int index;
    for (int i = start; i < start + 3; ++i)
    {
        index = mod(i, m_rects.size());
        Rect* r = &m_rects[index];

        float da = fabsf(r->a.distTo(position));
        float db = fabsf(r->b.distTo(position));
        float dc = fabsf(r->c.distTo(position));
        float dd = fabsf(r->d.distTo(position));

        float l = r->a.length();
        float w = r->b.length();

        if ((da <= w) && (db <= l) && (dc <= w) && (dd <= l))
        {
            return index;
        }
    }

    return -1;
}