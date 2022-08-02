#ifndef __MEM_INT_TRIE_H__
#define __MEM_INT_TRIE_H__

#include "../lib_base_trie/header.h"

trie_pointer_p mem_pointer_create(trie_p t);

void mem_int_trie_insert(trie_pointer_p *tp, char arr[], int value);
void mem_int_trie_display(trie_pointer_p p);
int  mem_int_trie_querie(trie_pointer_p tp, char len, char arr[]);
void mem_int_trie_free(trie_pointer_p tp);

#endif