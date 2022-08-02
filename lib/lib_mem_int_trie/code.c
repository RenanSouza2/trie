#include <stdio.h>

#include "../lib_int_value/header.h"
#include "../lib_mem_ptr/header.h"

trie_info_p miti = NULL;

trie_info_p get_mem_int_trie_info()
{
    if(miti != NULL) return miti;

    value_info_p ivi = int_value_info();
    return miti = mem_trie_info(ivi);
}


void mem_int_trie_delete(trie_pointer_p *tp, char arr[])
{
    trie_info_p ti = get_mem_int_trie_info();
    trie_delete(ti, tp, arr);
}

void mem_int_trie_insert(trie_pointer_p *tp, char arr[], int value)
{
    trie_info_p ti = get_mem_int_trie_info();
    value_p vp = int_value_create(value);
    trie_insert(ti, tp, arr, vp);
}

void mem_int_trie_display(trie_pointer_p tp)
{
    trie_info_p ti = get_mem_int_trie_info();
    trie_display(ti, tp);
}

int mem_int_trie_querie(trie_pointer_p tp, char len, char arr[])
{
    trie_info_p ti = get_mem_int_trie_info();
    value_p value = trie_querie(ti, tp, arr);
    return *(int*)value;
}

void mem_int_trie_free(trie_pointer_p tp)
{
    trie_info_p ti = get_mem_int_trie_info();
    trie_free(ti, tp);
}
