#ifndef __LIB_TRIE_H__
#define __LIB_TRIE_H__

#include "../../utils/struct.h"

PLACEHOLDER(trie);
PLACEHOLDER(value);
PLACEHOLDER(value_info);

PLACEHOLDER(pointer);
PLACEHOLDER(pointer_info);
PLACEHOLDER(trie_info);

STRUCT(root)
{
    int len;
    trie_info_p ti;
    pointer_p tp;
};

root_p root_init(pointer_info_p pi, int len, int max);

void root_delete(root_p r, char const arr[]);
void root_insert(root_p r, char const arr[], value_p value);

void root_display(root_p r, handler_p print);
handler_p root_querie(root_p r, char const arr[]);

void root_free(root_p r);

#endif
