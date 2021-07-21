#include "assetpack.h"
#include "wavefront.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <zip.h>

static bool endswith(const char* str, const char* pat)
{
    uint32_t str_len = strlen(str);
    uint32_t pat_len = strlen(pat);

    if (pat_len > str_len)
    {
        return false;
    }

    const char* start = str + (str_len - pat_len);

    return strcmp(start, pat) == 0;
}

int assetpack_open(assetpack* ap, const char* fn)
{
    zip_error_t ze;
    int err = 0;
    zip_t* archive;
    zip_file_t* obj_file;

    ap->mesh = NULL;
    ap->name = NULL;

    archive = zip_open(fn, ZIP_RDONLY, &err);
    if (err)
    {
        zip_error_init_with_code(&ze, err);
        fprintf(stderr, "zip error: %s\n", zip_error_strerror(&ze));
        return 1;
    }

    int n_entries = zip_get_num_entries(archive, 0);
    if (n_entries < 1)
    {
        fprintf(stderr, "zip error: no files in archive\n");
        zip_close(archive);
        return 1;
    }

    const char* indx_name;
    uint64_t obj_file_indx = UINT64_MAX;
    for (uint64_t i = 0; i < n_entries; ++i)
    {
        indx_name = zip_get_name(archive, i, ZIP_FL_ENC_GUESS); // guess encoding
        if (endswith(indx_name, ".obj"))
        {
            obj_file_indx = i;
        }
    }

    // read .obj file
    obj_file = zip_fopen_index(archive, obj_file_indx, ZIP_FL_UNCHANGED);
    if (!obj_file)
    {
        fprintf(stderr, "zip error: slice.obj does not exist in the archive\n");
        zip_close(archive);
        return 1;
    }

    zip_stat_t obj_file_stat;
    err = zip_stat_index(archive, obj_file_indx, 0, &obj_file_stat);
    int64_t obj_file_size = obj_file_stat.size;

    if (err)
    {
        fprintf(stderr, "fatal error\n");
        return 1;
    }
    if (obj_file_size == 0)
    {
        fprintf(stderr, "obj file is empty\n");
        return 1;
    }
    if (obj_file_size < 0)
    {
        fprintf(stderr, "fatal error\n");
        return 1;
    }

    FILE* tmp = fopen(".transfer.obj.tmp", "wb");
    if (!tmp)
    {
        fprintf(stderr, "io error: cannot open temporary file for writing\n");
        zip_fclose(obj_file);
        zip_close(archive);
        return 1;
    }

    size_t buf_size = 1024;
    char* buf = malloc(buf_size);
    if (!buf)
    {
        fprintf(stderr, "memory error\n");
        fclose(tmp);
        zip_fclose(obj_file);
        zip_close(archive);
        return 1;
    }

    uint32_t iterations = obj_file_size / buf_size + ((obj_file_size % buf_size) > 0);
    if (iterations < 1)
    {
        fprintf(stderr, "unable to process obj file\n");
        fclose(tmp);
        zip_fclose(obj_file);
        zip_close(archive);
        return 1;
    }
    
    uint64_t n_read;
    for (uint32_t i = 0; i <= iterations; ++i)
    {
        n_read = zip_fread(obj_file, buf, buf_size);
        fwrite(buf, sizeof(char), n_read, tmp);
    }

    fclose(tmp);
    zip_fclose(obj_file);
    zip_close(archive);

    wavefront_load(&ap->mesh, ".transfer.obj.tmp");

    remove(".transfer.obj.tmp");

    return 0;
}

void assetpack_free(assetpack* ap)
{
    if (ap->name)
    {
        free(ap->name);
        ap->name = NULL;
    }
    if (ap->mesh)
    {
        mesh_destroy(ap->mesh);
        ap->mesh = NULL;
    }
}