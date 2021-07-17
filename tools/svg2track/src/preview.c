#include "preview.h"

#include <stdlib.h>
#include <stdint.h>
#include <cairo.h>

#define WIDTH_A4_600PPI 7016
#define HEIGHT_A4_600PPI 4960

int preview_track_to_png(track* trk)
{
    cairo_t* cr;
    cairo_surface_t* surf;

    uint32_t width = WIDTH_A4_600PPI;
    uint32_t height = HEIGHT_A4_600PPI;

    surf = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
    cr = cairo_create(surf);

    // draw centerline
    cairo_set_line_width(cr, 4);
    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
    for (uint32_t i = 0; i < trk->n_lines; ++i)
    {
        line* cur = trk->lines[i];
        cairo_move_to(cr, cur->p1[0], cur->p1[1]);
        cairo_move_to(cr, cur->p2[0], cur->p2[1]);
    }
    cairo_stroke(cr);

    // draw track bounds

    // draw runoff bounds

    // draw wall bounds

    cairo_surface_write_to_png(surf, "preview.png");

    cairo_destroy(cr);
    cairo_surface_destroy(surf);

    return 0;
}