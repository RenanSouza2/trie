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

STRUCT(abs_value)
{
    int size;
    void_value value_print;
};

void trie_delete(abs_value_p value_info, trie_p *t, char arr[]);
void trie_insert(abs_value_p value_info, trie_p *t, char arr[], value_p value);

void trie_display(trie_p t);
value_p trie_querie(trie_p t, char arr[]);

#endif