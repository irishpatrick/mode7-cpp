#include <stdio.h>
#include <stdlib.h>
#include <msvg.h>

#include "mesh.h"
#include "bezier.h"

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

            break;

            case 'C': // cubic bezier
                repeat_counter = 0;
                cur_cmd = pt->cmd;
                if (cur_curve) // save curve
                {
                    
                }
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

    //MsvgPrintRawElementTree(stdout, root, 0);

    int result;
    result = MsvgRaw2CookedTree(root);
    if (result < 1)
    {
        fprintf(stderr, "parsing err\n");
        return 1;
    }
    
    MsvgSerCookedTree(root, my_parser);

    return 0;
}