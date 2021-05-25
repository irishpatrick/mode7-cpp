#include "arraylist.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define AL_DEFAULT_SIZE 100


int al_init(arraylist* al)
{
    return al_reserve(al, AL_DEFAULT_SIZE); 
}

int al_reserve(arraylist* al, unsigned long sz)
{
    al->length = 0;
    al->max = sz;
    al->array = malloc(al->max * sizeof(void*));
    if (!al->array)
    {
        return 1;
    }

    return 0;
}

int al_resize(arraylist* al, unsigned long sz)
{
    if (al->max >= sz)
    {
        return 1;
    }
    al->max = sz;
    void** newptr = realloc(al->array, al->max * sizeof(void*));
    if (!newptr)
    {
        return 1;
    }
    al->array = newptr;

    return 0;
}

void* al_get(arraylist* al, unsigned long indx)
{
    if (indx >= al->length)
    {
        return NULL;
    }

    return al->array[indx];
}

void** al_at(arraylist* al, unsigned long indx)
{
    if (indx >= al->length)
    {
        return NULL;
    }

    return &al->array[indx];
}

int al_free(arraylist* al)
{
    free(al->array);
    al->length = 0;
    al->max = 0;

    return 0;
}

