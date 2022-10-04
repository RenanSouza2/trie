#include "../header.h"
#include "../../../base/trie/header.h"
#include "../../../value/int/header.h"

int main()
{
    pointer_info_p pi = get_hash_info(); 
    root_p r = root_init(pi, 64, 16);

    root_display(r, int_print);

    char path[64] = "test";


    value_p v = set_int(1);
    root_insert(r, path, v);
}