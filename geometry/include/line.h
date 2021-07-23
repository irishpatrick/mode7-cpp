#ifndef LINE_H
#define LINE_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

typedef struct _line
{
    float p1[2];
    float p2[2];
} line;

void line_connect(line*, float, float, float, float);
void line_from_buffer(line*, float*);
void line_solve(line*, float, float[2]);
float line_inverse(line*, float, float);
float line_solve_y(line*, float);
float line_calc_distance(line*);
void line_print(line*);
bool line_intersects(line*, line*);
float line_slope(line*);
float line_intercept(line*);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* LINE_H */
