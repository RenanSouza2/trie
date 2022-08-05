#ifndef __BASE_TRIE_H__
#define __BASE_TRIE_H__

#include "../struct.h"

PLACEHOLDER(trie);
PLACEHOLDER(trie_pointer);
PLACEHOLDER(value_info);

typedef int(*int_pointer_f)(trie_pointer_p);
typedef void(*void_pointer_f)(trie_pointer_p);
typedef trie_p(*trie_pointer_f)(trie_pointer_p);
typedef trie_pointer_p(*pointer_trie_f)(trie_p);
typedef void(*void_pointer_trie_f)(trie_pointer_p,trie_p);

STRUCT(trie_info)
{
    int max, len;
    int pointer_size, fork_size, path_size, leaf_size;
    trie_pointer_p null;

    trie_pointer_f      get_trie;
    pointer_trie_f      pointer_create;
    int_pointer_f       pointer_is_null;
    void_pointer_f      pointer_display;
    void_pointer_trie_f trie_replace;
    void_pointer_f      trie_free_single;

    value_info_p vi;
};

#endif