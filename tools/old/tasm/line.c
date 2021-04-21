#include "line.h"
#include <stdio.h>

Line line_create(vec2 p, vec2 v)
{
    Line out;
    out.p = p;
    out.e = p;
    out.v = v;
    return out;
}

Line line_from_points(vec2 a, vec2 b)
{
    Line out;
    out.v = vec2_sub(b, a);
    out.p = a;
    out.e = b;
    return out;
}

vec2 line_normal(Line ln)
{
    vec2 out;
    out.x = -ln.v.y;
    out.y = ln.v.x;
    return vec2_norm(out);
}

vec2 line_solve(Line ln, float t)
{
    vec2 out;
    out = vec2_add(ln.p, vec2_scale(t, ln.v));
    return out;
}

float line_dist_to(Line ln, vec2 pt)
{
    vec2 q = vec2_sub(pt, ln.p);
    return vec2_dot(q, line_normal(ln));
}

float line_length(Line ln)
{
    return vec2_mag(vec2_sub(ln.p, ln.e));
}

void line_print(Line* ln)
{
    printf("[%.4f, %.4f, %.4f, %.4f]\n", ln->p.x, ln->p.y, ln->e.x, ln->e.y);
}