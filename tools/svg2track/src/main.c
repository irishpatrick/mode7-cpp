#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

#include "mesh.h"
#include "bezier.h"
#include "track.h"
#include "wavefront.h"
#include "preview.h"
#include "svg.h"
#include "assetpack.h"

static void print_usage(void)
{
    printf("usage: svg2track <.svg input> <.zip asset pack> <.obj output> [flags]\n");
}

int main(int argc, char** argv)
{
    int err;

    if (argc < 3)
    {
        print_usage();
        return 1;
    }

    for (int i = 3; i < argc; ++i)
    {
        // parse flags
    }

    track cur_track;
    assetpack ap;

    printf("open assetpack %s\n", argv[2]);
    err = assetpack_open(&ap, argv[2]);
    if (err)
    {
        return 1;
    }

    printf("construct track from svg\n");
    track_init(&cur_track, 5000);
    svg_construct_track(&cur_track, argv[1]);

    printf("create mesh\n");
    track_meshify(&cur_track, ap.mesh, argv[3]);
    assetpack_free(&ap);

    printf("draw preview\n");
    preview_track_to_png(&cur_track);

    printf("cleanup\n");
    track_destroy(&cur_track);

    return 0;
}
