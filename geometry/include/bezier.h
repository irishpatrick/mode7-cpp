#ifndef BEZIER_H
#define BEZIER_H

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

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
float bezier_estimate_distance(bezier*, float);
void bezier_destroy(bezier*);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* BEZIER_H */