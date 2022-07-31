#include <stdio.h>

#include "../lib_trie_base/header.h"
#include "../utils/header.h"

// STRUCT(trie_info)
// {
//     int max, len;
//     int pointer_size;
//     trie_pointer_p null;
//
//     trie_pointer                get_trie;
//     int_pointer                 pointer_is_null;
//     void_pointer                pointer_display;
//     pointer_pointer_int_pointer trie_fork_connect;
//     pointer_pointer_2           trie_path_connect;
//     pointer_pointer_int         trie_fork_disconnect;
//     pointer_pointer_int         trie_fork_create;
//     pointer_char_charv_pointer  trie_path_create_force;
//     pointer_char_charv_pointer  trie_path_create;
//     pointer_value               trie_leaf_create;
//     void_pointer                trie_free_single;
//     void_pointer                trie_free;
//     int_trie                    trie_fork_first_key;
//     void_pointer                pointer_free;
//
//     value_info_t vi;
// };

trie_p mem_get_trie(trie_pointer_p tp)
{
    return *(trie_p*)tp;
}

int mem_pointer_is_null(trie_pointer_p tp)
{
    trie_p t = mem_get_trie(tp);
    return t == NULL;
}

void mem_trie_pointer_display(trie_pointer_p tp)
{
    trie_p t = mem_get_trie(tp);
    mem_pointer_display(t);
}

//     trie_fork_connect;
//     trie_path_connect;
//     trie_fork_disconnect;
//     trie_fork_create;
//     trie_path_create_force;
//     trie_path_create;
//     trie_leaf_create;
//     trie_free_single;
//     trie_free;
//     trie_fork_first_key;
//     pointer_free;