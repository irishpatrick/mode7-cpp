#ifndef PART_HPP
#define PART_HPP

#include "vec2.hpp"
#include <vector>
#include <string>

class Part
{
public:

    Part();
    ~Part();

    void open(const std::string&);
    const std::vector<vec2>& getPoints();

private:

    std::vector<vec2> m_points;
};

#endif /* PART_HPP */
