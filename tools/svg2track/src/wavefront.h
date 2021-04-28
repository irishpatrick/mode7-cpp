#ifndef WAVEFRONT_H
#define WAVEFRONT_H

#include "mesh.h"

void wavefront_save(mesh*, const char*);
void wavefront_load(mesh*, const char*);

#endif /* WAVEFRONT */