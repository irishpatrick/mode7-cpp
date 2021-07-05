#include "geometry.h"

void quad_connect(quad* q, vec2 a, vec2 b, vec2 c, vec2 d)
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
       vec2* a = q->p + p1;
       vec2* b = q->p + p2;

       line_connect(q->l + i, a->x, a->y, b->x, b->y);
       q->length[i] = line_calc_distance(q->l + i);
    }
}

void quad_connect_raw(quad* q, float* a, float* b, float* c, float* d)
{
    vec2 va = {a[0], a[1]};
    vec2 vb = {b[0], b[1]};
    vec2 vc = {c[0], c[1]};
    vec2 vd = {d[0], d[1]};
    quad_connect(q, va, vb, vc, vd);
}

void quad_from_buffer(quad* q, float* buf)
{
    quad_connect_raw(q, buf, buf + 2, buf + 4, buf + 6);
}
