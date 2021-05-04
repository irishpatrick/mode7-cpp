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

#endif /* MESH_H */