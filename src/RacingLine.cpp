#include "RacingLine.hpp"
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

Point::Point() :
    pos(0.f),
    mode(ACCEL_PT)
{

}

Point::~Point()
{

}

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
    this->pos = pos - (0.5f * dim);
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
    //std::cout << "[" << glm::to_string(pos) << " | " << glm::to_string(dim) << "]" << std::endl;
    //std::cout << "[" << glm::to_string(r.pos) << " | " << glm::to_string(r.dim) << "]" << std::endl;
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
    for (auto& e : o["points"])
    {
        float x = e[0].get<float>();
        float y = e[1].get<float>();
        std::string mode = e[2].get<std::string>();
        
        Point p;
        p.pos = glm::vec2(x, y);
        
        if (mode == "accel")
        {
            p.mode = Point::ACCEL_PT;
        }
        else if (mode == "coast")
        {
            p.mode = Point::COAST_PT;
        }
        else if (mode == "brake")
        {
            p.mode = Point::BRAKE_PT;
        }

        points.push_back(p);
    }

    for (int i = 0; i < points.size(); ++i)
    {
        BBox b;
        glm::vec3 pt(points[i].pos.x, 0, points[i].pos.y);
        b.pos = pt;
        b.dim = glm::vec3(40.0f, 10.0f, 40.0f);
        check.push_back(b);
    }
}

void RacingLine::update(glm::vec3 position)
{
    //glm::vec2 dir = glm::normalize(points[increment()] - points[current]);
    if (check[current].intersects(position))
    {
        current = increment();
    }
}

glm::vec2 RacingLine::getTarget()
{
    return points[current].pos;
}

uint32_t RacingLine::getAction()
{
    return points[current].mode;
}

uint32_t RacingLine::increment()
{
    return (current + 1) % (uint32_t)points.size();
}