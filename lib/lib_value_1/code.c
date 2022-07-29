#include <stdio.h>

#include "../lib_trie/header.h"

void value_print(value_p value)
{
    printf("%i", *(int*)value);
}

value_info_t value_int = (value_info_t){4, value_print};

#define VALUE_INT(INT) ((value_p)(&(INT)))