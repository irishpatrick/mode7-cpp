#ifndef TRACKDATA_H
#define TRACKDATA_H

#include "line.h"
#include "geometry.h"

#include <stdint.h>

typedef struct _trackdata
{
    uint32_t n_segments;
    uint32_t max_segments;
    float width;

    uint32_t clp;
    uint32_t tbp;
    uint32_t rbp;
    uint32_t wbp;

    uint32_t clmax;
    uint32_t tbmax;
    uint32_t rbmax;
    uint32_t wbmax;
    
    line* centerline;
    quad* track_bounds;
    quad* runoff_bounds;
    quad* walls_bounds;
} trackdata;

void trackdata_init(trackdata*);
void trackdata_save(trackdata*, const char*);
void trackdata_free(trackdata*);

void trackdata_centerline(trackdata*, line*);
void trackdata_track_bounds(trackdata*, float*, float*, float*, float*);
void trackdata_runoff_bounds(trackdata*, float);
void trackdata_walls_bounds(trackdata*, float);
void trackdata_push_all(trackdata*);
void trackdata_push_clp(trackdata*);
void trackdata_push_tbp(trackdata*);
void trackdata_push_rbp(trackdata*);
void trackdata_push_wbp(trackdata*);

#endif /* TRACKDATA_H */
