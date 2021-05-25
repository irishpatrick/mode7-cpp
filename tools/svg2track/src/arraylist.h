#ifndef ARRAYLIST_H
#define ARRAYLIST_H

typedef struct _arraylist
{
    void** array;
    unsigned long length;
    unsigned long max;
} arraylist;

int al_init(arraylist*);
int al_reserve(arraylist*, unsigned long);
int al_resize(arraylist*, unsigned long);
void* al_get(arraylist*, unsigned long);
void** al_at(arraylist*, unsigned long);
int al_free(arraylist*);

#endif /* ARRAYLIST_H */
