#ifndef __LIB_TRIE_H__
#define __LIB_TRIE_H__

#include "../struct.h"

PLACEHOLDER(trie);
PLACEHOLDER(value);
PLACEHOLDER(value_info);

PLACEHOLDER(pointer);
PLACEHOLDER(trie_info);

void trie_delete(trie_info_p ti, pointer_p *tp, char arr[]);
void trie_insert(trie_info_p ti, pointer_p *t, char arr[], value_p value);

void trie_display(trie_info_p ti, pointer_p tp);
value_p trie_querie(trie_info_p ti, pointer_p tp, char arr[]);

void trie_free(trie_info_p ti, pointer_p tp);

#endif