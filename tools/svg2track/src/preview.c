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

#include "preview.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <cairo.h>

#define WIDTH_A4_600PPI 7016
#define HEIGHT_A4_600PPI 4960

#define X(v) ((v * scale) + offset_x)
#define Y(v) ((v * scale) + offset_y)
#define S(v) (v * scale)

int preview_track_to_png(track* trk)
{
    if (trk->tdata.n_segments < 1)
    {
        fprintf(stderr, "bad track\n");
        return 1;
    }
    int len = trk->tdata.clp;
    printf("draw %d segments\n", len);

    cairo_t* cr;
    cairo_surface_t* surf;

    uint32_t width = WIDTH_A4_600PPI;
    uint32_t height = HEIGHT_A4_600PPI;

    double scale = 20.0;
    double offset_x = 0.0;
    double offset_y = 0.0;
    double x;
    double y;

    surf = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
    cr = cairo_create(surf);

    // draw centerline
    cairo_set_line_width(cr, 6);
    cairo_set_source_rgba(cr, 1.0, 1.0, 1.0, 1.0);
    for (uint32_t i = 0; i < len; ++i)
    {
        line* cur = trk->tdata.centerline + i;
        x = X(cur->p1[0]);
        y = Y(cur->p1[1]);
        cairo_move_to(cr, x, y);
        //printf("move to %f,%f\n", x, y);
        x = X(cur->p2[0]);
        y = Y(cur->p2[1]);
        cairo_line_to(cr, x, y);
    }
    cairo_stroke(cr);

    cairo_set_line_width(cr, 4);

    // draw wall bounds
    cairo_set_source_rgba(cr, 1.0, 0.1, 0.1, 1.0);
    for (uint32_t i = 0; i < len; ++i)
    {
        quad* cur = trk->tdata.walls_bounds + i;
        cairo_move_to(cr, X(cur->p[0].x), Y(cur->p[0].y));
        cairo_line_to(cr, X(cur->p[1].x), Y(cur->p[1].y));
        cairo_line_to(cr, X(cur->p[2].x), Y(cur->p[2].y));
        cairo_line_to(cr, X(cur->p[3].x), Y(cur->p[3].y));
        cairo_close_path(cr);
    }
    cairo_stroke(cr);

    // draw runoff bounds
    cairo_set_source_rgba(cr, 1.0, 1.0, 0.2, 1.0);
    for (uint32_t i = 0; i < len; ++i)
    {
        quad* cur = trk->tdata.runoff_bounds + i;
        //printf("runoff %f,%f\n", X(cur->p[0].x), Y(cur->p[0].y));
        cairo_move_to(cr, X(cur->p[0].x), Y(cur->p[0].y));
        cairo_line_to(cr, X(cur->p[1].x), Y(cur->p[1].y));
        cairo_line_to(cr, X(cur->p[2].x), Y(cur->p[2].y));
        cairo_line_to(cr, X(cur->p[3].x), Y(cur->p[3].y));
        cairo_close_path(cr);
    }
    cairo_stroke(cr);

    // draw track bounds
    cairo_set_source_rgba(cr, 0.5, 1.0, 0.5, 1.0);
    for (uint32_t i = 0; i < len; ++i)
    {
        quad* cur = trk->tdata.track_bounds + i;
        cairo_move_to(cr, X(cur->p[0].x), Y(cur->p[0].y));
        cairo_line_to(cr, X(cur->p[1].x), Y(cur->p[1].y));
        cairo_line_to(cr, X(cur->p[2].x), Y(cur->p[2].y));
        cairo_line_to(cr, X(cur->p[3].x), Y(cur->p[3].y));
        cairo_close_path(cr);
    }
    cairo_stroke(cr);

    cairo_surface_write_to_png(surf, "preview.png");

    cairo_destroy(cr);
    cairo_surface_destroy(surf);

    return 0;
}