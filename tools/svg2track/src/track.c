#include "track.h"
#include "matrix.h"
#include "wavefront.h"

#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

static void resize_arrays(track* tr, float factor)
{
    int new_max = roundf(tr->max_segments * factor);

    tr->lookup = realloc(tr->lookup, new_max * sizeof(segment_t));
    if (tr->lookup == NULL)
    {
        return;
    }
    
    tr->lines = realloc(tr->lines, new_max * sizeof(line*));
    if (tr->lines == NULL)
    {
        free(tr->lookup);
        return;
    }
    
    tr->beziers = realloc(tr->beziers, new_max * sizeof(bezier*));
    if (tr->beziers == NULL)
    {
        free(tr->lookup);
        free(tr->lines);
        return;
    }
}

void track_init(track* tr, int max_segments)
{
    tr->n_segments = 0;
    tr->n_lines = 0;
    tr->n_beziers = 0;

    tr->max_segments = max_segments;
    tr->lookup = malloc(max_segments * sizeof(segment_t));
    if (tr->lookup == NULL)
    {
        return;
    }

    tr->lines = malloc(max_segments * sizeof(line*));
    if (tr->lines == NULL)
    {
        free(tr->lookup);
        tr->lookup = NULL;
        return;
    }

    tr->beziers = malloc(max_segments * sizeof(bezier*));
    if (tr->beziers == NULL)
    {
        free(tr->lines);
        tr->lines = NULL;
        free(tr->lookup);
        tr->lookup = NULL;
        return;
    }
}

void track_destroy(track* tr)
{
    if (tr->lookup)
    {
        free(tr->lookup);
        tr->lookup = NULL;
    }

    if (tr->lines)
    {
        free(tr->lines);
        tr->lines = NULL;
    }

    if (tr->beziers)
    {
        for (int i = 0; i < tr->n_beziers; ++i)
        {
            bezier_destroy(tr->beziers[i]);
            free(tr->beziers[i]);
        }
        free(tr->beziers);
        tr->beziers = NULL;
    }
}

void track_add_bezier(track* tr, bezier* bz)
{
    assert(tr->beziers);

    tr->lookup[tr->n_segments] = SEG_BEZIER;
    ++tr->n_segments;

    tr->beziers[tr->n_beziers] = bz;
    ++tr->n_beziers;
    if (tr->n_beziers >= tr->max_segments * 0.9)
    {
        resize_arrays(tr, 2.0);
    }
}

void track_add_line(track* tr, line* ln)
{
    assert(tr->lines);

    tr->lookup[tr->n_segments] = SEG_LINE;
    ++tr->n_segments;

    tr->lines[tr->n_lines] = ln;
    ++tr->n_lines;
    if (tr->n_lines >= tr->max_segments * 0.9)
    {
        resize_arrays(tr, 2.0);
    }
}

static int interpolate_line(float* start, line* ln, int n_points)
{
    printf("line\n");
    float step = 1.f / (float)n_points;
    float t;

    for (int i = 0; i < n_points; ++i)
    {
        t = (float)i * step;
        line_solve(ln, t, start);
        printf("(%f,%f)\n", start[0], start[1]);
        start += 2; // move head to next point
    }

    return n_points;
}

static int interpolate_bezier(float* start, bezier* ln, int n_points)
{
    printf("bezeir\n");
    float step = 1.f / (float)n_points;
    float t;

    for (int i = 0; i < n_points; ++i)
    {
        t = (float)i * step;
        bezier_cubic(ln, t, start);
        printf("(%f,%f)\n", start[0], start[1]);
        start += 2; // move head to next point
    }

    return n_points;
}

