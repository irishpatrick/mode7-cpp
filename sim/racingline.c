#include "racingline.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#ifdef _WIN32
#include "getline.h"
#endif

RacingLine* racingline_create()
{
    RacingLine* out = (RacingLine*)malloc(sizeof(RacingLine));
    memset(out, 0, sizeof(RacingLine));
    return out;
}

void racingline_load(RacingLine* rl, const char* fn)
{
    FILE* fp = fopen(fn, "r");
    if (!fp)
    {
        printf("cannot open %s!\n", fn);
        return;
    }

    size_t len = 0;
    size_t read;
    char* line = NULL;
    int lnum = 0;
    while ((read = getline(&line, &len, fp)) != -1)
    {
        if (lnum == 0)
        {
            rl->num_lines = atoi(line);
            rl->lines = (Line*)malloc(sizeof(Line) * rl->num_lines);
            rl->rects = (Rect*)malloc(sizeof(Rect) * rl->num_lines);
        }
        else
        {
            char* start = line;
            char ax[16];
            char ay[16];
            char bx[16];
            char by[16];
            
            int ind[4];
            int cur_ind = 0;
            for (char* c = line; *c != '\0'; ++c)
            {
                if (*c == ',' || *c == '\n')
                {
                    ind[cur_ind] = c - start;
                    ++cur_ind;
                }
            }

            memset(ax, 0, 16);
            memset(ay, 0, 16);
            memset(bx, 0, 16);
            memset(by, 0, 16);
            strncpy(ax, line, ind[0]);
            strncpy(ay, line + ind[0] + 1, ind[1] - ind[0]);
            strncpy(bx, line + ind[1] + 1, ind[2] - ind[1]);
            strncpy(by, line + ind[2] + 1, ind[3] - ind[2]);
            Line* cur = &rl->lines[lnum - 1];
            vec2 a;
            vec2 b;
            a.x = atof(ax);
            a.y = atof(ay);
            b.x = atof(bx);
            b.y = atof(by);
            *cur = line_from_points(a, b);
        }
        ++lnum;
    }

    fclose(fp);
    if (line)
    {
        free(line);
    }

    // generate rectangles
    for (int i = 0; i < rl->num_lines; ++i)
    {
        Line* l = &rl->lines[i];
        vec2 l_norm = line_normal(*l);
        Line p_norm = line_create(l->p, l_norm);
        Line e_norm = line_create(l->e, l_norm);
        float w = 5;
        vec2 a = line_solve(p_norm, -w);
        vec2 b = line_solve(p_norm, w);
        vec2 c = line_solve(e_norm, w);
        vec2 d = line_solve(e_norm, -w);
        rl->rects[i] = rect_create_corners(a, b, c, d);
    }
}

int racingline_get_zone(RacingLine* rl, vec2 pos)
{
    for (int i = 0; i < rl->num_lines; ++i)
    {
        if (rect_is_point_inside(&rl->rects[i], pos))
        {
            return i;
        }
    }

    return -1;
}

int racingline_check_advance(RacingLine* rl, vec2 pos, int rect)
{
    int i = (rect - 1) % rl->num_lines;
    for (; i < 2; ++i)
    {
        if (rect_is_point_inside(&rl->rects[i], pos))
        {
            return i;
        }
    }

    return rect;
}

Line* racingline_get_line(RacingLine* rl, int index)
{
    return &rl->lines[index];
}

void racingline_destroy(RacingLine* rl)
{
    if (rl->lines)
    {
        free(rl->lines);
    }

    if (rl->rects)
    {
        free(rl->rects);
    }

    free(rl);
}

void racingline_print(RacingLine* rl)
{
    for (int i = 0; i < rl->num_lines; ++i)
    {
        line_print(&rl->lines[i]);
        printf("\n");
        rect_print(&rl->rects[i]);
        printf("\n");
    }
}