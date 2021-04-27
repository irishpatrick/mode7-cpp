#include "bezier.h"

#include <stdlib.h>
#include <assert.h>

int bezier_init(bezier* b, int deg)
{
    b->control = malloc(2 * (deg - 1) * sizeof(float));
    if (!b->control)
    {
        return 1;
    }

    return 0;
}

void bezier_set_endpt(bezier* b, int indx, float x, float y)
{
    switch (indx)
    {
        case 0:
        b->p1[0] = x;
        b->p1[1] = y;
        break;

        case 1:
        b->p2[0] = x;
        b->p2[1] = y;
        break;
    }
}

void bezier_set_ctrlpt(bezier* b, int indx, float x, float y)
{
    assert(b->control != NULL);
    
    float* pt = &b->control[indx * 2];
    pt[0] = x;
    pt[1] = y;
}

void bezier_quadratic(bezier* b, float t, float out[2])
{
    float* c1 = b->control + 0;

    float minus = 1.f - t;
    out[0] = c1[0] + 
            (minus * minus) * (b->p1[0] - c1[0]) +
            (t * t) * (b->p2[0] - c1[0]);

    out[1] = c1[1] + 
            (minus * minus) * (b->p1[1] - c1[1]) +
            (t * t) * (b->p2[1] - c1[1]);
}

void bezier_cubic(bezier* b, float t, float out[2])
{
    float* c1 = b->control + 0;
    float* c2 = b->control + 2;

    float minus = 1.f - t;
    out[0] = (minus * minus * minus) * b->p1[0] + 
            (3 * minus * minus * t) * c1[0] + 
            (3 * minus * t * t) * c2[0] +
            (t * t * t) * b->p2[0];

    out[1] = (minus * minus * minus) * b->p1[1] + 
            (3 * minus * minus * t) * c1[1] + 
            (3 * minus * t * t) * c2[1] +
            (t * t * t) * b->p2[1];
}

void bezier_destroy(bezier* b)
{
    free(b->control);
}