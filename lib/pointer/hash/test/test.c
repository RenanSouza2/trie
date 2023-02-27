#include <stdio.h>
#include <string.h>

#include "../header.h"
#include "../../../base/trie/header.h"
#include "../../../value/int/header.h"

void test_insert()
{
    printf("\n\ttest insert\t\t");

    pointer_info_p pi = get_hash_info(); 
    root_p r = root_init(pi, 64, 16);

    root_display(r, int_print);

    char path[64];
    memset(path, 0, 64);

    value_p v = set_int(1);
    root_insert(r, path, v);
    root_display(r, int_print);
}

void test_hash()
{
    printf("\ntest hash\t\t");

    test_insert();
}

int main()
{
    setbuf(stdout, NULL);
    test_hash();
    printf("\n\tTest successful\n\n");
    return 0;
}