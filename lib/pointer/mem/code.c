#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../../base/header/pointer.h"

#ifdef DEBUG

extern int pointer_created;
extern int pointer_freed;

extern int fork_created;
extern int fork_freed;

extern int path_created;
extern int path_freed;

extern int leaf_created;
extern int leaf_freed;

#define INC(INT) INT##_created++;
#define DEC(INT) \
    {   \
        assert(INT##_created > INT##_freed); \
        INT##_freed++; \
    }
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

pointer_info_p get_mem_info()
{
    static pointer_info_p pi = NULL;
    if(pi) return pi;

    pi = malloc(sizeof(pointer_info_t));
    assert(pi);

    *pi = (pointer_info_t) {
        sizeof(void*),

        mem_display,

        mem_get,
        mem_set,
        mem_free
    };

    return pi;
}
