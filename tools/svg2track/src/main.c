#include <stdio.h>
#include <stdlib.h>
#include <msvg.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

#include "mesh.h"
#include "bezier.h"
#include "track.h"
#include "wavefront.h"
#include "preview.h"

static track cur_track;

static void print_usage(void)
{
    printf("usage: svg2track <.svg input> <.zip asset pack> <.obj output> [flags]\n");
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

    bool origin_set = false;
    float ox = 0.f;
    float oy = 0.f;

    for (int i = 0; i < sp->npoints; ++i)
    {
        pt = &(sp->spp[i]);
        printf("%c, %f, %f\n", pt->cmd, pt->x, pt->y);
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
                    printf("origin: %f,%f\n", ox, oy);
                }
                break;


            case 'L': // linear
                printf("process %d\n", cur_track.n_segments);
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
                    printf("fatal\n");
                    exit(1);
                    // todo improve
                }
                if (cur_curve) // save curve
                {
                    track_add_bezier(&cur_track, cur_curve);
                    cur_curve = NULL;   
                }

                assert(origin_set);
                line_connect(cur_line, head[0] - ox, head[1] - oy, pt->x - ox, pt->y - oy);
                
                // move head
                head[0] = pt->x;
                head[1] = pt->y;
                
                break;

            case 'C': // cubic bezier
                printf("process %d\n", cur_track.n_segments);
                repeat_counter = 0;
                cur_cmd = pt->cmd;
                if (cur_line)
                {
                    track_add_line(&cur_track, cur_line);
                    cur_line = NULL;
                }
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
        printf("eid rect\n");
        //YourDrawRectElement(newel);
        break;

        case EID_CIRCLE:
        printf("eid circ\n");
        //YourDrawCircleElement(newel);
        break;

        case EID_ELLIPSE:
        printf("eid ellipse\n");
        //YourDrawEllipseElement(newel);
        break;

        case EID_LINE:
        printf("eid line\n");
        //YourDrawLineElement(newel);
        break;

        case EID_POLYLINE:
        printf("eid polyline\n");
        //YourDrawPolylineElement(newel);
        break;

        case EID_POLYGON:
        printf("eid polygon\n");
        //YourDrawPolygonElement(newel);
        break;

        case EID_PATH:
        printf("eid_path\n");
        parse_path(newel);
        default:
        break;
    }
}

int main(int argc, char** argv)
{
    MsvgElement* root;
    int err;
    char stock_fn[100];
    mesh* stock = NULL;

    if (argc < 3)
    {
        print_usage();
        return 1;
    }

    memset(stock_fn, 0, 100);

    for (int i = 3; i < argc; ++i)
    {
        // parse flags
    }

    if (strlen(stock_fn) < 1)
    {
        strcpy(stock_fn, "./assets/default/default.obj");
    }

    int n_meshes = wavefront_load(&stock, stock_fn);
    if (n_meshes == 0 || n_meshes > 1)
    {
        printf("bad stock piece");
        for (int i = 0; i < n_meshes; ++i)
        {
            mesh_destroy(stock + i);
        }
        if (stock) free(stock);
        exit(1);
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

    mesh out;
    mesh_init(&out);

    track_meshify(&cur_track, &out, &stock[0], argv[2]);

    preview_track_to_png(&cur_track);

    track_destroy(&cur_track);

    for (int i = 0; i < n_meshes; ++i)
    {
        mesh_destroy(stock + i);
    }
    if (stock) free(stock);

    return 0;
}
