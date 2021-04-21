#include "util.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <string.h>

inline int startswith(char* ref, char* pat)
{
    if (strlen(ref) < strlen(pat))
    {
        return 0;
    }

    char* ref_p = ref;
    char* pat_p = pat;
    while (*pat_p != 0)
    {
        if (*ref_p != *pat_p)
        {
            return 0;
        }

        ++ref_p;
        ++pat_p;
    }

    return 1;
}

inline char* skip(char* line, int amt)
{
    line += 3;
    while (*line == ' ' || *line == '\t')
    {
        ++line;
    }
    return line;
}

inline float d2r(float deg)
{
    return deg * M_PI / 180.f;
}

inline float r2d(float rad)
{
    return rad * 180.f / M_PI;
}