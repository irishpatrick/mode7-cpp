#ifndef VEC2_H
#define VEC2_H

typedef struct _vec2
{
    float x;
    float y;
} vec2;

vec2 vec2_add(vec2, vec2);
vec2 vec2_sub(vec2, vec2);
vec2 vec2_scale(float, vec2);
vec2 vec2_rotate(vec2, float);
float vec2_dot(vec2, vec2);
float vec2_cross(vec2, vec2);
float vec2_mag(vec2);
void vec2_print(vec2);
vec2 vec2_norm(vec2);

#endif /* VEC2_H */