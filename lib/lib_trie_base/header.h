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

STRUCT(trie_pointer);

typedef void(*void_pointer)(trie_pointer_p);
typedef int(*int_pointer)(trie_pointer_p);
typedef trie_p(*trie_pointer)(trie_pointer_p);
typedef trie_pointer_p(*pointer_pointer_2)(trie_pointer_p,trie_pointer_p);
typedef trie_pointer_p(*pointer_pointer_int)(trie_pointer_p,int);
typedef trie_pointer_p(*pointer_pointer_int_pointer)(trie_pointer_p,int,trie_pointer_p);
typedef trie_pointer_p(*pointer_char_charv_pointer)(char,char[],trie_pointer_p);
typedef trie_pointer_p(*pointer_value)(value_p);
typedef int(*int_trie)(trie_p);


STRUCT(trie_info)
{
    int max, len;
    int pointer_size;
    int next_size;
    trie_pointer_p null;

    trie_pointer                get_trie;
    int_pointer                 pointer_is_null;
    void_pointer                pointer_display;
    pointer_pointer_int_pointer trie_fork_connect;
    pointer_pointer_2           trie_path_connect;
    pointer_pointer_int         trie_fork_disconnect;
    pointer_pointer_int         trie_fork_create;
    pointer_char_charv_pointer  trie_path_create_force;
    pointer_char_charv_pointer  trie_path_create;
    pointer_value               trie_leaf_create;
    void_pointer                trie_free_single;
    void_pointer                trie_free;
    int_trie                    trie_fork_first_key;

    value_info_t vi;
};

void trie_delete(value_info_p vi, trie_p *t, char arr[]);
void trie_insert(value_info_p vi, trie_p *t, char arr[], value_p value);

void trie_display(trie_info_p ti, value_info_p vi, trie_pointer_p p);
value_p trie_querie(trie_p t, char arr[]);

void trie_free(trie_p t);

#endif