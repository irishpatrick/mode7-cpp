/**
 * This file is part of mode7-cpp.
 * Copyright (C) 2021  Patrick Roche
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
**/

#include "mesh.h"
#include "bezier.h"
#include "track.h"
#include "wavefront.h"
#include "preview.h"
#include "svg.h"
#include "assetpack.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static void print_usage(void)
{
    printf("usage: svg2track <.svg input> <.zip asset pack> <.obj output> [flags]\n");
    printf("flags:\n\t--track-width\t\t<float>\n\t--runoff-width\t\t<float>\n\t--wall-width\t\t<float>\n\t--curvature-weight\t<float>\n\tmore \n");
}

static bool startswith(const char* str, const char* prefix)
{
    uint32_t prefix_len = strlen(prefix);
    if (prefix_len > strlen(str))
    {
        return false;
    }

    return strncmp(str, prefix, prefix_len) == 0;
}

int main(int argc, char** argv)
{
    int err;

    if (argc < 3)
    {
        print_usage();
        return 1;
    }

    // parse flags
    float track_width = 5.0;
    float runoff_width = 10.0;
    float wall_width = 1.0;
    char* flag;
    char* val;
    for (int i = 4; i < argc; ++i)
    {
        flag = argv[i];
        val = strrchr(flag, ' ') + 1;
        if (val == NULL)
        {
            printf("ignoring malformed flag %s\n", flag);
            continue;
        }

        if (startswith(flag, "--track-width"))
        {
            track_width = atof(val);
        }
        else if (startswith(flag, "--runoff-width"))
        {
            runoff_width = atof(val);
        }
        else if (startswith(flag, "--wall-width"))
        {
            wall_width = atof(val);
        }
        else
        {
            printf("ignoring unknown flag %s\n", flag);
        }
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
    track_init(&cur_track, 100);
    svg_construct_track(&cur_track, argv[1]);

    printf("create mesh\n");
    track_meshify(&cur_track, ap.mesh, argv[3], track_width, runoff_width, wall_width);
    assetpack_free(&ap);

    printf("draw preview\n");
    preview_track_to_png(&cur_track);

    printf("cleanup\n");
    track_destroy(&cur_track);

    return 0;
}
