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
    vi = {4, null, int_value_print, int_value_is_null};
    return vi;
}

value_info_t value_int = (value_info_t){4, value_print};


#define VALUE(INT) ((value_p)(&(INT)))

void trie_int_delete(trie_int_p *t, char arr[])
{
    trie_delete(&value_int, (trie_p*)t, arr);
}

void trie_int_insert(trie_int_p *t, char arr[], int value)
{
    trie_insert(&value_int, (trie_p*)t, arr, VALUE(value));
}

void trie_int_display(trie_int_p t)
{
    trie_display(&value_int, (trie_p)t);
}

int  trie_int_querie(trie_int_p t, char arr[])
{
    value_p value = trie_querie((trie_p)t, arr);
    if(value == NULL) return 0;

    int int_value = *(int*)value;
    return int_value;
}

void trie_int_free(trie_int_p t)
{
    trie_free((trie_p)t);
}