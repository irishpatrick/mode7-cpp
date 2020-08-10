#include "car.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

Car* car_create()
{
    Car* out = (Car*)malloc(sizeof(Car));
    memset(out, 0, sizeof(Car));
    return out;
}

void car_set_line(Car* c, RacingLine* rl)
{
    c->racingline = rl;
}

void car_control(Car* c, Line cur, Line next)
{
    float dist_to_line = line_dist_to(cur, c->pos);
    float dist_to_next = vec2_mag(vec2_sub(next.p, c->pos));
    float abs_dist_to_line = fabsf(dist_to_line);
    
    float dot_line = vec2_dot(cur.v, c->front);
    float cross_line = vec2_cross(cur.v, c->front);
    
    float dot_next = vec2_dot(vec2_norm(next.v), vec2_norm(c->front));
    float cross_next = vec2_cross(next.v, c->front);

    /*printf(
        "%f\t%f\t%f\t%f\t%f\t%f\t%f\n", 
        c->pos.x, 
        c->pos.y, 
        dot_line, 
        cross_line, 
        dist_to_line, 
        dot_next,
        dist_to_next
    );*/

    int moving_right = cross_line < 0.f;
    int moving_left = cross_line > 0.f;
    int right_of_line = dist_to_line < 0.f;
    int left_of_line = dist_to_line > 0.f;
    int dist_threshold = abs_dist_to_line > 0.5f;

    /*printf(
        "dist: %f\tleft_of_line: %d\tright_of_line: %d\n",
        dist_to_line,
        left_of_line,
        right_of_line
    );*/

    if (dist_to_next < 1.f && dot_next < 0.9f)
    {
        //fprintf(stderr, "slow\n");
        c->vel = 0.5f;
    }
    else
    {
        //fprintf(stderr, "fast\n");
        c->vel = 0.5f;
    }

    c->vel = 0.1f + dot_next;

    //if (dist_to_line < 0.f && cross_line <= 0.f) // turn left
    if (right_of_line && moving_right && dist_threshold)
    {
        c->front = vec2_rotate(c->front, 0.2f);
        printf("turn left\n");
    }
    //else if (dist_to_line > 0.f && cross_line >= 0.f) // turn right
    else if (left_of_line && moving_left && dist_threshold)
    {
        c->front = vec2_rotate(c->front, -0.2f);
        printf("turn right\n");
    }

    //printf("\n");
}

void car_update(Car* c)
{
    c->pos = vec2_add(c->pos, vec2_scale(c->vel, c->front));
    //c->front = vec2_rotate(c->front, 0.001f);

    int zone = racingline_get_zone(c->racingline, c->pos);
    if (zone >= 0)
    {
        int future = (zone + 1) % c->racingline->num_lines;
        //printf("%d: ", zone);
        car_control(c, *racingline_get_line(c->racingline, zone), *racingline_get_line(c->racingline, future));
    }
    else
    {
        printf("bad zone\n");
    }
    
}

void car_destroy(Car* c)
{
    free(c);
}