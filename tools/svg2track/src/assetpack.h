#ifndef ASSETPACK_H
#define ASSETPACK_H

#include "mesh.h"

typedef struct _assetpack
{
    char* name;
    mesh* mesh;
} assetpack;

int assetpack_open(assetpack*, const char*);
void assetpack_free(assetpack*);

#endif /* ASSETPACK_H */