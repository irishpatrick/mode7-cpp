#ifndef GEOMETRY_H
#define GEOMETRY_H

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include "line.h"
#include <math.h>

typedef struct _vec2
{
    float x;
    float y;
} vec2;

typedef struct _quad
{
    vec2 p[4];
    line l[4];
    float length[4];
    float area;
} quad;

int quad_connect(quad*, vec2, vec2, vec2, vec2);
int quad_connect_raw(quad*, float*, float*, float*, float*);
int quad_from_buffer(quad*, float*);
int quad_orient(quad*, int);
void quad_print(quad*);

float vec2_magnitude(vec2*);
void vec2_normalize(vec2*);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* GEOMETRY_H */
