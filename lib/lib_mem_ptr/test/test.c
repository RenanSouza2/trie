#include <stdio.h>
#include <assert.h>

#include "../header.h"

int main()
{
    trie_info_p ti = mem_trie_info(NULL);

    printf("\nmax: %d", ti->max);

    printf("\n");
    return 0;
}