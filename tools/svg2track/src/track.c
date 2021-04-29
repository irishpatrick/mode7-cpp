#include "track.h"
#include "matrix.h"

#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

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

    tr->lines[tr->n_lines] = ln;
    ++tr->n_lines;
    if (tr->n_lines >= tr->max_segments * 0.9)
    {
        resize_arrays(tr, 2.0);
    }
}

static int interpolate_line(float* start, line* ln)
{
    return 20;
}

static int interpolate_bezier(float* start, bezier* ln)
{
    return 20;
}

void track_meshify(track* tr, mesh* out, mesh* stock)
{
    printf("start meshify...\n");
    // start with 100 pairs allocated
    int max_points = 100;
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
        switch (type)
        {
            case SEG_LINE:
                added = interpolate_line(points + (n_points * 2), *lines_head);
                n_points += added;
                ++lines_head;
                assert(lines_head - tr->lines <= tr->n_lines);
                break;

            case SEG_BEZIER:
                added = interpolate_bezier(points + (n_points * 2), *beziers_head);
                n_points += added;
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
                free(points);
                return;
            }
            points = newp;
        }
    }

    // compute all lines
    printf("gen lines...\n");

    int n_lines = n_points;
    line* lines = malloc(n_lines * sizeof(line));
    if (lines == NULL)
    {
        free(points);
        return;
    }

    float track_width = 3.f; // todo change

    float* a;
    float* b;
    float d[2];
    float u[2];
    float v[2];
    float mag;
    float tmp;
    for (int i = 0; i < n_points - 1; ++i)
    {
        // fetch current and next point
        a = points + ((i + 0) * 2);
        b = points + ((i + 1) * 2);

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
    }

    free(points);

    // transform all stock pieces
    printf("compute transforms...\n");

    // write to single mesh
    printf("saving...\n");

}