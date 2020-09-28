#include "RacingLine.hpp"
#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstdint>
#include <cassert>

namespace mode7
{

RacingLine::RacingLine() :
    m_queuedAction(0)
{

}

RacingLine::~RacingLine()
{

}

int RacingLine::load(const std::string& fn)
{
    m_path.init();
    std::vector<glm::vec2> points;

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
            //continue;
        }

        if (line[0] == '#')
        {
            continue;
        }

        if (line[0] == '!')
        {
            size_t loc = line.find(" ");
            sscanf(line.substr(loc + 1).c_str(), "%d", &m_queuedAction);
            continue;
        }

        sscanf(line.c_str(), "%f,%f,%f,%f", &a, &b, &c, &d);
        
        points.push_back({a, b});
        points.push_back({c, d});

        Line2D segment;
        segment.fromPoints(glm::vec2(a, b), glm::vec2(c, d));
        assert(segment.v().x != 0.f || segment.v().y != 0.f);
        m_lines.push_back(std::pair<Line2D, int>(segment, m_queuedAction));
        m_queuedAction = 0;

        Line2D top, bot;
        glm::vec2 n = segment.normal();
        assert(n.x != 0.f || n.y != 0.f);
        bot.create(segment.p(), n);
        top.create(segment.e(), n);
        float w = 20;
        glm::vec2 a, b, c, d;
        a = top.solve(w);   // top left
        b = top.solve(-w);  // top right
        c = bot.solve(-w);  // bottom right
        d = bot.solve(w);   // bottom left

        Rect r;
        r.a.fromPoints(a, b);
        r.b.fromPoints(b, c);
        r.c.fromPoints(c, d);
        r.d.fromPoints(d, a);

        //float templ = r.b.length();
        m_rects.push_back(r);
        
        w = 10.f;
        a = top.solve(w);   // top left
        b = top.solve(-w);  // top right
        c = bot.solve(-w);  // bottom right
        d = bot.solve(w);   // bottom left

        glm::vec2 h = glm::normalize(b - c);
        float bias = 10.f;
        a += bias * h;
        b += bias * h;
        c += -bias * h;
        d += -bias * h;

        r.a.fromPoints(a, b);
        r.b.fromPoints(b, c);
        r.c.fromPoints(c, d);
        r.d.fromPoints(d, a);
        m_bounds.push_back(r);
    }

    points.push_back(points.back());
    points.push_back(points[0]);
    m_path.createFromPoints(points);
    m_path.position.y = 0.5f;
    //m_path.position.x = -325.f;
    m_path.update();

    in.close();

    return 0;
}

int RacingLine::getCurrentIndex(glm::vec2 position, int current)
{
    //int start = current - 1;
    int index;
    //for (int i = start; i < start + 3; ++i)
    for (uint32_t i = 0; i < m_rects.size(); ++i)
    {
        index = mod(i, m_rects.size());
        Rect* r = &m_rects[index];

        float da = fabsf(r->a.distTo(position));// - 325.f / 2.f;
        float db = fabsf(r->b.distTo(position));
        float dc = fabsf(r->c.distTo(position));// - 325.f / 2.f;
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

}
