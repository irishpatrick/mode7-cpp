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

void line_from_buffer(line* l, float* buf)
{
    line_connect(l, buf[0], buf[1], buf[2], buf[3]);
}

void line_solve(line* l, float t, float out[2])
{
    out[0] = (1.f - t) * l->p1[0] + t * l->p2[0];
    out[1] = (1.f - t) * l->p1[1] + t * l->p2[1];
}

float line_solve_y(line* l, float x)
{
    return line_slope(l) * x + line_intercept(l);
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

float line_slope(line* l)
{
    return (l->p2[1] - l->p1[1]) / (l->p2[0] - l->p1[0]);
}

float line_intercept(line* l)
{
    return l->p1[1] - line_slope(l) * l->p1[0];
}

float line_inverse(line* l, float x, float y)
{

}

float line_length(line* l)
{
    float dx = l->p2[0] - l->p1[0];
    float dy = l->p2[1] - l->p1[1];
    return sqrt(dx * dx + dy * dy);
}

bool line_intersects(line* l1, line* l2)
{
    line_print(l1);
    printf("intersects\n");
    line_print(l2);

    float dm = line_slope(l1) - line_slope(l2);
    if (dm == 0.0)
    {
        return false;
    }

    float db = line_intercept(l2) - line_intercept(l1);

    float xint = db / dm;
    printf("xint %f\n", xint);
    //float yint = line_solve_y(l1, xint);

    float mag = line_length(l1);
    float xdir = (l1->p2[0] - l1->p1[0]);
    printf("xdir %f\n", xdir);

    float t_int = (xint - l1->p1[0]) / xdir;
    
    //if (t_int >= 0.0 && t_int <= 1.0)
    {
        printf("%f\n", t_int);
    }

    return t_int >= 0.002 && t_int <= 1.0 - .002;
}