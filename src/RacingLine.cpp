#include "RacingLine.hpp"
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

Rect::Rect() :
    pos(0),
    dim(1)
{

}

Rect::~Rect()
{

}

void Rect::create(glm::vec2 pos, glm::vec2 dim)
{
    this->pos = pos;
    this->dim = dim;
}

bool Rect::check(glm::vec2 pt)
{
    Rect r;
    r.create(pt, glm::vec2(1, 1));
    return check(r);
}

bool Rect::check(Rect r)
{
    return
        pos.x < r.pos.x + r.dim.x &&
        pos.x + dim.x > r.pos.x &&
        pos.y < r.pos.y + r.dim.y &&
        pos.y + dim.y > r.pos.y ;
}

RacingLine::RacingLine() :
    current(0)
{

}

RacingLine::~RacingLine()
{

}

void RacingLine::open(const std::string& fn)
{
    // open
    std::ifstream in(fn);
    if (!in)
    {
        std::cout << "cannot open " << fn << std::endl;
        return;
    }

    json o;
    o << in;
    in.close();

    // parse
    uint32_t width = 100;

    for (auto& e : o["points"])
    {
        float x = e[0].get<float>();
        float y = e[1].get<float>();

        point.push_back(glm::vec2(x, y));
    }

    for (int i = 0; i < point.size() - 1; ++i)
    {
        Rect r;
        r.create(point[i], glm::vec2(10, 2));
        check.push_back(r);
    }
}

void RacingLine::update(glm::vec2 position)
{
    //glm::vec2 dir = glm::normalize(point[increment()] - point[current]);
    if (check[current].check(position))
    {
        current = increment();
    }
}

glm::vec2 RacingLine::getTarget()
{
    return point[current];
}

uint32_t RacingLine::increment()
{
    return (current + 1) % (uint32_t)point.size();
}