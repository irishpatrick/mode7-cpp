#ifndef RACINGLINE_H
#define RACINGLINE_H

#include "line.h"
#include "rect.h"

typedef struct _RacingLine
{
    Line* lines;
    Rect* rects;
    int num_lines;
} RacingLine;

RacingLine* racingline_create();
void racingline_load(RacingLine*, const char*);
int racingline_get_zone(RacingLine*, vec2);
int racingline_check_advance(RacingLine*, vec2, int);
Line* racingline_get_line(RacingLine*, int);
void racingline_destroy(RacingLine*);
void racingline_print(RacingLine*);

#endif /* RACINGLINE_H */