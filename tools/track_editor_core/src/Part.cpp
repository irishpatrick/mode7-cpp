#include "Part.hpp"
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

Part::Part()
{

}

Part::~Part()
{

}

void Part::open(const std::string& fn)
{
    std::ifstream in(fn);
    if (!in)
    {
        return;
    }

    json o;
    in >> o;
    in.close();

    for (auto& e : o["points"])
    {
        m_points.push_back({e[0], e[1]});
    }
}

const std::vector<vec2>& Part::getPoints()
{
    return m_points;
}
