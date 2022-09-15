#include <stdlib.h>
#include <assert.h>

#include "header.h"
#include "../header/trie.h"

trie_info_p get_trie_info(int max, value_info_p vi, pointer_info_p pi)
{
    trie_info_p ti = malloc(sizeof(trie_info_t));
    assert(ti);

    *ti = (trie_info_t) {max, vi, pi};
    return ti;
}
