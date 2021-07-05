#ifndef LINE_H
#define LINE_H

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
float line_calc_distance(line*);
void line_print(line*);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* LINE_H */
