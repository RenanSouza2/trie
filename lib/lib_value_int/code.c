#include <stdio.h>

#include "header.h"
#include "../lib_trie_base/header.h"

void value_print(value_p value)
{
    printf("%d", *(int*)value);
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