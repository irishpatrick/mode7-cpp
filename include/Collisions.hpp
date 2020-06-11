#ifndef COLLISIONS_HPP
#define COLLISIONS_HPP

#include <vector>
#include "Object.hpp"
#include "gl.h"

class Collisions
{
public:

    Collisions();
    ~Collisions();

    void addObject(Object*, float);
    void update();

private:

    std::vector<std::pair<Object*, float>> m_objs;
};

#endif /* COLLISIONS_HPP */
