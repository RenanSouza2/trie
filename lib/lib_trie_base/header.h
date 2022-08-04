#ifndef __LIB_TRIE_H__
#define __LIB_TRIE_H__

#include "../lib_base_header/value.h"

PLACEHOLDER(trie);

void trie_delete(value_info_p vi, trie_p *t, char arr[]);
void trie_insert(value_info_p vi, trie_p *t, char arr[], value_p value);

void trie_display(value_info_p vi, trie_p t);
value_p trie_querie(trie_p t, char arr[]);

void trie_free(trie_p t);

#endif