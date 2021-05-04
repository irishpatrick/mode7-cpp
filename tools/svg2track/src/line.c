#include "line.h"

#include <stdio.h>
#include <math.h>

void line_connect(line* l, float x1, float y1, float x2, float y2)
{
    l->p1[0] = x1;
    l->p1[1] = y1;
    l->p2[0] = x2;
    l->p2[1] = y2;
}

void line_solve(line* l, float t, float out[2])
{
    out[0] = (1.f - t) * l->p1[0] + t * l->p2[0];
    out[1] = (1.f - t) * l->p1[1] + t * l->p2[1];
}

float line_calc_distance(line* l)
{
    float a[2];
    float b[2];
    float d[2];

    line_solve(l, 0.0f, a);
    line_solve(l, 1.0f, b);
    d[0] = a[0] - b[0];
    d[1] = a[1] - b[1];

    return sqrtf(d[0] * d[0] + d[1] * d[1]);
}

void line_print(line* l)
{
    printf("line: {(%f,%f) --> (%f,%f)}\n", 
        l->p1[0], 
        l->p1[1], 
        l->p2[0], 
        l->p2[1]);
}