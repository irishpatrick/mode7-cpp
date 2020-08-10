#ifndef CAR_H
#define CAR_H

#include "racingline.h"
#include "vec2.h"

typedef struct _Car
{
    vec2 pos;
    vec2 front;
    float vel;
    int zone;
    RacingLine* racingline;
} Car;

Car* car_create();
void car_control(Car*, Line, Line);
void car_update(Car*);
void car_set_line(Car*, RacingLine*);
void car_destroy(Car*);

#endif /* CAR_H */