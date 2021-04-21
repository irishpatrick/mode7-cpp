#include "vec2.h"
#include "line.h"
#include <stdio.h>
#include <math.h>

vec2 vec2_add(vec2 a, vec2 b)
{
    vec2 c;
    c.x = a.x + b.x;
    c.y = a.y + b.y;
    return c;
}

vec2 vec2_sub(vec2 a, vec2 b)
{
    vec2 c;
    c.x = a.x - b.x;
    c.y = a.y - b.y;
    return c;
}

vec2 vec2_scale(float m, vec2 v)
{
    vec2 out;
    out.x = m * v.x;
    out.y = m * v.y;
    return out;
}

vec2 vec2_rotate(vec2 v, float theta)
{
    vec2 out;
    out.x = v.x * cosf(theta) - v.y * sinf(theta);
    out.y = v.x * sinf(theta) + v.y * cosf(theta);
    return out;
}

float vec2_dot(vec2 a, vec2 b)
{
    return (a.x * b.x) + (a.y * b.y);
}

float vec2_cross(vec2 a, vec2 b)
{
    return (a.x * b.y) - (a.y * b.x);
}

float vec2_mag(vec2 v)
{
    return sqrtf(v.x * v.x + v.y * v.y);
}

void vec2_print(vec2 v)
{
    printf("%f\t%f\n", v.x, v.y);
}

vec2 vec2_norm(vec2 v)
{
    vec2 out;
    float m = vec2_mag(v);
    out.x = v.x / m;
    out.y = v.y / m;
    return out;
}

vec2 vec2_proj(vec2 a, vec2 b)
{
    float a1 = (1.f / vec2_mag(b)) * vec2_dot(a, b);
    return vec2_scale(a1, vec2_norm(b));
}

vec2 vec2_reflect(vec2 v, Line* l)
{
    vec2 vp = vec2_sub(v, l->p);
    float away = line_dist_to(*l, v);
    float along = vec2_dot(vp, l->p);
    return vec2_add(
        vec2_scale(away, line_normal(*l)),
        vec2_scale(along, vec2_norm(l->p))
    );
}