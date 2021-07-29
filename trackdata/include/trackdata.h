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

#ifndef TRACKDATA_H
#define TRACKDATA_H

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include "line.h"
#include "geometry.h"

#include <stdint.h>

typedef struct _trackdata
{
    uint32_t n_segments;
    uint32_t max_segments;
    float width;

    uint32_t clp;
    uint32_t tbp;
    uint32_t rbp;
    uint32_t wbp;

    uint32_t clmax;
    uint32_t tbmax;
    uint32_t rbmax;
    uint32_t wbmax;
    
    line* centerline;
    quad* track_bounds;
    quad* runoff_bounds;
    quad* walls_bounds;
} trackdata;

void trackdata_init(trackdata*);
void trackdata_save(trackdata*, const char*);
void trackdata_free(trackdata*);

void trackdata_centerline(trackdata*, line*);
void trackdata_track_bounds(trackdata*, float*, float*, float*, float*);
void trackdata_runoff_bounds(trackdata*, float);
void trackdata_walls_bounds(trackdata*, float);
void trackdata_push_all(trackdata*);
void trackdata_push_clp(trackdata*);
void trackdata_push_tbp(trackdata*);
void trackdata_push_rbp(trackdata*);
void trackdata_push_wbp(trackdata*);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* TRACKDATA_H */
