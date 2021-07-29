/**
 * This file is part of mode7-cpp.
 * Copyright (C) 2021  Patrick Roche
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
**/

#include "Util.hpp"

#include <cmath>
#include <ctime>
#include <cstdlib>
#include <iostream>

#ifdef _WIN32
#include <WinUser.h>
#endif /* _WIN32 */
#ifdef __linux__
#include <X11/Xlib.h>
#endif

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

static glm::vec2 project(std::vector<Line2D*>& shape, Line2D* line)
{
    glm::vec2 a;
    glm::vec2 b;
    glm::vec2 origin = line->p();
    glm::vec2 dir = line->v();
    glm::vec2 proj(FLT_MAX, FLT_MIN); // should be fine

    for (auto& e : shape)
    {
        a = e->p() - origin;
        b = e->e() - origin;
        proj.x = fmin(proj.x, glm::dot(a, dir));
        proj.y = fmax(proj.y, glm::dot(b, dir));
    }

    return proj;
}

static inline bool check_overlap(glm::vec2 a, glm::vec2 b)
{
    return
        a.x >= b.x && a.x <= b.y ||
        a.y >= b.x && a.y <= b.y || 
        b.x >= a.x && b.x <= a.y ||
        b.y >= a.x && b.y <= a.y;
}

bool Util::sat(std::vector<Line2D*>& a, std::vector<Line2D*>& b)
{
    bool result = true;
    glm::vec2 axis;
    glm::vec2 proj_a;
    glm::vec2 proj_b;
    Line2D* line;

    for (auto& e : a)
    {
        axis = glm::normalize(e->v());
        line = e;

        proj_a = project(a, e);
        proj_b = project(b, e);

        //std::cout << "A: proj_a=" << glm::to_string(proj_a) << "\tproj_b=" << glm::to_string(proj_b) << " : " << check_overlap(proj_a, proj_b) << std::endl;

        result &= check_overlap(proj_a, proj_b);
    }

    for (auto& e : b)
    {
        axis = glm::normalize(e->v());
        line = e;

        proj_a = project(a, e);
        proj_b = project(b, e);

        //std::cout << "B: proj_a=" << glm::to_string(proj_a) << "\tproj_b=" << glm::to_string(proj_b) << " : " << check_overlap(proj_a, proj_b) << std::endl;

        result &= check_overlap(proj_a, proj_b);
    }

    return result;
}

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

glm::mat4 Util::fromAi(const aiMatrix4x4& from)
{
    return {
        from.a1, from.b1, from.c1, from.d1,
        from.a2, from.b2, from.c2, from.d2,
        from.a3, from.b3, from.c3, from.d3,
        from.a4, from.b4, from.c4, from.d4
    };
}

std::pair<int, int> Util::getMonitorRes()
{
    std::pair<int, int> out(0, 0);
#ifdef _WIN32
    out.first = GetSystemMetrics(SM_CXSCREEN);
    out.second = GetSystemMetrics(SM_CYSCREEN);
#elif defined __linux__
    Display* d = XOpenDisplay(NULL);
    if (!d)
    {
        return out;
    }
    Screen* s = DefaultScreenOfDisplay(d);
    if (!s)
    {
        return out;
    }
    out.first = s->width;
    out.second = s->height;
#endif

    return out;
}

int Util::getMonitorScale()
{
    int h;
#ifdef _WIN32
    h = GetSystemMetrics(SM_CYSCREEN);
#elif defined __linux__
    Display* d = XOpenDisplay(NULL);
    Screen* s = DefaultScreenOfDisplay(d);
    h = s->height;
#endif

    int sc = h / 1080;
    if (sc < 1)
    {
        sc = 1;
    }
    return sc;
}