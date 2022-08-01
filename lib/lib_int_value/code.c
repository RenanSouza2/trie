#include <stdio.h>
#include <stdlib.h>

#include "header.h"

#define SIZE 4

void int_value_print(value_p value)
{
    printf("%d", *(int*)value);
}

int int_value_is_null(value_p value)
{
    return *(int*)value == 0;
} 

value_info_p int_value_info()
{
    int *null = malloc(sizeof(int));
    *null = 0;

    value_info_p vi = malloc(sizeof(value_info_t));
    *vi = (value_info_t){SIZE, (value_p)null, int_value_print, int_value_is_null};
    return vi;
}
