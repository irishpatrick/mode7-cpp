#ifndef BEZIER_H
#define BEZIER_H

typedef struct _bezier
{
    float p1[2];
    float p2[2];
    float* control;
} bezier;

int bezier_init(bezier*, int);
void bezier_set_endpt(bezier*, int, float, float);
void bezier_set_ctrlpt(bezier*, int, float, float);
void bezier_quadratic(bezier* b, float t, float out[2]);
void bezier_cubic(bezier*, float, float[2]);
void bezier_destroy(bezier*);

#endif /* BEZIER_H */