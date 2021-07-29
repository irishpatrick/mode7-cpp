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

#include "bezier.h"

#include "line.h"

#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <math.h>

int bezier_init(bezier* b, int deg)
{
    b->control = malloc(2 * (deg - 1) * sizeof(float));
    if (!b->control)
    {
        return 1;
    }

    return 0;
}

void bezier_set_endpt(bezier* b, int indx, float x, float y)
{
    switch (indx)
    {
        case 0:
        b->p1[0] = x;
        b->p1[1] = y;
        break;

        case 1:
        b->p2[0] = x;
        b->p2[1] = y;
        break;
    }
}

void bezier_set_ctrlpt(bezier* b, int indx, float x, float y)
{
    assert(b->control != NULL);
    
    float* pt = &b->control[indx * 2];
    pt[0] = x;
    pt[1] = y;
}

void bezier_quadratic(bezier* b, float t, float out[2])
{
    float* c1 = b->control + 0;

    float minus = 1.f - t;
    out[0] = c1[0] + 
            (minus * minus) * (b->p1[0] - c1[0]) +
            (t * t) * (b->p2[0] - c1[0]);

    out[1] = c1[1] + 
            (minus * minus) * (b->p1[1] - c1[1]) +
            (t * t) * (b->p2[1] - c1[1]);
}

void bezier_cubic(bezier* b, float t, float out[2])
{
    float* c1 = b->control + 0;
    float* c2 = b->control + 2;

    float minus = 1.f - t;
    out[0] = (minus * minus * minus) * b->p1[0] + 
            (3 * minus * minus * t) * c1[0] + 
            (3 * minus * t * t) * c2[0] +
            (t * t * t) * b->p2[0];

    out[1] = (minus * minus * minus) * b->p1[1] + 
            (3 * minus * minus * t) * c1[1] + 
            (3 * minus * t * t) * c2[1] +
            (t * t * t) * b->p2[1];
}

float bezier_estimate_distance(bezier* bz, float stepsize)
{
    uint32_t n_steps = 1.0f / stepsize;
    line seg;
    float a[2];
    float b[2];
    float tc;
    float tn;
    float sum = 0;

    for (int i = 0 ; i <= n_steps; ++i)
    {
        tc = stepsize * i;
        tn = stepsize * (i + 1);
       
        bezier_cubic(bz, tc, a);
        bezier_cubic(bz, tn, b); 
        line_connect(&seg, a[0], a[1], b[0], b[1]);
        sum += line_calc_distance(&seg);
    }

    return sum;
}

void bezier_destroy(bezier* b)
{
    free(b->control);
}