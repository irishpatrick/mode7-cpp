#include "trackdata.h"

#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

static line* centerline_head(trackdata* td)
{
    return td->centerline + (td->n_segments - 1);
}

static quad* track_head(trackdata* td)
{
    return td->track_bounds + (td->n_segments - 1);
}

static quad* runoff_head(trackdata* td)
{
    return td->runoff_bounds + (td->n_segments - 1);
}

static quad* walls_head(trackdata* td)
{
    return td->walls_bounds + (td->n_segments - 1);
}

static int check(void* ptr)
{
    return ptr == NULL;
}

void trackdata_init(trackdata* td)
{
    td->max_segments = 100; // initial size
    td->n_segments = 1;
    int size_line = td->max_segments * sizeof(line);
    int size_quad = td->max_segments * sizeof(quad);
    
    td->centerline = malloc(size_line);
    if (check(td->centerline))
    {
        return;
    }
    td->track_bounds = malloc(size_quad);
    if (check(td->track_bounds))
    {
        free(td->centerline);
        return;
    }
    td->runoff_bounds = malloc(size_quad);
    if (check(td->runoff_bounds))
    {
        free(td->track_bounds);
        free(td->centerline);
        return;
    }
    td->walls_bounds = malloc(size_quad);
    if (check(td->walls_bounds))
    {
        free(td->runoff_bounds);
        free(td->track_bounds);
        free(td->centerline);
        return;
    }

    memset(td->centerline, 0, size_line);
    memset(td->track_bounds, 0, size_quad);
    memset(td->runoff_bounds, 0, size_quad);
    memset(td->walls_bounds, 0, size_quad);
}

void trackdata_save(trackdata* td, const char* fn)
{

}

void trackdata_free(trackdata* td)
{

}

void trackdata_centerline(trackdata* td, line* ln)
{
    line* cl = centerline_head(td);
    memcpy(cl, ln, sizeof(line));
}

void trackdata_track_bounds(trackdata* td, float* a, float* b, float* c, float* d)
{
    line* cl = centerline_head(td);
    if (!cl)
    {
        return;
    }

    quad* tb = track_head(td);
}

void trackdata_runoff_bounds(trackdata* td, float len)
{
    line* cl = centerline_head(td);
    if (!cl)
    {
        return;
    }

    quad* rb = runoff_head(td);
}

void trackdata_walls_bounds(trackdata* td, float len)
{
    line* cl = centerline_head(td);
    if (!cl)
    {
        return;
    }

    quad* wb = walls_head(td);
}

void trackdata_push(trackdata* td)
{
    ++td->n_segments;
    if (td->n_segments >= 0.75 * td->max_segments)
    {
        td->max_segments *= 2;
        void* newptr;
        int size_line = td->max_segments * sizeof(line);
        int size_quad = td->max_segments * sizeof(quad);

        newptr = realloc(td->centerline, size_line);
        if (!newptr)
        {
            return;
        }
        td->centerline = newptr;

        newptr = realloc(td->track_bounds, size_quad);
        if (!newptr)
        {
            return;
        }
        td->track_bounds = newptr;

        newptr = realloc(td->runoff_bounds, size_quad);
        if (!newptr)
        {
            return;
        }
        td->runoff_bounds = newptr;

        newptr = realloc(td->walls_bounds, size_quad);
        if (!newptr)
        {
            return;
        }
        td->walls_bounds = newptr;
    }
}