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
    
    pointer_info_p pi = get_mem_info();
    trie_info_p ti = get_trie_info(10, pi);
    root_p r = root_init(ti, 8);

    value_p value = set_int(1);
    root_insert(r, "12345678", value);
    
    value = set_int(2);
    root_insert(r, "12045678", value);
    
    value = set_int(3);
    root_insert(r, "12040678", value);
    
    value = set_int(4);
    root_insert(r, "12340678", value);

    root_display(r, int_print);
    
    printf("\n");
    return 0;
}