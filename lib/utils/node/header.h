#ifndef __UTILS_TRIE_H__
#define __UTILS_TRIE_H__

#include "../struct.h"
#include "../../base/header/trie.h"
#include "../../base/header/value.h"
#include "../../base/header/pointer.h"

#define FORK 0
#define PATH 1
#define LEAF 2

#define PI ti->pi
#define VI ti->vi

#define HP(POINTER) ((char*)(((trie_p)(POINTER))+1))
#define FN(POINTER, INDEX) ((pointer_p)(HP(POINTER) + (INDEX) * PI->size))
#define PS(POINTER) ((string_p)(HP(POINTER) + PI->size))
#define PN(POINTER) ((pointer_p)HP(POINTER))
#define LV(POINTER) ((value_p)HP(POINTER))


#define PTR_CPY(POINTER1, POINTER2) memcpy(POINTER1, POINTER2, PI->size)

#define FORK_SIZE (sizeof(trie_t) + ti->max * PI->size)
#define PATH_SIZE(LENGTH) (sizeof(trie_t) + PI->size + string_size(LENGTH))
#define LEAF_SIZE(SIZE) (sizeof(trie_t) + SIZE)

PLACEHOLDER(trie_info);
PLACEHOLDER(pointer);

STRUCT(trie)
{
    int type, connected;
};

trie_p trie_fork_set(trie_info_p ti, int key, pointer_p tp_next);
trie_p trie_path_set(trie_info_p ti, char len, char arr[], pointer_p tp_next);
trie_p trie_leaf_set(trie_info_p ti, value_p value);

#endif