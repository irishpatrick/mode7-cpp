#include "wavefront.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>

static int extract_faces(uint32_t* dest, const char* str, const char* delim, int n_to_extract)
{
    char buf[100];
    memset(buf, 0, 100);
    char* bp = buf;
    int n_extracted = 0;

    char last = *str;

    while (last != 0 && n_extracted <= n_to_extract)
    {
        if (*str == delim[0] || *str == delim[1] || *(str) == 0)
        {
            if (strlen(buf) > 0)
            {
                *dest = atoi(buf);
                bp = buf;
                *bp = 0;
                ++dest; // move to next float in array
                ++n_extracted;
            }
        }
        else
        {
            *bp = *str;
            ++bp;
            *bp = 0;
        }
        last = *str;
        ++str;
    }

    return n_extracted;
}
static int extract_floats(float* dest, const char* str, char delim, int n_to_extract)
{
    char buf[100];
    memset(buf, 0, 100);
    char* bp = buf;
    int n_extracted = 0;
    char last = *str;

    while (last != 0 && n_extracted <= n_to_extract)
    {
        
        if (*str == delim || *(str) == 0)
        {
            if (strlen(buf) > 0)
            {
                *dest = atof(buf);
                bp = buf;
                *bp = 0;
                ++dest; // move to next float in array
                ++n_extracted;
            }
        }
        else
        {
            *bp = *str;
            ++bp;
            *bp = 0;
        }
        last = *str;
        ++str;
    }

    return n_extracted;
}

int wavefront_load(mesh** m, const char* fn)
{
    FILE* fp = fopen(fn, "r");
    if (fp == NULL)
    {
        return 0;
    }

    char* line = NULL;
    size_t len = 0;
    ssize_t nread;
    float fbuf[3];
    uint32_t ubuf[9];

    int cur_indx = -1;
    mesh* cur = NULL;

    *m = malloc(10 * sizeof(mesh));
    if (*m == NULL)
    {
        return 0;
    }

    while ((nread = getline(&line, &len, fp)) != -1)
    {
        line[strlen(line)-1] = 0; // strip newline
        if (strncmp(line, "o ", 2) == 0)
        {
            if (cur)
            {
                mesh_finish_building(cur);
            }
            ++cur_indx;
            cur = *m + cur_indx;
            mesh_init(cur);
            mesh_begin_building(cur);
        }
        if (strncmp(line, "v ", 2) == 0)
        {
            int ne = extract_floats(fbuf, line + 2, ' ', 3);
            if (ne < 3)
            {
                printf("bad vertex!\n");
                continue;
            }
            mesh_add_vertex(cur, fbuf);
        }
        else if (strncmp(line, "vt ", 3) == 0)
        {
            int ne = extract_floats(fbuf, line + 3, ' ', 2);
            if (ne < 2)
            {
                printf("bad uv!\n");
                continue;
            }
            mesh_add_uv(cur, fbuf);
        }
        else if (strncmp(line, "vn ", 3) == 0)
        {
            int ne = extract_floats(fbuf, line + 3, ' ', 3);
            if (ne < 2)
            {
                printf("bad normal!\n");
                continue;
            }
            mesh_add_normal(cur, fbuf);
        }
        else if (strncmp(line, "f ", 2) == 0)
        {
            int ne = extract_faces(ubuf, line + 2, " /", 9);
            if (ne < 9)
            {
                printf("bad face!\n");
                continue;
            }
            mesh_add_index(cur, ubuf[0], ubuf[1], ubuf[2]);
            mesh_add_index(cur, ubuf[3], ubuf[4], ubuf[5]);
            mesh_add_index(cur, ubuf[6], ubuf[7], ubuf[8]);
        }
    }

    if (cur)
    {
        mesh_finish_building(cur);
    }

    if (line != NULL)
    {
        free(line);
    }

    fclose(fp);

    return cur_indx + 1;
}

void wavefront_save(mesh* m, const char* fn, int n_meshes)
{
    FILE* fp = fopen(fn, "w");
    if (!fp)
    {
        return;
    }

    uint32_t vctr = 0;
    uint32_t nctr = 0;
    uint32_t tctr = 0;
    uint32_t vstart = 0;
    uint32_t nstart = 0;
    uint32_t tstart = 0;

    mesh* cur;
    for (int i = 0; i < n_meshes; ++i)
    {
        cur = m + i;
        fprintf(fp, "o m%d\n", i);
        for (int n = 0; n < cur->n_vertices; ++n)
        {
            int st = n * 3;
            fprintf(fp, "v %f %f %f\n", 
                cur->vertices[st + 0],
                cur->vertices[st + 1],
                cur->vertices[st + 2]);
        }

        for (int n = 0; n < cur->n_normals; ++n)
        {
            int st = n * 3;
            fprintf(fp, "vn %f %f %f\n",
                cur->normals[st + 0], 
                cur->normals[st + 1],
                cur->normals[st + 2]);
        }

        for (int n = 0; n < cur->n_uvs; ++n)
        {
            int st = n * 2;
            fprintf(fp, "vt %f %f\n",
                cur->normals[st + 0],
                cur->normals[st + 1]);
        }

        fprintf(fp, "usemtl None\ns off\n");

        for (int n = 0; n < cur->n_indices; ++n)
        {
            if (n % 3 == 0)
            {
                fprintf(fp, "f ");
            }
            int st = n * 3;
            fprintf(fp, "%d/%d/%d", 
                cur->indices[st + 0] + vstart,
                cur->indices[st + 1] + tstart,
                cur->indices[st + 2] + nstart
            );

            if ((n + 1) % 3 == 0)
            {
                fprintf(fp, "\n");
            }
            else
            {
                fprintf(fp, " ");
            }
        }
        fprintf(fp, "\n");

        vstart += cur->n_vertices;
        nstart += cur->n_normals;
        tstart += cur->n_uvs;

    }

    fclose(fp);
}