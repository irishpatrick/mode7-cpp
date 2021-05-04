#ifndef WAVEFRONT_H
#define WAVEFRONT_H

#include "mesh.h"

void wavefront_save(mesh*, const char*, int);
int wavefront_load(mesh**, const char*);

#endif /* WAVEFRONT */