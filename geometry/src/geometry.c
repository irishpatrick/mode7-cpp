#include "geometry.h"

#include <stdbool.h>
#include <stdio.h>
#include <assert.h>

static int ntf = 0;

int quad_connect(quad* q, vec2 a, vec2 b, vec2 c, vec2 d)
{
    q->p[0].x = a.x;
    q->p[0].y = a.y;
    q->p[1].x = b.x;
    q->p[1].y = b.y;
    q->p[2].x = c.x;
    q->p[2].y = c.y;
    q->p[3].x = d.x;
    q->p[3].y = d.y;

    int p1;
    int p2;
    for (int i = 0; i < 4; ++i)
    {
       p1 = (i + 0) % 4;
       p2 = (i + 1) % 4;
       vec2* u = q->p + p1;
       vec2* v = q->p + p2;

       line_connect(q->l + i, u->x, u->y, v->x, v->y);
       q->length[i] = line_calc_distance(q->l + i);
    }

    q->area = q->length[0] * q->length[1]; // wrong

    bool int02 = line_intersects(q->l + 0, q->l + 2);
    bool int13 = line_intersects(q->l + 1, q->l + 3);
    assert(!(int02 && int13));

    int result = 2 * int02 + 1 * int13;
    return result;
}

void quad_print(quad* q)
{
    printf("(%.3f,%.3f)\t(%.3f,%.3f)\n(%.3f,%.3f)\t(%.3f,%.3f)\n", 
        q->p[0].x, q->p[0].y,
        q->p[1].x, q->p[1].y,
        q->p[2].x, q->p[2].y,
        q->p[3].x, q->p[3].y
    );
}

int quad_orient(quad* q, int type)
{
    vec2 fixed1 = q->p[0];
    vec2 swap1;
    vec2 swap2;
    vec2 fixed2;
    int out = 0;

    switch (type)
    {
        case 2:
        swap1 = q->p[1];
        swap2 = q->p[2];
        fixed2 = q->p[3];
        out = quad_connect(q, fixed1, swap2, swap1, fixed2);
        // fixed2 = q->p[1];
        // swap1 = q->p[2];
        // swap2 = q->p[3];
        // out = quad_connect(q, fixed1, fixed2, swap2, swap1);
        break;

        case 1:
        fixed2 = q->p[1];
        swap1 = q->p[2];
        swap2 = q->p[3];
        out = quad_connect(q, fixed1, fixed2, swap2, swap1);
        break;
    }

    return out;
}

int quad_connect_raw(quad* q, float* a, float* b, float* c, float* d)
{
    vec2 va = {a[0], a[1]};
    vec2 vb = {b[0], b[1]};
    vec2 vc = {c[0], c[1]};
    vec2 vd = {d[0], d[1]};
    return quad_connect(q, va, vb, vc, vd);
}

int quad_from_buffer(quad* q, float* buf)
{
    return quad_connect_raw(q, buf, buf + 2, buf + 4, buf + 6);
}

float vec2_magnitude(vec2* v)
{
    return sqrtf(v->x * v->x + v->y * v->y);
}

void vec2_normalize(vec2* v)
{
    float m = vec2_magnitude(v);
    v->x /= m;
    v->y /= m;
}
