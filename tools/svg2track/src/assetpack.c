#include "assetpack.h"

#include <stdlib.h>
#include <stdio.h>
#include <zip.h>

int assetpack_open(assetpack* ap, const char* fn)
{
    zip_error_t ze;
    zip_t* archive;
    zip_file_t* obj_file;

    archive = zip_open(fn, ZIP_RDONLY, &ze);
    if (zip_error_strerror(&ze))
    {
        fprintf(stderr, "zip error: %s\n", zip_error_strerror(&ze));
        return 1;
    }

    int n_entries = zip_get_num_entries(archive, 0);
    if (n_entries < 1)
    {
        fprintf(stderr, "zip error: no files in archive");
        return 1;
    }

    // read .obj file
    obj_file = zip_fopen(archive, "slice.obj", 0);
    if (!obj_file)
    {
        fprintf(stderr, "zip error: slice.obj does not exist in the archive\n");
        return 1;
    }
}