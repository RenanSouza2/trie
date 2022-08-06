#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "header.h"
#include "../lib_string/header.h"
#include "../lib_base_header/trie.h"
#include "../lib_base_header/value.h"
#include "../lib_base_header/pointer.h"

#define PI ti->pi
#define VI ti->vi

#define HP(POINTER) ((char*)(((trie_p)(POINTER))+1))
#define FN(POINTER, INDEX) ((pointer_p)(HP(POINTER) + (INDEX) * PI->size))
#define PS(POINTER) ((string_p)(HP(POINTER) + PI->size))
#define PN(POINTER) ((pointer_p)HP(POINTER))
#define LV(POINTER) ((value_p)HP(POINTER))

#define PTR_CPY(POINTER1, POINTER2) memcpy(POINTER1, POINTER2, PI->size);
#define FORK_SIZE (8 + MAX * PI->size)
#define PATH_SIZE(LENGTH) (9 + PI->size + LENGTH)
#define LEAF_SIZE(SIZE) (8 + SIZE)

#define FALSE 0
#define TRUE  1

#define FORK 0
#define PATH 1
#define LEAF 2

#define MAX 10
#define LEN 8



STRUCT(trie)
{
    int type, connected;
};



pointer_p pointer_copy(pointer_info_p pi, pointer_p p)
{
    pointer_p p_new = malloc(pi->size);
    assert(p_new);

    memcpy(p_new, p, pi->size);
    return p_new;
}

trie_p trie_path_copy(trie_info_p ti, trie_p t)
{
    int len  = PS(t)->len;
    int size = PATH_SIZE(len);
    trie_p t_new = malloc(size);
    assert(t_new);

    memcpy(t_new, t, size);
    return t_new;
}

int trie_fork_first_key(trie_info_p ti, trie_p t)
{
    for(int i=0; i<MAX; i++)
    {
        pointer_p next = FN(t, i);
        if(!PI->is_null(next)) return i;
    }
    assert(FALSE);
}



void trie_display_single(trie_info_p ti, pointer_p tp) 
{
    printf("\ntrie: ");
    PI->display(tp);

    trie_p t = PI->get(tp);
    switch (t->type)
    {
        case FORK:
        printf("\t(FORK)");
        printf("\nconnected: %d", t->connected);
        for(int i=0; i<MAX; i++)
        {
            pointer_p next = FN(t, i);
            if(PI->is_null(next)) continue;

            printf("\n\t%d: ", i);
            PI->display(next);
        }
        break;
    
        case PATH:
        printf("\t(PATH)");
        string_display(PS(t));
        printf("\nnext: ");
        PI->display(PN(t));
        break;

        case LEAF:
        printf("\t(LEAF)");
        ti->vi->print(LV(t));
        break;
    }
    printf("\n");
}

void trie_display_structure_rec(trie_info_p ti, pointer_p tp)
{
    trie_p t = PI->get(tp);
    trie_display_single(ti, tp);
    switch (t->type)
    {
        case FORK:
        for(int i=0; i<MAX; i++)
        {
            pointer_p next = FN(t, i);
            if(PI->is_null(next)) continue;

            trie_display_structure_rec(ti, next);
        }
        break;

        case PATH:
        trie_display_structure_rec(ti, PN(t));
        break;
    }
}

void trie_display_rec(trie_info_p ti, pointer_p tp, int len, char res[])
{
    trie_p t = PI->get(tp);
    switch(t->type)
    {
        case FORK:
        for(int i=0; i<MAX; i++)
        {
            pointer_p next = FN(t, i);
            if(PI->is_null(next)) continue;

            res[len] = i;
            trie_display_rec(ti, next, len+1, res);
        }
        break;

        case PATH:
        string_p str = PS(t);
        memcpy(&res[len], str->arr, str->len);
        trie_display_rec(ti, PN(t), len + str->len, res);
        break;

        case LEAF:
        printf("\n");
        for(int i=0; i<len; i++)
            printf(" %2d", res[i]);
        printf("\t->\t"); VI->print(LV(t));
        break;
    }
}



void trie_display_structure(trie_info_p ti, pointer_p tp)
{
    if(PI->is_null) printf("\nEmpty trie");
    else            trie_display_structure_rec(ti, tp);
}

void trie_display(trie_info_p ti, pointer_p tp)
{
    char res[LEN];
    if(tp == NULL) printf("\nEMPTY TRIE");
    else           trie_display_rec(ti, tp, 0, res);
    printf("\n");
}



