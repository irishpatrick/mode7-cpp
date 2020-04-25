#ifndef UTIL_HPP
#define UTIL_HPP

#include "gl.h"

class Util
{
public:

    static void seed();
    static int randint(int, int);
    static glm::vec3 xAxis();
    static glm::vec3 yAxis();
    static glm::vec3 zAxis();
    static float lerp(float, float, float);
    static float map(float, float, float, float, float);

private:


};

#endif /* UTIL_HPP */
