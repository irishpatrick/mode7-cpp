#ifndef LINE_H
#define LINE_H

#include "vec2.h"

typedef struct _Line
{
    vec2 p;
    vec2 e;
    vec2 v;
} Line;

Line line_create(vec2, vec2);
Line line_from_points(vec2, vec2);
vec2 line_normal(Line);
float line_dist_to(Line, vec2);
float line_length(Line);
vec2 line_solve(Line, float);
void line_print(Line*);

#endif /* LINE_H */