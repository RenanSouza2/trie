#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../lib_trie_base/header.h"
#include "../utils/header.h"

#define MTF(POINTER) ((mem_trie_fork_p)(POINTER))
#define MTP(POINTER) ((mem_path_fork_p)(POINTER))

#define MAX 10
#define LEN 8
#define PTR_SIZE 8

#define FORK_SIZE (MAX*PTR_SIZE)

STRUCT(mem_trie_fork)
{
    trie_fork_t t;
    trie_p next[MAX];
};

STRUCT(mem_path_fork)
{
    trie_fork_t t;
    trie_p next;
};

trie_p mem_get_trie(trie_pointer_p tp)
{
    return *(void**)tp;
}

trie_pointer_p mem_get_pointer(trie_p t)
{
    void **tp = calloc(1, sizeof(void*));
    *tp = t;
    return (trie_pointer_p)tp;
}

int mem_pointer_is_null(trie_pointer_p tp)
{
    return mem_get_trie(tp) == NULL;
}

trie_pointer_p mem_trie_fork_disconnect(trie_pointer_p tp, int key)
{
    trie_p t = mem_get_trie(tp);
    if(MTF(t)->next[key] == NULL) return tp;

    if(TF(t)->connected == 1)
    {
        free(t);
        return NULL;
    }

    (TF(t)->connected)--;
    return tp;
}

trie_pointer_p mem_trie_fork_connect(trie_pointer_p tp, int key, trie_pointer_p tp_next)
{
    trie_p t_next = mem_get_trie(tp);
    if(t_next == NULL) return mem_trie_fork_disconnect(tp, key);

    trie_p t = mem_get_trie(tp);
    if(MTF(t)->next[key] == NULL) (TF(tp)->connected)++;

    MTF(t)->next[key] = t_next;
    return tp;
}

#define VOID(POINTER) ((void*)(POINTER))
#define DIFF(POINTER1, POINTER2) ((VOID(POINTER1)) - (VOID(POINTER2)))
#define OFFSET(STR, ARG) (DIFF(&(STR.ARG), &STR))

trie_pointer_p mem_trie_path_connect(trie_pointer_p tp, trie_pointer_p tp_next)
{
    trie_p t = mem_get_trie(tp);
    MTP(t)->next = mem_get_trie(tp_next);
    return tp;
}

trie_pointer_p mem_trie_leaf_set_value(value_info_p vi, trie_pointer_p tp, value_p value)
{
    trie_p t = mem_get_trie(tp);
    memcpy(LV(t), value, vi->size);
    return tp;
}


void mem_trie_free_single(trie_pointer_p tp)
{
    free(tp);
}

trie_info_p mem_trie_info(value_info_p vi)
{
    trie_info_p ti = calloc(1, sizeof(trie_info_t));
    *ti = (trie_info_t) {
        MAX, LEN,
        PTR_SIZE, FORK_SIZE,
        mem_get_pointer(NULL),

        mem_get_trie,
        mem_get_pointer,
        mem_pointer_is_null,
        (void_pointer)mem_pointer_display,
        mem_trie_fork_connect,
        mem_trie_path_connect,
        mem_trie_fork_disconnect,
        mem_trie_leaf_set_value,
        mem_trie_free_single,

        vi
    };
    return ti;
} 