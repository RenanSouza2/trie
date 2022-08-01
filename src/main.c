#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../lib/lib_mem_int_trie/header.h"

int main()
{
    setbuf(stdout, NULL);

    trie_pointer_p tp = NULL;

    char arr[] = {1, 2, 3, 4, 5, 6, 7, 8};
    mem_int_trie_insert(&tp, arr, 1);
    
    arr[2] = 0;
    mem_int_trie_insert(&tp, arr, 2);
    
    arr[4] = 0;
    mem_int_trie_insert(&tp, arr, 3);
    
    arr[2] = 3;
    mem_int_trie_insert(&tp, arr, 4);

    mem_int_trie_display(tp);
    
    printf("\n");
    return 0;
}