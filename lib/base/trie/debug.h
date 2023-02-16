#ifndef __TRI_DEBUG_H__
#define __TRI_DEBUG_H__

#include "header.h"
#include "../header/trie.h"
#include "../header/pointer.h"
#include "../header/value.h"

#define PI ti->pi

STRUCT(trie)
{
    int type, connected;
};

#define FORK 0
#define PATH 1
#define LEAF 2

#define HP(POINTER) ((char*)(((trie_p)(POINTER))+1))
#define FN(POINTER, INDEX) ((pointer_p)(HP(POINTER) + (INDEX) * PI->size))
#define PS(POINTER) ((string_p)(HP(POINTER) + PI->size))
#define PN(POINTER) ((pointer_p)HP(POINTER))
#define LV(POINTER) ((handler_p)HP(POINTER))


#ifdef DEBUG

#define DEF_H(TYPE) \
    extern int TYPE##_created; \
    extern int TYPE##_freed;

DEF_H(value);
DEF_H(pointer);
DEF_H(fork);
DEF_H(path);
DEF_H(leaf);

int decrease(int created, int freed);
void assert_memory();

#define INC(INT) INT##_created++
#define DEC(INT) INT##_freed = decrease(INT##_created, INT##_freed)
#else
#define INC(INT)
#define DEC(INT)
#endif

pointer_p pointer_copy(pointer_info_p pi, pointer_p p);

pointer_p trie_fork_create(trie_info_p ti, char key, pointer_p tp_next);
pointer_p trie_path_create_force(trie_info_p ti, char len, char arr[], pointer_p tp_next);
pointer_p trie_path_create(trie_info_p ti, char len, char arr[], pointer_p tp_next);
pointer_p trie_leaf_create(trie_info_p ti, value_p value);

pointer_p trie_fork_disconnect(trie_info_p ti, pointer_p tp, int key);
pointer_p trie_fork_connect(trie_info_p ti, pointer_p tp, int key, pointer_p tp_next);
pointer_p trie_path_connect(trie_info_p ti, pointer_p tp, pointer_p tp_next);
pointer_p trie_leaf_set_value(trie_info_p ti, pointer_p tp, value_p value);

int trie_joinnable(trie_p t);
pointer_p trie_path_break(trie_info_p ti, pointer_p tp, int len);
pointer_p trie_fork_convert(trie_info_p ti, pointer_p tp);
pointer_p trie_join(trie_info_p ti, pointer_p tp1, pointer_p tp2);

pointer_p trie_delete(trie_info_p ti, pointer_p tp, char arr[]);
pointer_p trie_insert(trie_info_p ti, pointer_p tp, char len, char arr[], value_p value);
handler_p trie_querie(trie_info_p ti, pointer_p tp, char arr[]);
void trie_free(trie_info_p ti, pointer_p tp);

trie_info_p get_trie_info(int max, pointer_info_p pi);


#endif