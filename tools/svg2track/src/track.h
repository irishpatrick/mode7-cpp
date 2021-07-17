#ifndef TRACK_H
#define TRACK_H

#include "line.h"
#include "bezier.h"
#include "mesh.h"
#include "trackdata.h"

typedef enum {SEG_LINE = 0, SEG_BEZIER = 1} segment_t;

typedef struct _track
{
    int n_segments;
    int n_lines;
    int n_beziers;
    int max_segments;
    segment_t* lookup;
    line** lines;
    bezier** beziers;
    trackdata tdata;
} track;

void track_init(track*, int);
void track_destroy(track*);

void track_add_bezier(track*, bezier*);
void track_add_line(track*, line*);;

void track_meshify(track*, mesh*, const char*);

#endif /* TRACK_H */