pointer_p trie_fork_create(trie_info_p ti, int key, pointer_p tp_next)
{
    int size = FORK_SIZE;
    trie_p t = calloc(1, size);
    assert(t);

    t->type = FORK;
    t->connected = 1;

    pointer_p next = FN(t, key);
    PTR_CPY(next, tp_next);
    free(tp_next);

    return PI->set(t, size);
}

pointer_p trie_path_create_force(trie_info_p ti, char len, char arr[], pointer_p tp_next)
{
    assert(len > 0);

    int size = PATH_SIZE(len);
    trie_p t = malloc(size);
    assert(t);

    t->type = PATH;

    pointer_p next = PN(t);
    PTR_CPY(next, tp_next);

    string_p str = PS(t);
    str->len = len;
    memcpy(str->arr, arr, len);

    return PI->set(t, size);
}

pointer_p trie_path_create(trie_info_p ti, char len, char arr[], pointer_p tp_next)
{
    switch (len)
    {
        case 0: return tp_next;
        case 1: return trie_fork_create(ti, arr[0], tp_next);
    }

    return trie_path_create_force(ti, len, arr, tp_next);
}

pointer_p trie_leaf_create(trie_info_p ti, value_p value)
{
    int value_size = VI->size(value);
    int size = LEAF_SIZE(value_size);
    trie_p t = calloc(1, size);
    assert(t);

    t->type = LEAF;
    memcpy(LV(t), value, value_size);
    free(value);

    return PI->set(t, value_size);
}



pointer_p trie_fork_disconnect(trie_info_p ti, pointer_p tp, int key)
{
    trie_p t = PI->get(tp);
    assert(t->type == FORK);

    pointer_p next = FN(t, key);
    if(PI->is_null(next)) return tp;

    if(t->connected == 1)
    {
        PI->free(tp);
        return NULL;
    }

    t->connected--;
    PTR_CPY(next, PI->null);

    return PI->replace(tp, t, FORK_SIZE);
}

pointer_p trie_fork_connect(trie_info_p ti, pointer_p tp, int key, pointer_p tp_next)
{
    if(tp_next == NULL) return trie_fork_disconnect(ti, tp, key);

    trie_p t = PI->get(tp);
    assert(t->type == FORK);

    pointer_p next = FN(t, key);
    if(PI->is_null(next)) t->connected++;
    PTR_CPY(next, tp_next);
    free(tp_next);

    return PI->replace(tp, t, FORK_SIZE);
}

pointer_p trie_path_connect(trie_info_p ti, pointer_p tp, pointer_p tp_next)
{
    if(tp_next == NULL)
    {
        PI->free(tp);
        return NULL;
    }

    trie_p t = PI->get(tp);
    PTR_CPY(PN(t), tp_next);

    int len = PS(t)->len;
    return PI->replace(tp, t, len);
}

pointer_p trie_leaf_set_value(trie_info_p ti, pointer_p tp, value_p value)
{
    trie_p t = PI->get(tp);

    int size = VI->size(value);
    memcpy(LV(t), value, size);

    return PI->replace(tp, t, LEAF_SIZE(size));
}




int trie_joinnable(trie_p t)
{
    if(t->type == PATH) return TRUE;
    if(t->type == LEAF) return FALSE;
    return t->connected == 1;
}

pointer_p trie_path_break(trie_info_p ti, pointer_p tp, int len)
{
    trie_p t = PI->get(tp);
    string_p str = PS(t);
    assert(len < str->len);

    char *arr = str->arr;

    pointer_p t_aux = pointer_copy(PI, PN(t));
    t_aux = trie_path_create(ti, str->len - len-1, &arr[len+1], t_aux);
    t_aux = trie_fork_create(ti, arr[len], t_aux);
    t_aux = trie_path_create(ti, len, arr, t_aux);

    PI->free(tp);
    return t_aux;
}

pointer_p trie_fork_convert(trie_info_p ti, pointer_p tp)
{
    trie_p t = PI->get(tp);
    if(t->type == PATH) return tp;

    assert(t->type == FORK);
    assert(t->connected == 1);

    int key = trie_fork_first_key(ti, t);
    pointer_p t_next = pointer_copy(PI, FN(t, key));
    PI->free(tp);

    char arr[] = {key};
    return trie_path_create_force(ti, 1, arr, t_next);
}

pointer_p trie_join(trie_info_p ti, pointer_p tp1, pointer_p tp2)
{
    tp1 = trie_fork_convert(ti, tp1);
    tp2 = trie_fork_convert(ti, tp2);
    trie_p t1 = PI->get(tp1);
    trie_p t2 = PI->get(tp2);

    string_p str1 = PS(t1);
    string_p str2 = PS(t2);
    int len1 = str1->len;
    int len2 = str2->len;
    int len  = len1 + len2;

    char arr[len];
    memcpy(arr, str1->arr, len1);
    memcpy(&arr[len1], str2->arr, len2);
    
    pointer_p tp = trie_path_create(ti, len, arr, PN(t2));
    PI->free(tp1);
    PI->free(tp2);
    return tp;
}



