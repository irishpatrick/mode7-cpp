#include <stdlib.h>
#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <SDL.h>
#include "racingline.h"
#include "car.h"
#include "monitor.h"

int main(int argc, char** argv)
{
    monitor_init(1024, 1024);
    monitor_set_viewport(64, 64, 0);

    Car* car_a = car_create();
    RacingLine* rl = racingline_create();
    
    racingline_load(rl, "../../sim/circle.track");

    car_set_line(car_a, rl);
    car_a->pos.x = 64;
    car_a->pos.y = 0;
    car_a->front.y = 1;
    car_a->front = vec2_rotate(car_a->front, -M_PI / 4.f);
    car_a->vel = 0.1f;

    while (!monitor_should_quit())
    {
        monitor_update();

        monitor_clear();
        // do drawing
        for (int i = 0; i < rl->num_lines; ++i)
        {
            Line* l = racingline_get_line(rl, i);
            //printf("%f,%f\n", l->p.x, l->p.y);
            monitor_draw_line((int)l->p.x, (int)l->p.y, (int)l->e.x, (int)l->e.y);
        }
        car_update(car_a);
        //monitor_set_viewport(512 - roundf(car_a->pos.x / 1.f), 512 - roundf(car_a->pos.y / 1.f), 1.f);
        monitor_draw_car(roundf(car_a->pos.x), roundf(car_a->pos.y), 255, 0, 0);
        monitor_draw();
        SDL_Delay(20);
    }

    car_destroy(car_a);
    racingline_destroy(rl);

    monitor_destroy();

    return 0;
}