#ifndef VCURVE_HPP
#define VCURVE_HPP

#include <string>
#include <vector>
#include "Line.hpp"

namespace mode7
{

typedef struct _Point
{
    uint32_t x;
    uint32_t y;
} Point;

class ResponseCurve
{
public:

    ResponseCurve();
    ~ResponseCurve();

    void open(const std::string&);
    float getX(float);
    float getY(float);

private:

    std::vector<std::pair<Point, Line>> m_segments;
};

}

#endif /* VCURVE_HPP */
