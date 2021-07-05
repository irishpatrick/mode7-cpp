#ifndef GEOMETRY_H
#define GEOMETRY_H

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include "line.h"

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
} quad;

void quad_connect(quad*, vec2, vec2, vec2, vec2);
void quad_connect_raw(quad*, float*, float*, float*, float*);
void quad_from_buffer(quad*, float*);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* GEOMETRY_H */
