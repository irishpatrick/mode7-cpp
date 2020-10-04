#ifndef UTIL_HPP
#define UTIL_HPP

#include <assimp/Importer.hpp>
#include "gl.h"

#ifdef __linux__
#include <X11/Xlib.h>
#endif

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
    static glm::mat4 fromAi(const aiMatrix4x4&);
    static std::pair<int, int> getMonitorRes();

    static inline float constrain(float val, float min, float max)
    {
        if (val <= min) val = min;
        if (val >= max) val = max;
        return val;
    }

private:


};

#endif /* UTIL_HPP */
