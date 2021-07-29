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

#ifndef TRACK_H
#define TRACK_H

#include "line.h"
#include "bezier.h"
#include "mesh.h"
#include "trackdata.h"

typedef enum {SEG_LINE = 0, SEG_BEZIER = 1} segment_t;

typedef struct _track
{
    int n_segments;
    int n_lines;
    int n_beziers;
    int max_segments;
    segment_t* lookup;
    line** lines;
    bezier** beziers;
    trackdata tdata;
} track;

void track_init(track*, int);
void track_destroy(track*);

void track_add_bezier(track*, bezier*);
void track_add_line(track*, line*);

void track_meshify(track*, mesh*, const char*, float, float, float);

#endif /* TRACK_H */

