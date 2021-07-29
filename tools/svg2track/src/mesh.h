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

#ifndef MESH_H
#define MESH_H

#include <stdint.h>

typedef float vertex[3];
typedef float color[3];
typedef float uv[2];
typedef uint32_t idx[3];

typedef struct _mesh
{
    float* vertices;
    float* normals;
    float* colors;
    float* uvs;
    uint32_t* indices;

    uint32_t n_vertices;
    uint32_t n_normals;
    uint32_t n_indices;
    uint32_t n_uvs;
    uint32_t max_vertices;
    uint32_t max_normals;
    uint32_t max_uvs;
    uint32_t max_indices;

    float length[3];
} mesh;

void mesh_init(mesh*);
void mesh_begin_building(mesh*);
void mesh_finish_building(mesh*);
void mesh_copy(mesh*, mesh*);
void mesh_destroy(mesh*);
void mesh_add_vertex(mesh*, vertex);
void mesh_add_color(mesh*, color);
void mesh_add_normal(mesh*, vertex);
void mesh_add_uv(mesh*, float*);
void mesh_add_index(mesh*, uint32_t, uint32_t, uint32_t);
void mesh_connect(mesh*);
void mesh_compute_bbox(mesh*);

#endif /* MESH_H */