void track_meshify(track* tr, mesh* out, mesh* stock, const char* out_fn)
{
    printf("start meshify...\n");
    // start with 100 pairs allocated
    int n_inter_pts = 30;
    int max_points = 1000;
    int n_points = 0;
    float* points = malloc(max_points * 2 * sizeof(float));
    if (points == NULL)
    {
        return;
    }

    line** lines_head = tr->lines;
    bezier** beziers_head = tr->beziers;

    // place all points
    printf("gen points...\n");
    for (int i = 0; i < tr->n_segments; ++i)
    {
        int added = 0;
        segment_t type = tr->lookup[i];
        printf("seg type: %d\n", type);
        switch (type)
        {
            case SEG_LINE:
                added = interpolate_line(points + (n_points * 2), *lines_head, n_inter_pts);
                n_points += added;
                ++lines_head;
                assert(lines_head - tr->lines <= tr->n_lines);
                break;

            case SEG_BEZIER:
                added = interpolate_bezier(points + (n_points * 2), *beziers_head, n_inter_pts);
                n_points += added;
                ++beziers_head;
                assert(beziers_head - tr->beziers <= tr->n_beziers);
                break;
        }

        if (n_points >= max_points * 0.9)
        {
            max_points *= 2;
            float* newp;
            newp = realloc(points, max_points * 2 * sizeof(float));
            if (!newp)
            {
                printf("oom\n");
                free(points);
                return;
            }
            points = newp;
        }
    }

    assert(n_points > 0);

    // compute all lines
    printf("gen lines...\n");

    int n_lines = n_points;
    line* lines = malloc(n_lines * sizeof(line));
    if (lines == NULL)
    {
        free(points);
        return;
    }

    float track_width = 2.f; // todo change

    float* a;
    float* b;
    float d[2];
    float u[2];
    float v[2];
    float mag;
    float tmp;
    for (int i = 0; i < n_lines; ++i)
    {
        // fetch current and next point, loop back to first if necessary
        a = points + (((i + 0) % n_points) * 2);
        b = points + (((i + 1) % n_points) * 2);

        // compute vector from a to b
        d[0] = b[0] - a[0];
        d[1] = b[1] - a[1];
        // normalize
        mag = sqrtf(d[0] * d[0] + d[1] * d[1]);
        d[0] = d[0] / mag;
        d[1] = d[1] / mag;

        // compute normal to line
        tmp = d[1];
        d[1] = -d[0];
        d[0] = tmp;

        u[0] = a[0] + d[0] * -track_width;
        u[1] = a[1] + d[1] * -track_width;
        v[0] = a[0] + d[0] * track_width;
        v[1] = a[1] + d[1] * track_width;

        line_connect(&lines[i], u[0], u[1], v[0], v[1]);
        //line_print(&lines[i]);
    }

    free(points);

    // transform all stock pieces
    printf("compute transforms...\n");

    int n_meshes = n_lines + 1;
    mesh* meshes = malloc(n_meshes * sizeof(mesh));
    if (meshes == NULL)
    {
        return;
    }

    line* front;
    line* back;
    mesh* cur;
    float fbuf[2];
    for (int i = 0; i < n_meshes; ++i)
    {
        int a = (i + 0) % n_lines;
        int b = (i + 1) % n_lines;
        front = &lines[a];
        back = &lines[b];

        //for (int j = 0; j < n_lines; ++j)
        {
            cur = &meshes[i];
            mesh_copy(cur, stock);
            for (int k = 0; k < cur->n_vertices; ++k)
            {
                float* v = &cur->vertices[k * 3];
                // snap vertices to line
                
                v[2] += 4.5; // adjust to midpoint;
                float t = v[2] / 4.5; // for line calculation
                assert (t >= -1.f && t <= 1.f);
                // back vertices
                if (v[0] == 0.0f)
                {
                    line_solve(back, t, fbuf);
                    //printf("solved: (%f,%f)\n", fbuf[0], fbuf[1]);
                }
                // front vertices
                else if (v[0] == 1.0f)
                {
                    line_solve(front, t, fbuf);
                    //printf("solved: (%f,%f)\n", fbuf[0], fbuf[1]);
                }
                v[0] = fbuf[0];
                v[2] = fbuf[1];
            }
        }
    }

    // write to single mesh
    printf("saving to %s...\n", out_fn);
    wavefront_save(meshes, out_fn, n_meshes);
}