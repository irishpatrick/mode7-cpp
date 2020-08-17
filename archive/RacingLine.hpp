#ifndef RACINGLINE_HPP
#define RACINGLINE_HPP

#include <string>
#include <vector>
#include <cstdint>
#include "gl.h"
#include "BBox.hpp"

class Point
{
public:

    Point();
    ~Point();

    glm::vec2 pos;
    uint32_t mode;

    static const uint32_t ACCEL_PT = 0;
    static const uint32_t COAST_PT = 1;
    static const uint32_t BRAKE_PT = 2;
private:

};

class Rect
{
public:

    Rect();
    ~Rect();

    void create(glm::vec2, glm::vec2);
    bool check(glm::vec2);
    bool check(Rect);

private:

    glm::vec2 pos;
    glm::vec2 dim;
};

class RacingLine
{
public:

    RacingLine();
    ~RacingLine();

    void open(const std::string&);
    void update(glm::vec3);
    glm::vec2 getTarget();
    uint32_t getAction();

private:

    uint32_t increment();

    uint32_t current;
    std::vector<Point> points;
    std::vector<BBox> check;
};

#endif /* RACINGLINE_HPP */