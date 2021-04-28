#include <stdio.h>
#include <stdlib.h>
#include <msvg.h>
#include <assert.h>

#include "mesh.h"
#include "bezier.h"
#include "track.h"

static track cur_track;

static void print_usage(void)
{
    printf("usage: svg2track <.svg file>\n");
}

static void parse_path(MsvgElement* el)
{
    MsvgPathAttributes* pa = el->ppathattr;
    MsvgSubPath* sp = pa->sp;
    MsvgSubPathPoint* pt;

    double head[2];
    char cur_cmd;
    bezier* cur_curve = NULL;
    line* cur_line = NULL;
    int repeat_counter = 0;

    for (int i = 0; i < sp->npoints; ++i)
    {
        pt = &(sp->spp[i]);
        printf("%c, %f, %f\n", pt->cmd, pt->x, pt->y);
        switch (pt->cmd)
        {
            case 'M': // move to
                head[0] = pt->x;
                head[1] = pt->y;
            break;

            case 'L': // linear
                cur_cmd = pt->cmd;
                if (cur_line)
                {
                    track_add_line(&cur_track, cur_line);
                    cur_line = NULL;
                }

                assert(cur_line == NULL);
                cur_line = malloc(sizeof(line));
                if (cur_line == NULL)
                {
                    exit(1);
                    // todo improve
                }

                line_connect(cur_line, head[0], head[1], pt->x, pt->y);
                
                // move head
                head[0] = pt->x;
                head[1] = pt->y;
                
            break;

            case 'C': // cubic bezier
                repeat_counter = 0;
                cur_cmd = pt->cmd;
                if (cur_curve) // save curve
                {
                    track_add_bezier(&cur_track, cur_curve);
                    cur_curve = NULL;   
                }

                assert(cur_curve == NULL);
                cur_curve = malloc(sizeof(bezier));
                if (cur_curve == NULL)
                {
                    exit(1); // todo improve
                }
                bezier_init(cur_curve, 100);

                bezier_set_endpt(cur_curve, 0, head[0], head[1]);
                bezier_set_ctrlpt(cur_curve, 0, pt->x, pt->y);
                break;

            case ' ': // extra points
                if (cur_cmd == 'C')
                {
                    assert(cur_curve != NULL);
                    ++repeat_counter;
                    if (repeat_counter == 1)
                    {
                        bezier_set_ctrlpt(cur_curve, 1, pt->x, pt->y);
                    }
                    else if (repeat_counter == 2)
                    {
                        bezier_set_endpt(cur_curve, 1, pt->x, pt->y);
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
        track_add_bezier(&cur_track, cur_curve);
        cur_curve = NULL;  
    }
    
    if (cur_line)
    {
        track_add_line(&cur_track, cur_line);
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
        //YourDrawRectElement(newel);
        break;

        case EID_CIRCLE:
        //YourDrawCircleElement(newel);
        break;

        case EID_ELLIPSE:
        //YourDrawEllipseElement(newel);
        break;

        case EID_LINE:
        //YourDrawLineElement(newel);
        break;

        case EID_POLYLINE:
        //YourDrawPolylineElement(newel);
        break;

        case EID_POLYGON:
        //YourDrawPolygonElement(newel);
        break;

        case EID_PATH:
        parse_path(newel);
        default:
        break;
    }
}

int main(int argc, char** argv)
{
    MsvgElement* root;
    int err;

    if (argc < 2)
    {
        print_usage();
        return 1;
    }

    root = MsvgReadSvgFile(argv[1], &err);
    if (root == NULL)
    {
        fprintf(stderr, "error %d reading %s\n", err, argv[1]);
        return 1;
    }

    int result;
    result = MsvgRaw2CookedTree(root);
    if (result < 1)
    {
        fprintf(stderr, "parsing err\n");
        return 1;
    }

    track_init(&cur_track, 100);
    
    MsvgSerCookedTree(root, my_parser);

    printf("Beziers extracted: %d\n", cur_track.n_beziers);
    printf("Lines extracted: %d\n", cur_track.n_lines);

    track_destroy(&cur_track);

    return 0;
}