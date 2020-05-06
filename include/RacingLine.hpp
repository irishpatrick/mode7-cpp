#ifndef RACINGLINE_HPP
#define RACINGLINE_HPP

#include <string>
#include <vector>
#include <cstdint>
#include "gl.h"

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
    void update(glm::vec2);
    glm::vec2 getTarget();

private:

    uint32_t increment();

    uint32_t current;
    std::vector<glm::vec2> point;
    std::vector<Rect> check;
};

#endif /* RACINGLINE_HPP */