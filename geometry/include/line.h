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

#ifndef LINE_H
#define LINE_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

typedef struct _line
{
    float p1[2];
    float p2[2];
} line;

void line_connect(line*, float, float, float, float);
void line_from_buffer(line*, float*);
void line_solve(line*, float, float[2]);
float line_inverse(line*, float, float);
float line_solve_y(line*, float);
float line_calc_distance(line*);
void line_print(line*);
bool line_intersects(line*, line*);
float line_slope(line*);
float line_intercept(line*);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* LINE_H */
