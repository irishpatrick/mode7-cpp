#include "track.h"
#include "matrix.h"

#include <math.h>
#include <stdlib.h>
#include <assert.h>

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

void track_meshify(track* tr, mesh* m)
{
    // place all points

    // make all lines

    // transform all stock pieces

    // write to single mesh
}