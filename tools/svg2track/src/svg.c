#include "svg.h"

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <msvg.h>

static track* cur_track;

static void parse_path(track* tr, MsvgElement* el)
{
    assert(tr != NULL);

    MsvgPathAttributes* pa = el->ppathattr;
    MsvgSubPath* sp = pa->sp;
    MsvgSubPathPoint* pt;

    double head[2];
    char cur_cmd;
    bezier* cur_curve = NULL;
    line* cur_line = NULL;
    int repeat_counter = 0;

    bool origin_set = false;
    float ox = 0.f;
    float oy = 0.f;

    for (int i = 0; i < sp->npoints; ++i)
    {
        pt = &(sp->spp[i]);
        //printf("%c, %f, %f\n", pt->cmd, pt->x, pt->y);
        switch (pt->cmd)
        {
            case 'M': // move to
                head[0] = pt->x;
                head[1] = pt->y;
                if (!origin_set)
                {
                    origin_set = true;
                    //ox = pt->x;
                    //oy = pt->y;
                    //printf("origin: %f,%f\n", ox, oy);
                }
                break;


            case 'L': // linear
                //printf("process %d\n", tr->n_segments);
                cur_cmd = pt->cmd;
                if (cur_line)
                {
                    track_add_line(tr, cur_line);
                    cur_line = NULL;
                }

                assert(cur_line == NULL);
                cur_line = malloc(sizeof(line));
                if (cur_line == NULL)
                {
                    printf("fatal\n");
                    exit(1);
                    // todo improve
                }
                if (cur_curve) // save curve
                {
                    track_add_bezier(tr, cur_curve);
                    cur_curve = NULL;   
                }

                assert(origin_set);
                line_connect(cur_line, head[0] - ox, head[1] - oy, pt->x - ox, pt->y - oy);
                
                // move head
                head[0] = pt->x;
                head[1] = pt->y;
                
                break;

            case 'C': // cubic bezier
                //printf("process %d\n", tr->n_segments);
                repeat_counter = 0;
                cur_cmd = pt->cmd;
                if (cur_line)
                {
                    track_add_line(tr, cur_line);
                    cur_line = NULL;
                }
                if (cur_curve) // save curve
                {
                    track_add_bezier(tr, cur_curve);
                    cur_curve = NULL;   
                }

                assert(cur_curve == NULL);
                cur_curve = malloc(sizeof(bezier));
                if (cur_curve == NULL)
                {
                    exit(1); // todo improve
                }
                bezier_init(cur_curve, 100);

                bezier_set_endpt(cur_curve, 0, head[0] - ox, head[1] - oy);
                bezier_set_ctrlpt(cur_curve, 0, pt->x - ox, pt->y - oy);
                break;

            case ' ': // extra points
                if (cur_cmd == 'C')
                {
                    assert(cur_curve != NULL);
                    ++repeat_counter;
                    if (repeat_counter == 1)
                    {
                        bezier_set_ctrlpt(cur_curve, 1, pt->x - ox, pt->y - oy);
                    }
                    else if (repeat_counter == 2)
                    {
                        bezier_set_endpt(cur_curve, 1, pt->x - ox, pt->y - oy);
                        head[0] = pt->x;
                        head[1] = pt->y;
                    }
                    else
                    {
                        exit(1); // todo improve
                    }
                }
                break;

            default:

            break;
        }
    }
    if (cur_curve) // check again if curve needs to be saved
    {
        track_add_bezier(tr, cur_curve);
        cur_curve = NULL;  
    }
    
    if (cur_line)
    {
        track_add_line(tr, cur_line);
        cur_line = NULL;
    }
}

static void my_parser(MsvgElement* el, MsvgPaintCtx* pctx)
{
    MsvgElement* newel;

    newel = MsvgTransformCookedElement(el, pctx);
    if (newel == NULL)
    {
        return;
    }
    
    switch (newel->eid)
    {
        case EID_RECT:
        //printf("eid rect\n");
        //YourDrawRectElement(newel);
        break;

        case EID_CIRCLE:
        //printf("eid circ\n");
        //YourDrawCircleElement(newel);
        break;

        case EID_ELLIPSE:
        //printf("eid ellipse\n");
        //YourDrawEllipseElement(newel);
        break;

        case EID_LINE:
        //printf("eid line\n");
        //YourDrawLineElement(newel);
        break;

        case EID_POLYLINE:
        //printf("eid polyline\n");
        //YourDrawPolylineElement(newel);
        break;

        case EID_POLYGON:
        //printf("eid polygon\n");
        //YourDrawPolygonElement(newel);
        break;

        case EID_PATH:
        //printf("eid_path\n");
        parse_path(cur_track, newel);
        
        default:
        break;
    }
}

int svg_construct_track(track* tr, const char* fn)
{
    MsvgElement* root;
    int err;
    int result;

    root = MsvgReadSvgFile(fn, &err);
    if (root == NULL)
    {
        fprintf(stderr, "error %d reading %s\n", err, fn);
        return 1;
    }

    result = MsvgRaw2CookedTree(root);
    if (result < 1)
    {
        fprintf(stderr, "parsing err\n");
        return 1;
    }

    cur_track = tr;
    MsvgSerCookedTree(root, my_parser);
    cur_track = NULL;

    printf("Beziers extracted: %d\n", tr->n_beziers);
    printf("Lines extracted: %d\n", tr->n_lines);

    return 0;
}