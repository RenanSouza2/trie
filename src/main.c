#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../lib/lib_trie/header.h"

int main()
{
    setbuf(stdout, NULL);
    
    trie_p t = NULL;

    char arr[] = {1, 2, 3, 4, 5, 6, 7, 8};
    trie_insert(&t, arr, 1);
    
    arr[2] = 0;
    trie_insert(&t, arr, 2);
    
    arr[4] = 0;
    trie_insert(&t, arr, 3);
    
    arr[2] = 3;
    trie_insert(&t, arr, 4);

    trie_display(t);
    
    printf("\n");
    return 0;
}