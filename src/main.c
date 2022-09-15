#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../lib/base/trie/header.h"
#include "../lib/value/int/header.h"
#include "../lib/pointer/mem/header.h"
#include "../lib/base/lib_trie_info/header.h"

int main()
{
    setbuf(stdout, NULL);
    
    value_info_p vi = get_int_value_info();
    pointer_info_p pi = get_mem_info();
    trie_info_p ti = get_trie_info(10, vi, pi);
    root_p r = root_init(ti, 8);

    char arr[] = {1, 2, 3, 4, 5, 6, 7, 8};
    value_p value = set_int(1);
    root_insert(r, arr, value);
    
    arr[2] = 0;
    value = set_int(2);
    root_insert(r, arr, value);
    
    arr[4] = 0;
    value = set_int(3);
    root_insert(r, arr, value);
    
    arr[2] = 3;
    value = set_int(4);
    root_insert(r, arr, value);

    root_display(r);
    
    printf("\n");
    return 0;
}