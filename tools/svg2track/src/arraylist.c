/**
 * This file is part of mode7-cpp.
 * Copyright (C) 2021  Patrick Roche
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
**/

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

