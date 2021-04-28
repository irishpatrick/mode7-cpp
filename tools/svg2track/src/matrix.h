#ifndef MATRIX_H
#define MATRIX_H

typedef float mat4[4][4];

void eye4(mat4);
void translate4(mat4, float, float, float);
void rotate4(mat4, float, float, float);
void mul4(mat4, mat4, mat4);

#endif /* MATRIX_H */