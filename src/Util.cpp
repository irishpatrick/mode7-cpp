#include "Util.hpp"
#include <cmath>
#include <ctime>
#include <cstdlib>

static glm::vec3 ax = { 1, 0, 0 };
static glm::vec3 ay = { 0, 1, 0 };
static glm::vec3 az = { 0, 0, 1 };

void Util::seed()
{
    srand(time(NULL));
}

/*void Util::toQuat(float* v, float* r)
{
    float cx = cosf(r[0] / 2.0f);
    float sx = sinf(r[0] / 2.0f);
    float cy = cosf(r[1] / 2.0f);
    float sy = sinf(r[1] / 2.0f);
    float cz = cosf(r[2] / 2.0f);
    float sz = sinf(r[2] / 2.0f);

    float w = cx * cy * cz + sx * sy * sz;
    float x = sx * cy * cz - cx * sy * sz;
    float y = cx * sy * cz + sx * cy * sz;
    float z = cx * cy * sz - sx * sy * cz;

    v[3] = w;
    v[0] = x;
    v[1] = y;
    v[2] = z;
}*/

float Util::lerp(float v0, float v1, float t)
{
    return (1 - t) * v0 + t * v1;
}

float Util::map(float x, float a0, float a1, float b0, float b1)
{
    return ((x - a0) / (a1 - a0)) * (b1 - b0) + b0;
}

glm::vec3 Util::xAxis()
{
    return ax;
}

glm::vec3 Util::yAxis()
{
    return ay;
}

glm::vec3 Util::zAxis()
{
    return az;
}
