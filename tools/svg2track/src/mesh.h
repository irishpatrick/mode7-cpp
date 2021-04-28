#ifndef MESH_H
#define MESH_H

#include <stdint.h>

typedef float vertex[3];
typedef float color[3];
typedef float uv[2];

typedef struct _mesh
{
    vertex* vertices;
    color* colors;
    uv* textures;
    uint32_t* indices;

    uint32_t n_vertices;
    uint32_t n_indices;
} mesh;

void mesh_init(mesh*, uint32_t);
void mesh_destroy(mesh*);
void mesh_add_vertex(mesh*, vertex);
void mesh_add_color(mesh*, color);
void mesh_add_uv(mesh*, uv);
void mesh_connect(mesh*);

#endif /* MESH_H */