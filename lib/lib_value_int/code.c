#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "header.h"
#include "../lib_base_header/value.h"
#include "../lib_trie_base/header.h"

void value_print(value_p value)
{
    printf("%d", *(int*)value);
}

value_p set_int(int value)
{
    value_p vp = malloc(sizeof(int));
    assert(value);

    *(int*)vp = value;
    return vp;
}

int get_int(value_p value)
{
    return (value == NULL) ? 0 : *(int*)value;
}

int get_int_size(value_p value)
{
    return 4;
}

value_info_p get_int_value_info()
{
    value_info_p vi = malloc(sizeof(value_info_p));
    assert(vi);

    *vi = (value_info_t){
        get_int_size, 
        value_print
    };
    return vi;
}