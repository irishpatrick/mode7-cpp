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
