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
void trackdata_push(trackdata*);

#endif /* TRACKDATA_H */