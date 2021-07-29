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

#ifndef BEZIER_H
#define BEZIER_H

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

typedef struct _bezier
{
    float p1[2];
    float p2[2];
    float* control;
} bezier;

int bezier_init(bezier*, int);
void bezier_set_endpt(bezier*, int, float, float);
void bezier_set_ctrlpt(bezier*, int, float, float);
void bezier_quadratic(bezier* b, float t, float out[2]);
void bezier_cubic(bezier*, float, float[2]);
float bezier_estimate_distance(bezier*, float);
void bezier_destroy(bezier*);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* BEZIER_H */