#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../lib/lib_base_trie/header.h"
#include "../lib/lib_value_int/header.h"

int main()
{
    setbuf(stdout, NULL);
    
    value_info_p vi = get_int_value_info();
    trie_p t = NULL;

    char arr[] = {1, 2, 3, 4, 5, 6, 7, 8};
    value_p value = set_int(1);
    trie_insert(vi, &t, arr, value);
    
    arr[2] = 0;
    value = set_int(2);
    trie_insert(vi, &t, arr, value);
    
    arr[4] = 0;
    value = set_int(3);
    trie_insert(vi, &t, arr, value);
    
    arr[2] = 3;
    value = set_int(4);
    trie_insert(vi, &t, arr, value);

    trie_display(vi, t);
    
    printf("\n");
    return 0;
}