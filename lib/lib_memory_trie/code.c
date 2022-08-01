#include <stdio.h>
#include <assert.h>

#include "../lib_trie_base/header.h"
#include "../utils/header.h"

// STRUCT(trie_info)
// {
//     int max, len;
//     int pointer_size, fork_size;
//     trie_pointer_p null;
//
//     trie_pointer                get_trie;
//     pointer_trie                get_pointer;
//     int_pointer                 pointer_is_null;
//     void_pointer                pointer_display;
//     pointer_pointer_int_pointer trie_fork_connect;
//     pointer_pointer_2           trie_path_connect;
//     pointer_pointer_int         trie_fork_disconnect;
//     pointer_pointer_value       trie_leaf_set_value;
//     void_pointer                trie_free_single;
//
//     value_info_p vi;
// };



#define MTF(POINTER) ((mem_trie_fork_p)(POINTER))
#define MTP(POINTER) ((mem_path_fork_p)(POINTER))

#define MAX 10

STRUCT(mem_trie_fork)
{
    trie_fork_t t;
    trie_pointer_p next[MAX];
};

STRUCT(mem_path_fork)
{
    trie_fork_t t;
    trie_pointer_p next;
};

trie_p mem_get_trie(trie_pointer_p tp)
{
    return T(tp);
}

trie_pointer_p mem_get_pointer(trie_p t)
{
    return (trie_pointer_p)t;
}

int mem_pointer_is_null(trie_pointer_p tp)
{
    return tp == NULL;
}

void mem_trie_pointer_display(trie_pointer_p tp)
{
    trie_p t = mem_get_trie(tp);
    mem_pointer_display(t);
}

trie_pointer_p mem_trie_fork_disconnect(trie_pointer_p tp, int key)
{
    if(MTF(tp)->next[key] == NULL) return tp;

    if(TF(tp)->connected == 1)
    {
        free(tp);
        return NULL;
    }

    (TF(tp)->connected)--;
    return tp;
}

trie_pointer_p mem_trie_fork_connect(trie_pointer_p tp, int key, trie_pointer_p tp_next)
{
    if(tp_next == NULL) return mem_trie_fork_disconnect(tp, key);

    if(MTF(tp)->next[key] == NULL) (TF(tp)->connected)++;

    MTF(tp)->next[key] = tp_next;
    return tp;
}

trie_pointer_p mem_trie_path_connect(trie_pointer_p tp, trie_pointer_p tp_next)
{
    MTP(tp)->next = tp_next;
    return tp;
}

trie_pointer_p mem_trie_leaf_set_value(value_info_p vi, trie_pointer_p tp, value_p value)
{
    memcpy(LV(tp), value, vi->size);
    return tp;
}

//     pointer_pointer_value       trie_leaf_set_value;
//     trie_free_single;