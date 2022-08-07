#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../lib_base_header/pointer.h"

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

    *(handler_p*)p = h;
    return p;
}

void mem_delete(pointer_p p)
{
    handler_p h = mem_get(p);
    free(h);
    free(p);
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
        mem_delete
    };
    return pi;
}
