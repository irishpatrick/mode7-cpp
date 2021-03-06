#include "matrix.h"

#include <math.h>
#include <assert.h>
#include <string.h>

void eye4(mat4 m)
{
    memset(m, 0, 4 * 4 * sizeof(float));
    for (int i = 0; i < 4; ++i)
    {
        m[i][i] = 1.f;
    }
}

void translate4(mat4 m, float x, float y, float z)
{
    mat4 t;

    // build tranlation matrix
    eye4(t);
    t[0][3] = x;
    t[1][3] = y;
    t[2][3] = z;

    // apply translation
    mul4(m, t, m);
}

void rotate4(mat4 m, float x, float y, float z)
{
    mat4 rx;
    mat4 ry;
    mat4 rz;

    eye4(rx);
    rx[1][1] = cos(x);
    rx[1][2] = -sin(x);
    rx[2][1] = sin(x);
    rx[2][2] = cos(x);

    eye4(ry);
    ry[0][0] = cos(y);
    ry[0][2] = sin(y);
    ry[2][0] = -sin(y);
    ry[2][2] = cos(y);
    
    eye4(rz);
    rz[0][0] = cos(z);
    rz[0][1] = -sin(z);
    rz[1][0] = sin(z);
    rz[1][1] = cos(z);

    mul4(m, rx, m);
    mul4(m, ry, m);
    mul4(m, rz, m);
}

void mul4(mat4 a, mat4 b, mat4 c)
{
    mat4 cpy;
    int i;
    int j;
    int k;
    float sum;

    for (i = 0; i < 4; ++i)
    {
        for (j = 0; j < 4; ++j)
        {
            sum = 0;
            for (k = 0; k < 4; ++k)
            {
                sum += a[i][k] * b[k][j];
            }
            cpy[i][j] = sum;
        }
    }

    memcpy(c, cpy, 4 * 4 * sizeof(float));
}