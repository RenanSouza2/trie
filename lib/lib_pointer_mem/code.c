#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../lib_base_header/pointer.h"

#ifdef DEBUG

int decrease(int created, int freed);

extern int pointer_created;
extern int pointer_free;

extern int fork_created;
extern int fork_free;

extern int path_created;
extern int path_free;

extern int leaf_created;
extern int leaf_free;

#define INC(INT) INT##_created++;
#define DEC(INT) INT##_free = decrease(INT##_created, INT##_free)
#else
#define INC(INT)
#define DEC(INT)
#endif

handler_p mem_get(pointer_p p)
{
    return *(handler_p*)p;
}

void mem_display(pointer_p p)
{
    handler_p h = mem_get(p);
    if(h == NULL)   printf("NULL");
    else            printf("%p", h);
}

int mem_is_null(pointer_p p)
{
    return mem_get(p) == NULL;
}

pointer_p mem_set(handler_p h, int size)
{
    pointer_p p = malloc(sizeof(handler_p));
    assert(p);
    INC(pointer);

    *(handler_p*)p = h;
    return p;
}

void mem_free(pointer_p p)
{
    handler_p h = mem_get(p);

#ifdef DEBUG
    switch (*(int*)h)
    {
        case 0: DEC(fork); break;
        case 1: DEC(path); break;
        case 2: DEC(leaf); break;
    }
#endif

    free(h);
    free(p);
    DEC(pointer);
}

void mem_replace(pointer_p p, handler_p h, int size)
{
    *(handler_p*)p = h;
}

pointer_info_p get_mem_info()
{
    pointer_info_p pi = malloc(sizeof(pointer_info_t));
    assert(pi);

    *pi = (pointer_info_t) {
        sizeof(void*),
        mem_set(NULL, 0),

        mem_display,
        mem_is_null,

        mem_get,
        mem_set,
        mem_replace,
        mem_free
    };

#ifdef DEBUG
    pointer_created = 0;
#endif

    return pi;
}
