#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include "racingline.h"
#include "car.h"

int main(int argc, char** argv)
{
    Car* car_a = car_create();
    RacingLine* rl = racingline_create();
    
    racingline_load(rl, "../../sim/circle.track");
    //racingline_print(rl);

    car_set_line(car_a, rl);
    car_a->pos.x = 16;
    car_a->pos.y = 0;
    car_a->front.y = 1;
    car_a->front = vec2_rotate(car_a->front, -M_PI / 4.f);
    car_a->vel = 0.1f;

    // simulate
    for (int i = 0; i < 100000; ++i)
    //while (1)
    {
        //sleep(1);
        car_update(car_a);
    }

    car_destroy(car_a);
    racingline_destroy(rl);

    return 0;
}