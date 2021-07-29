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

#include "mesh.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <string.h>

void mesh_init(mesh* m)
{
    memset(m, 0, sizeof(mesh));
}

void mesh_begin_building(mesh* m)
{
    m->max_vertices = 100;
    m->n_vertices = 0;
    m->vertices = malloc(m->max_vertices * sizeof(vertex));
    if (!m->vertices)
    {
        printf("out of memory!\n");
        return;
    }

    m->max_normals = 100;
    m->n_normals = 0;
    m->normals = malloc(m->max_normals * sizeof(vertex));
    if (!m->normals)
    {
        printf("out of memory!\n");
        free(m->vertices);
        return;
    }

    m->max_uvs = 100;
    m->n_uvs = 0;
    m->uvs = malloc(m->max_uvs * sizeof(uv));
    if (!m->uvs)
    {
        printf("out of memory!\n");
        free(m->normals);
        free(m->vertices);
        return;
    }
    
    m->max_indices = 100;
    m->n_indices = 0;
    m->indices = malloc(m->max_indices * sizeof(idx));
    if (!m->indices)
    {
        printf("out of memory!\n");
        free(m->normals);
        free(m->vertices);
        free(m->uvs);
        return;
    }
}

void mesh_finish_building(mesh* m)
{
    // reclaim memory
    float* newf;
    uint32_t* newu;

    newf = realloc(m->vertices, m->n_vertices * sizeof(vertex));
    if (newf == NULL)
    {
        printf("out of memory!\n");
        return;
    }
    m->vertices = newf;

    newf = realloc(m->normals, m->n_normals * sizeof(vertex));
    if (newf == NULL)
    {
        printf("out of memory!\n");
        return;
    }
    m->normals = newf;

    newf = realloc(m->uvs, m->n_uvs * sizeof(uv));
    if (newf == NULL)
    {
        printf("out of memory!\n");
        return;
    }
    m->uvs = newf;

    newu = realloc(m->indices, m->n_indices * sizeof(idx));
    if (!newu)
    {
        return;
    }
    m->indices = newu;

    mesh_compute_bbox(m);
}

void mesh_copy(mesh* dest, mesh* src)
{
    dest->n_vertices = src->n_vertices;
    dest->max_vertices = src->max_vertices;
    dest->vertices = malloc(dest->n_vertices * sizeof(vertex));
    if (!dest->vertices)
    {
        return;
    }
    memcpy(dest->vertices, src->vertices, dest->n_vertices * sizeof(vertex));
    memcpy(dest->length, src->length, 3 * sizeof(float));

    dest->n_normals = src->n_normals;
    dest->max_normals = src->max_normals;
    dest->normals = malloc(dest->n_normals * sizeof(vertex));
    if (!dest->normals)
    {
        free(dest->vertices);
        return;
    }
    memcpy(dest->normals, src->normals, dest->n_normals * sizeof(vertex));

    dest->n_uvs = src->n_uvs;
    dest->max_uvs = src->max_uvs;
    dest->uvs = malloc(dest->n_uvs * sizeof(uv));
    if (!dest->uvs)
    {
        free(dest->normals);
        free(dest->vertices);
        return;
    }
    memcpy(dest->uvs, src->uvs, dest->n_uvs * sizeof(uv));
    
    dest->n_indices = src->n_indices;
    dest->max_indices = src->max_indices;
    dest->indices = malloc(dest->n_indices * sizeof(idx));
    if (!dest->indices)
    {
        free(dest->uvs);
        free(dest->normals);
        free(dest->vertices);
        return;
    }
    memcpy(dest->indices, src->indices, dest->n_indices * sizeof(idx));
}

void mesh_destroy(mesh* m)
{
    if (m->vertices) free(m->vertices);
    if (m->normals) free(m->normals);
    if (m->uvs) free(m->uvs);
}

void mesh_add_vertex(mesh* m, vertex v)
{
    if (m->n_vertices >= 0.9 * m->max_vertices)
    {
        //printf("resize: %d -> %d\n", m->max_vertices, m->max_vertices * 2);
        float* ra = realloc(m->vertices, m->max_vertices * 2 * sizeof(vertex));
        if (!ra)
        {
            printf("out of memory!\n");
            return;
        }

        m->vertices = ra;
    }

    m->vertices[m->n_vertices * 3 + 0] = v[0];
    m->vertices[m->n_vertices * 3 + 1] = v[1];
    m->vertices[m->n_vertices * 3 + 2] = v[2];
    m->n_vertices += 1;
}

void mesh_add_normal(mesh* m, vertex v)
{
    if (m->n_normals >= 0.9 * m->max_normals)
    {
        float* ra = realloc(m->normals, m->max_normals * 2 * sizeof(vertex));
        if (!ra)
        {
            printf("out of memory!\n");
            return;
        }

        m->normals = ra;
    }

    m->normals[m->n_normals * 3 + 0] = v[0];
    m->normals[m->n_normals * 3 + 1] = v[1];
    m->normals[m->n_normals * 3 + 2] = v[2];
    m->n_normals += 1;
}

void mesh_add_color(mesh* m, color v)
{

}

void mesh_add_uv(mesh* m, float* v)
{
    if (m->n_uvs >= 0.9 * m->max_uvs)
    {
        float* ra = realloc(m->uvs, m->max_uvs * 2 * sizeof(uv));
        if (!ra)
        {
            printf("out of memory!\n");
            return;
        }

        m->uvs = ra;
    }

    m->uvs[m->n_uvs * 2 + 0] = v[0];
    m->uvs[m->n_uvs * 2 + 1] = v[1];
    m->n_uvs += 1;
}

void mesh_add_index(mesh* m, uint32_t v, uint32_t t, uint32_t n)
{
    if (m->n_indices >= 0.9 * m->max_indices)
    {
        uint32_t* ra = realloc(m->indices, m->max_indices * 2 * sizeof(idx));
        if (!ra)
        {
            printf("out of memory!\n");
            return;
        }

        m->indices = ra;
    }

    m->indices[m->n_indices * 3 + 0] = v;
    m->indices[m->n_indices * 3 + 1] = t;
    m->indices[m->n_indices * 3 + 2] = n;
    m->n_indices += 1;
}

void mesh_compute_bbox(mesh* m)
{
    if (m->n_vertices < 1)
    {
        return;
    }

    float* v;
    v = &m->vertices[0];
    float xmin = v[0];
    float xmax = v[0];
    float ymin = v[1];
    float ymax = v[1];
    float zmin = v[2];
    float zmax = v[2];

    for (uint32_t i = 1; i < m->n_vertices; ++i)
    {
        v = &m->vertices[i * 3];
        if (v[0] < xmin)
        {
            xmin = v[0];
        }
        if (v[0] > xmax)
        {
            xmax = v[0];
        }
        if (v[1] < ymin)
        {
            ymin = v[1];
        }
        if (v[1] > ymax)
        {
            ymax = v[1];
        }
        if (v[2] < zmin)
        {
            zmin = v[2];
        }
        if (v[2] > zmax)
        {
            zmax = v[2];
        }
    }

    m->length[0] = xmax - xmin;
    m->length[1] = ymax - ymin;
    m->length[2] = zmax - zmin;

    printf("bounding box: %fx%fx%f\n", m->length[0], m->length[1], m->length[2]);
}
