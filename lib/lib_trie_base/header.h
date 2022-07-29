#ifndef LIB_TRIE_H
#define LIB_TRIE_H

#include "../struct.h"
#include "../lib_my_string/header.h"

STRUCT(trie)
{
    int type;
};

STRUCT(value);
typedef void (*void_value)(value_p);

STRUCT(value_info)
{
    int size;
    void_value value_print;
};

STRUCT(pointer);
struct(node);

typedef node_p (*node_pointer)(pointer_p);

STRUCT(trie_info);
{
    int max, len;
    int pointer_size;
    value_info_t vi;
}

void trie_delete(value_info_p vi, trie_p *t, char arr[]);
void trie_insert(value_info_p vi, trie_p *t, char arr[], value_p value);

void trie_display(value_info_p vi, trie_p t);
value_p trie_querie(trie_p t, char arr[]);

void trie_free(trie_p t);

#endif