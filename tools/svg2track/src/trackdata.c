#include "trackdata.h"

#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

static line* centerline_head(trackdata* td)
{
    return td->centerline + (td->clp - 1);
}

static quad* track_head(trackdata* td)
{
    return td->track_bounds + (td->tbp - 1);
}

static quad* runoff_head(trackdata* td)
{
    return td->runoff_bounds + (td->rbp - 1);
}

static quad* walls_head(trackdata* td)
{
    return td->walls_bounds + (td->wbp - 1);
}

static int check(void* ptr)
{
    return ptr == NULL;
}

static void extend_quad(quad* dest, quad* src, float amt)
{
    line wide[2];
    int lp = 0;
    int skip = -1;
    float max = -1.f;
    int max_indx = -1;
    for (int i = 0; i < 2; ++i)
    {
        max = -1.f;
        for (int j = 0; j < 4; ++j)
        {
            if (max_indx == j)
            {
                continue;
            }

            if (src->length[j] > max)
            {
                max = src->length[j];
                skip = j;
            }
            max_indx = skip;
        }

        memcpy(&wide[lp], src->l + skip, sizeof(line));
        ++lp;
    }

    quad_connect_raw(dest, wide[0].p1, wide[0].p2, wide[1].p2, wide[1].p1);
}

void trackdata_init(trackdata* td)
{
    td->max_segments = 100; // initial size
    td->n_segments = 1;
    td->clmax = 100;
    td->tbmax = 100;
    td->rbmax = 100;
    td->wbmax = 100;
    td->clp = 1;
    td->tbp = 1;
    td->rbp = 1;
    td->wbp = 1;
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
    FILE* fp = fopen(fn, "wb");
    if (!fp)
    {
        return;
    }

    // begin header
    // magic word
    fwrite("TDAT", sizeof(char), 4, fp);
    // num segments
    printf("%d,%d,%d,%d\n", td->clp, td->tbp, td->rbp, td->wbp);
    //assert((td->clp - td->tbp + td->rbp - td->wbp) == 0);
    fwrite(&td->clp, sizeof(uint32_t), 1, fp);
    // starting locations
    uint32_t start = 
        4 * sizeof(char) +          // magic word
        1 * sizeof(uint32_t) +      // num segments
        1 * sizeof(uint32_t) +      // starting locations
        1 * sizeof(uint32_t);       // stride
    fwrite(&start, sizeof(uint32_t), 1, fp);
    
    uint32_t stride = (4 + 8 + 8) * sizeof(float);
    fwrite(&stride, sizeof(uint32_t), 1, fp);
    
    line* ln;
    quad* qd;
    float qbuf[8];
    int i;
    int j;
    for (i = 0; i < td->clp; ++i)
    {
        ln = td->centerline + i;
        fwrite(ln->p1, sizeof(float), 2, fp);
        fwrite(ln->p2, sizeof(float), 2, fp);

        qd = td->track_bounds + i;
        for (j = 0; j < 4; ++j)
        {
            qbuf[j * 2 + 0] = qd->p[j].x;
            qbuf[j * 2 + 1] = qd->p[j].y;
        }
        fwrite(qbuf, sizeof(float), 8, fp);

        qd = td->runoff_bounds + i;
        for (j = 0; j < 4; ++j)
        {
            qbuf[j * 2 + 0] = qd->p[j].x;
            qbuf[j * 2 + 1] = qd->p[j].y;
        }
        fwrite(qbuf, sizeof(float), 8, fp);

        qd = td->walls_bounds + i;
        for (j = 0; j < 4; ++j)
        {
            qbuf[j * 2 + 0] = qd->p[j].x;
            qbuf[j * 2 + 1] = qd->p[j].y;
        }
        fwrite(qbuf, sizeof(float), 8, fp);

    }

    fclose(fp);
}

void trackdata_free(trackdata* td)
{
    free(td->centerline);
    free(td->track_bounds);
    free(td->runoff_bounds);
    free(td->walls_bounds);
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
    quad_connect_raw(tb, a, b, c, d);
}

void trackdata_runoff_bounds(trackdata* td, float len)
{
    line* cl = centerline_head(td);
    if (!cl)
    {
        return;
    }

    quad* tb = track_head(td);
    if (!tb)
    {
        return;
    }

    quad* rb = runoff_head(td);
    extend_quad(rb, tb, len);
}

void trackdata_walls_bounds(trackdata* td, float len)
{
    line* cl = centerline_head(td);
    if (!cl)
    {
        return;
    }

    quad* tb = track_head(td);
    if (!tb)
    {
        return;
    }

    quad* wb = walls_head(td);
    extend_quad(wb, tb, len);
}

void trackdata_push_all(trackdata* td)
{
    trackdata_push_clp(td);
    trackdata_push_tbp(td);
    trackdata_push_rbp(td);
    trackdata_push_wbp(td);
}

void trackdata_push_clp(trackdata* td)
{
    ++td->clp;
    if (td->clp >= 0.75 * td->clmax)
    {
        td->clmax *= 2;
        int sz = td->clmax * sizeof(line);
        void* newp = realloc(td->centerline, sz);
        if (!newp)
        {
            return;
        }
        td->centerline = newp;
    }
}

void trackdata_push_tbp(trackdata* td)
{
    ++td->tbp;
    if (td->tbp >= 0.75 * td->tbmax)
    {
        td->tbmax *= 2;
        int sz = td->tbmax * sizeof(quad);
        void* newp = realloc(td->track_bounds, sz);
        if (!newp)
        {
            return;
        }
        td->track_bounds = newp;
    }
}

void trackdata_push_rbp(trackdata* td)
{
    ++td->rbp;
    if (td->rbp >= 0.75 * td->rbmax)
    {
        td->rbmax *= 2;
        int sz = td->rbmax * sizeof(quad);
        void* newp = realloc(td->runoff_bounds, sz);
        if (!newp)
        {
            return;
        }
        td->runoff_bounds = newp;
    }
}

void trackdata_push_wbp(trackdata* td)
{
    ++td->wbp;
    if (td->wbp >= 0.75 * td->wbmax)
    {
        td->wbmax *= 2;
        int sz = td->wbmax * sizeof(quad);
        void* newp = realloc(td->walls_bounds, sz);
        if (!newp)
        {
            return;
        }
        td->walls_bounds = newp;
    }
}
