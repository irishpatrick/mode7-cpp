#ifndef TRACKDATA_H
#define TRACKDATA_H

#include "line.h"
#include "geometry.h"

#include <stdint.h>

typedef struct _trackdata
{
    uint32_t n_segments;
    float width;
    
    line* centerline;
    quad* track_bounds;
    quad* runoff_bounds;
    quad* walls_bounds;
} trackdata;

void trackdata_add_segment(trackdata*, float* a, float* b);

#endif /* TRACKDATA_H */