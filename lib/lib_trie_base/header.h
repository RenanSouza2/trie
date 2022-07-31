#ifndef LIB_TRIE_H
#define LIB_TRIE_H

#include "../struct.h"
#include "../lib_my_string/header.h"

#define FORK 0
#define PATH 1
#define LEAF 2

#define TF(POINTER) ((trie_fork_p)(POINTER))

STRUCT(trie)
{
    int type;
};

STRUCT(trie_fork)
{
    trie_t t;
    int connected;
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
    void_pointer                pointer_free;

    value_info_t vi;
};

void trie_delete(trie_info_p ti, trie_pointer_p *tp, char arr[]);
void trie_insert(trie_info_p ti, trie_pointer_p *tp, char arr[], value_p value);

void trie_display(trie_info_p ti, value_info_p vi, trie_pointer_p p);
value_p trie_querie_rec(trie_info_p ti, trie_pointer_p tp, char len, char arr[]);

void trie_free(trie_info_p ti, trie_pointer_p tp);

#endif