pointer_p trie_delete_rec(trie_info_p ti, pointer_p tp, char len, char arr[])
{
    if(PI->is_null(tp)) 
    {
        free(tp);
        return NULL;
    }

    pointer_p tp_next;
    trie_p t = PI->get(tp);
    switch (t->type)
    {
        case FORK:;
        int key = arr[0];

        tp_next = pointer_copy(PI, FN(t, key));
        tp_next = trie_delete_rec(ti, tp_next, len-1, &arr[1]);
        tp = trie_fork_connect(ti, tp, key, tp_next);
        if(tp == NULL) return NULL;

        t = PI->get(tp);
        if(t->connected > 1) return tp;

        tp_next = FN(t, key);
        break;
    
        case PATH:;
        string_p str = PS(t);
        int path_len = str->len;
        int index = string_cmp(str, arr);
        if(index < path_len) return tp;

        tp_next = pointer_copy(PI, PN(t));
        tp_next = trie_delete_rec(ti, tp_next, len-path_len, &arr[path_len]);
        tp = trie_path_connect(ti, tp, tp_next);
        if(tp == NULL) return NULL;

        t = PI->get(tp);
        tp_next = PN(t);
        break;

        case LEAF:
        PI->free(tp);
        return NULL;
    }

    t = PI->get(tp_next);
    return (trie_joinnable(t)) ? trie_join(ti, tp, tp_next) : tp;
}

pointer_p trie_insert_rec(trie_info_p ti, pointer_p tp, char len, char arr[], value_p value)
{
    if(PI->is_null(tp))
    {
        free(tp);
        tp = trie_leaf_create(ti, value);
        return trie_path_create(ti, len, arr, tp);
    }

    trie_p t = PI->get(tp);
    if(t->type == LEAF) return trie_leaf_set_value(ti, tp, value);

    if(t->type == PATH)
    {
        string_p str = PS(t);
        int index = string_cmp(str, arr);
        if(index < str->len) tp = trie_path_break(ti, tp, index);

        if(t->type == PATH)
        {
            pointer_p tp_next = pointer_copy(PI, PN(tp));
            tp_next = trie_insert_rec(ti, tp_next, len-index, &arr[index], value);
            return trie_path_connect(ti, tp, tp_next);
        }
    }

    int key = arr[0];
    pointer_p tp_next = pointer_copy(PI, FN(t, key));
    tp_next = trie_insert_rec(ti, tp_next, len-1, &arr[1], value);
    return trie_fork_connect(ti, tp, key, tp_next);
}

value_p trie_querie_rec(trie_info_p ti, pointer_p tp, char len, char arr[])
{
    if(PI->is_null(tp)) return NULL;

    trie_p t = PI->get(tp);
    switch (t->type)
    {
        case FORK:
        return trie_querie_rec(ti, FN(t, arr[0]), len-1, &arr[1]);
        
        case PATH:;
        string_p str = PS(t);
        int index = string_cmp(str, arr);
        if(index < str->len) return NULL;

        return trie_querie_rec(ti, PN(t), len-index, &arr[index]);

        case LEAF:
        assert(!len);
        return LV(t);
    }
    assert(FALSE);
}



void trie_insert(trie_info_p ti, pointer_p *tp, char arr[], value_p value)
{
    *tp = (VI->is_null(value)) 
        ? trie_delete_rec(ti, *tp, LEN, arr)
        : trie_insert_rec(ti, *tp, LEN, arr, value);
}

void trie_delete(trie_info_p ti, pointer_p *tp, char arr[])
{
    *tp = trie_delete_rec(ti, *tp, LEN, arr);
}

value_p trie_querie(trie_info_p ti, pointer_p tp, char arr[])
{
    return trie_querie_rec(ti, tp, LEN, arr);
}



void trie_free(trie_info_p ti, pointer_p tp)
{
    if(tp == NULL) return;

    trie_p t = PI->get(tp);
    switch (t->type)
    {
        case FORK:
        for(int i=0; i<MAX; i++)
        {
            pointer_p next = FN(t, i);
            if(PI->is_null(next)) continue;
            
            trie_free(ti, next);
        }
        break;
    
        case PATH:
        trie_free(ti, PN(t));
        break;
    }
    PI->free(tp);
}
