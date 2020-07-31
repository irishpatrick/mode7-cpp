#ifndef RECT_H
#define RECT_H

#include "line.h"
#include "vec2.h"

typedef struct _Rect
{
    Line a;
    Line b;
    Line c;
    Line d;
} Rect;

Rect rect_create_corners(vec2, vec2, vec2, vec2);
int rect_is_point_inside(Rect*, vec2);
void rect_print(Rect*);

#endif /* RECT_H */