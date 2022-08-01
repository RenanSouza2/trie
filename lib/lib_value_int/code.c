#include <stdio.h>

#include "header.h"
#include "../lib_trie_base/header.h"

// STRUCT(value_info)
// {
//     int size;
//     value_p null;

//     void_value  value_print;
//     int_value   value_is_null;
// };

#define SIZE 4

void int_value_print(value_p value)
{
    printf("%d", *(int*)value);
}

int int_value_is_null(value_p value)
{
    return *(int*)value == 0;
}

value_info_p get_value_info()
{
    int *null = malloc(sizeof(int));
    *null = 0;

    value_info_p vi = malloc(sizeof(value_info_t));
    *vi = (value_info_t){SIZE, null, int_value_print, int_value_is_null};
    return vi;
}
