#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include "vec2.h"
#include "line.h"
#include "util.h"

static int num_lines;
static int buffer_size;
static float* buffer;

float get_angle(Line l)
{
    float theta = atan2f(l.v.y, l.v.x);
    if (theta < 0.f)
    {
        theta += 2 * M_PI;
    }
    return theta;
}

void print_usage()
{
    printf("usage: ./tasm <file> [output]\n");
}

Line buffer_get_last()
{
    Line out;
    float* ptr = buffer + ((num_lines - 1) * 4);
    vec2 head = {ptr[0], ptr[1]};
    vec2 tail = {ptr[2], ptr[3]};
    out = line_from_points(head, tail);
    return out;
}

void buffer_append(float x1, float y1, float x2, float y2)
{
    float* end = buffer + (num_lines * 4);
    end[0] = x1;
    end[1] = y1;
    end[2] = x2;
    end[3] = y2;
    ++num_lines;
    if (num_lines * 4 == buffer_size / sizeof(float))
    {
        buffer_size += buffer_size / 2;
        buffer = (float*)realloc(buffer, buffer_size);
    }
}

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        print_usage();
        return 0;
    }

    FILE* in = fopen(argv[1], "r");
    if (!in)
    {
        printf("cannot open %s\n", argv[1]);
        return 1;
    }

    buffer_size = 100 * 4 * sizeof(float);
    buffer = (float*)malloc(buffer_size);


    char* line = NULL;
    size_t len = 0;
    ssize_t read;
    vec2 last;
    while ((read = getline(&line, &len, in)) != -1)
    {
        if (startswith(line, "beg"))
        {
            char* start = skip(line, 3);
            sscanf(start, "%f,%f", &last.x, &last.y);
        }
        if (startswith(line, "lin"))
        {
            char* start = skip(line, 3);
            float x2, y2;
            sscanf(start, "%f,%f", &x2, &y2);
            buffer_append(last.x, last.y, x2, y2);
            last.x = x2;
            last.y = y2;
        }
        else if (startswith(line, "arc"))
        {
            char* start = skip(line, 3);

            float radius, degs;
            int dir;
            sscanf(start, "%f,%f,%d", &radius, &degs, &dir);
            
            Line last_line = buffer_get_last();
            vec2 norm = vec2_norm(line_normal(last_line));
            float theta_0 = get_angle(last_line);
            if (dir)
            {
                norm = vec2_scale(-1, norm);
                theta_0 += M_PI / 2.f;
            }
            else
            {
                theta_0 -= M_PI / 2.f;
            }
            vec2 center = vec2_add(last_line.e, vec2_scale(radius, norm));
            int steps = degs / 5;
            float part = d2r(degs) / (float)steps;
            vec2 prev = last;
            vec2 cur;
            for (int i = 0; i < steps + 1; ++i)
            {
                float theta = theta_0 + part * (float)i;
                cur.x = radius * cosf(theta);
                cur.y = radius * sinf(theta);
                cur = vec2_add(center, cur);
                buffer_append(prev.x, prev.y, cur.x, cur.y);
                prev = cur;
            }
            last = cur;
        }
    }
    
    fclose(in);
    if (line)
    {
        free(line);
    }

    FILE* out;
    if (argc == 3)
    {
        out = fopen(argv[2], "w");
    }
    else
    {
        out = fopen("a.line", "w");
    }
    for (int i = 0; i < num_lines * 4; i += 4)
    {
        fprintf(out, "%f,%f,%f,%f\n",
            buffer[i + 0],
            buffer[i + 1],
            buffer[i + 2],
            buffer[i + 3]
        );
    }
    fclose(out);

    return 0;
}