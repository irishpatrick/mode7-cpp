#include "rect.h"
#include <string.h>
#include <stdio.h>
#include <math.h>

Rect rect_create_corners(vec2 a, vec2 b, vec2 c, vec2 d)
{
    Rect out;
    out.a = line_from_points(a, b);
    out.b = line_from_points(b, c);
    out.c = line_from_points(c, d);
    out.d = line_from_points(d, a);
    return out;
}

int rect_is_point_inside(Rect* r, vec2 pt)
{
    float da = fabsf(line_dist_to(r->a, pt));
    float db = fabsf(line_dist_to(r->b, pt));
    float dc = fabsf(line_dist_to(r->c, pt));
    float dd = fabsf(line_dist_to(r->d, pt));

    float l = line_length(r->a);
    float w = line_length(r->b);

    //printf("%f,%f,%f,%f,%f,%f\n", da,db,dc,dd,l,w);

    return (da <= w) && (db <= l) && (dc <= w) && (dd <= l);
}

void rect_print(Rect* r)
{
    printf("Rectangle\n");
    line_print(&r->a);
    line_print(&r->b);
    line_print(&r->c);
    line_print(&r->d);
}