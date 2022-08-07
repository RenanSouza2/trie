#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../lib/lib_base_trie/header.h"
#include "../lib/lib_value_int/header.h"
#include "../lib/lib_pointer_mem/header.h"
#include "../lib/lib_trie_info/header.h"

int main()
{
    setbuf(stdout, NULL);
    
    value_info_p vi = get_int_value_info();
    pointer_info_p pi = get_mem_info();
    trie_info_p ti = get_trie_info(10, 8, vi, pi);
    pointer_p tp = NULL;

    char arr[] = {1, 2, 3, 4, 5, 6, 7, 8};
    value_p value = set_int(1);
    trie_insert(ti, &tp, arr, value);
    
    arr[2] = 0;
    value = set_int(2);
    trie_insert(ti, &tp, arr, value);
    
    arr[4] = 0;
    value = set_int(3);
    trie_insert(ti, &tp, arr, value);
    
    arr[2] = 3;
    value = set_int(4);
    trie_insert(ti, &tp, arr, value);

    trie_display(ti, tp);
    
    printf("\n");
    return 0;
}