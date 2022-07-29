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

typedef trie_p(*trie_pointer)(pointer_p);
typedef int(*int_pointer)(pointer_p);

STRUCT(trie_info)
{
    int max, len;
    int pointer_size;
    int next_size;
    trie_pointer get_trie;
    int_pointer pointer_is_null;
    value_info_t vi;
};

void trie_delete(value_info_p vi, trie_p *t, char arr[]);
void trie_insert(value_info_p vi, trie_p *t, char arr[], value_p value);

void trie_display(trie_info_p ti, value_info_p vi, pointer_p p);
value_p trie_querie(trie_p t, char arr[]);

void trie_free(trie_p t);

#endif