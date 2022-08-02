#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "header.h"
#include "../lib_my_string/header.h"

#define T(POINTER)  ((trie_p)(POINTER))
#define FN(POINTER, INDEX) ((trie_pointer_p)(HP(POINTER) + (INDEX) * ti->pointer_size))
#define PN(POINTER) ((trie_pointer_p)(HP(POINTER)))
#define PS(POINTER) ((string_p)(HP(POINTER) + ti->pointer_size))
#define TPC(POINTER1, POINTER2) (memcpy(POINTER1, POINTER2, ti->pointer_size))
#define PC(POINTER1, POINTER2) (memcpy(POINTER1, POINTER2, ti->path_size + PS(POINTER2)->len))

#define P(POINTER) ((trie_pointer_p)(POINTER))

#define FORK 0
#define PATH 1
#define LEAF 2

#define FALSE 0
#define TRUE  1


trie_pointer_p trie_pointer_copy(trie_info_p ti, trie_pointer_p tp)
{
    trie_pointer_p tp_new = calloc(1, sizeof(ti->pointer_size));
    assert(tp);

    TPC(tp_new, tp);
    return tp_new;
}



void trie_display_rec(trie_info_p ti, trie_pointer_p tp, int len, char res[])
{
    trie_p t = ti->get_trie(tp);
    switch(t->type)
    {
        case FORK:
        for(int i = 0; i < ti->max; i++)
        {
            trie_pointer_p next = FN(t, i);
            if(ti->pointer_is_null(next)) continue;
            
            res[len] = i;
            trie_display_rec(ti, next, len+1, res);
        }
        break;

        case PATH:;
        string_p str = PS(t);
        trie_pointer_p next = PN(t);
        memcpy(&res[len], str->arr, str->len);
        trie_display_rec(ti, next, len + str->len, res);
        break;

        case LEAF:
        printf("\n");
        for(int i=0; i<len; i++)
            printf(" %2d", res[i]);
        printf("\t->\t"); 
        ti->vi->value_print(LV(t));
        break;
    }
}

void trie_display(trie_info_p ti, trie_pointer_p tp)
{
    char res[ti->len];
    printf("\nBefore test");
    if(ti->pointer_is_null(tp)) printf("\nEMPTY TRIE");
    else                        trie_display_rec(ti, tp, 0, res);
    printf("\n");
}



trie_pointer_p trie_fork_create(trie_info_p ti, int key, trie_pointer_p tp_next)
{
    trie_p t = calloc(1, ti->fork_size);
    assert(t);

    t->type = FORK;
    TF(t)->connected = 1;

    for(int i=0; i < ti->max; i++)
    {
        trie_pointer_p next = FN(t, i);
        TPC(next, ti->null);
    }

    trie_pointer_p next = FN(t, key);
    TPC(next, tp_next);
    free(tp_next);

    trie_pointer_p tp = ti->pointer_create(t);
    return tp;
}

trie_pointer_p trie_path_create_force(trie_info_p ti, char len, char arr[], trie_pointer_p tp_next)
{
    assert(len > 0);

    trie_p t = calloc(1, ti->path_size + len);
    assert(t);

    t->type = PATH;
    string_p str = PS(t);
    str->len = len;
    memcpy(str->arr, arr, len);

    trie_pointer_p next = PN(t);
    TPC(next, tp_next);
    free(tp_next);
    
    trie_pointer_p tp = ti->pointer_create(t);
    return tp;
}

trie_pointer_p trie_path_create(trie_info_p ti, char len, char arr[], trie_pointer_p tp_next)
{
    switch (len)
    {
        case 0: return tp_next;
        case 1: return trie_fork_create(ti, arr[0], tp_next);
    }

    return trie_path_create_force(ti, len, arr, tp_next);
}

trie_pointer_p trie_leaf_create(trie_info_p ti, value_p value)
{
    trie_p t = calloc(1, ti->leaf_size);
    assert(t);

    value_p _value = LV(t);
    memcpy(_value, value, ti->vi->size);
    free(value);

    trie_pointer_p tp = ti->pointer_create(t);
    return tp;
}



trie_p trie_copy(trie_info_p ti, trie_p t)
{
    int size;
    switch (t->type)
    {
        case FORK: size = ti->fork_size; break;
        case PATH: size = ti->path_size + PS(t)->len; break;
        case LEAF: size = ti->leaf_size; break;
        default: assert(FALSE);
    }
    trie_p t_new = malloc(size);
    memcpy(t_new, t, size);
    return t;
}



trie_pointer_p trie_fork_disconnect(trie_info_p ti, trie_pointer_p tp, int key)
{
    trie_p t = ti->get_trie(tp);
    trie_pointer_p next = FN(t, key);
    if(ti->pointer_is_null(next)) return tp;

    if(TF(t)->connected == 1)
    {
        ti->trie_free_single(tp);
        return NULL;
    }

    trie_p t_new = trie_copy(ti, t);
    next = FN(t_new, key);

    (TF(t_new)->connected)--;
    TPC(next, ti->null);

    ti->trie_replace(tp, t_new);
    return tp;
}

trie_pointer_p trie_fork_connect(trie_info_p ti, trie_pointer_p tp, int key, trie_pointer_p tp_next)
{
    if(ti->pointer_is_null(tp_next)) return trie_fork_disconnect(ti, tp, key);

    trie_p t = ti->get_trie(tp);
    t = trie_copy(ti, t);

    trie_pointer_p next = FN(t, key);
    if(ti->pointer_is_null(next)) (TF(t)->connected)++;
    TPC(next, tp_next);
    free(tp_next);

    ti->trie_replace(tp, t);
    return tp;
}

trie_pointer_p trie_path_connect(trie_info_p ti, trie_pointer_p tp, trie_pointer_p tp_next)
{
    if(ti->pointer_is_null(tp_next))
    {
        ti->trie_free_single(tp);
        return NULL;
    }

    trie_p t = ti->get_trie(tp);
    t = trie_copy(ti, t);

    trie_pointer_p next = PN(t);
    TPC(next, tp_next);
    free(tp_next);

    ti->trie_replace(tp, t);
    return tp;
}

void trie_leaf_set_value(trie_info_p ti, trie_pointer_p tp, value_p value)
{
    trie_p t = ti->get_trie(tp);
    t = trie_copy(ti, t);

    memcpy(LV(t), value, ti->vi->size);

    ti->trie_replace(tp, t);
}



int trie_joinnable(trie_p t)
{
    if(t->type == PATH) return TRUE;
    if(t->type == LEAF) return FALSE;
    return TF(t)->connected == 1;
}

int trie_fork_first_key(trie_info_p ti, trie_p t)
{
    for(int i=0; i < ti->max; i++)
    {
        trie_pointer_p next = FN(t,i);
        if(ti->pointer_is_null(next)) continue;

        return i;
    }
    assert(FALSE);
}

trie_pointer_p trie_path_break(trie_info_p ti, trie_pointer_p tp, int len)
{
    trie_p t = ti->get_trie(tp);
    assert(t->type == PATH);

    string_p str = PS(t);
    assert(len < str->len);

    char *arr = str->arr;

    trie_pointer_p tp_aux = trie_pointer_copy(ti, PN(t));
    tp_aux = trie_path_create(ti, str->len - len-1, &arr[len+1], tp_aux);
    tp_aux = trie_path_create(ti, 1, &arr[len], tp_aux);
    tp_aux = trie_path_create(ti, len, arr, tp_aux);

    ti->trie_free_single(tp);
    return tp_aux;
}

trie_pointer_p trie_fork_convert(trie_info_p ti, trie_pointer_p tp)
{
    trie_p t = ti->get_trie(tp);
    if(t->type == PATH) return tp;

    assert(t->type == FORK);
    assert(TF(t)->connected == 1);

    char key = trie_fork_first_key(ti, t);
    trie_pointer_p next = FN(t, key);
    trie_pointer_p tp_new = trie_path_create_force(ti, 1, &key, next);

    ti->trie_free_single(tp);
    return tp_new;
}

trie_pointer_p trie_join(trie_info_p ti, trie_pointer_p tp1, trie_pointer_p tp2)
{
    tp1 = trie_fork_convert(ti, tp1);
    tp2 = trie_fork_convert(ti, tp2);
    trie_p t1 = ti->get_trie(tp1);
    trie_p t2 = ti->get_trie(tp2);

    string_p str1 = PS(t1);
    string_p str2 = PS(t2);
    int len1 = str1->len;
    int len2 = str2->len;
    int len  = len1 + len2;

    char arr[len];
    memcpy( arr      , str1->arr, len1);
    memcpy(&arr[len1], str2->arr, len2);

    trie_pointer_p next = PN(t2);
    trie_pointer_p tp = trie_path_create(ti, len, arr, next);

    ti->trie_free_single(tp1);
    ti->trie_free_single(tp2);
    return tp;
}



trie_pointer_p trie_delete_rec(trie_info_p ti, trie_pointer_p tp, char len, char arr[])
{
    if(ti->pointer_is_null(tp)) return NULL;

    trie_pointer_p tp_next;
    trie_p t = ti->get_trie(tp);
    switch (t->type)
    {
        case FORK:;
        int key = arr[0];
        tp_next = trie_pointer_copy(ti, FN(t, key));
        tp_next = trie_delete_rec(ti, tp_next, len-1, &arr[1]);
        tp = trie_fork_connect(ti, tp, key, tp_next);
        if(tp == NULL) return NULL;
        
        t = ti->get_trie(tp);
        if(TF(t)->connected > 1) return tp; 

        key = trie_fork_first_key(ti, t);
        tp_next = FN(t, key);
        break;
    
        case PATH:;
        string_p str = PS(t);
        int path_len = str->len;
        int index = string_cmp(str, arr);
        if(index < path_len) return tp;

        tp_next = trie_pointer_copy(ti, PN(t));
        tp_next = trie_delete_rec(ti, tp_next, len-path_len, &arr[path_len]);
        tp = trie_path_connect(ti, tp, tp_next);
        break;

        case LEAF:
        assert(len == 0);
        ti->trie_free_single(tp);
        return NULL;
    }

    trie_p t_next = ti->get_trie(tp_next);
    return (trie_joinnable(t_next)) ? trie_join(ti, tp, tp_next) : tp;
}

trie_pointer_p trie_insert_rec(trie_info_p ti, trie_pointer_p tp, char len, char arr[], value_p value)
{
    if(ti->pointer_is_null(tp))
    {
        tp = trie_leaf_create(ti, value);
        tp = trie_path_create(ti, len, arr, tp);
        return tp;
    }

    trie_p t = ti->get_trie(tp);
    if(t->type == LEAF)
    {
        assert(len == 0);
        trie_leaf_set_value(ti, tp, value);
        return tp;
    }

    if(t->type == PATH)
    {
        string_p str = PS(t);
        int index = string_cmp(str, arr);
        if(index < str->len) 
        {
            tp = trie_path_break(ti, tp, index);
            t  = ti->get_trie(tp);
        }
        if(t->type == PATH)
        {
            trie_pointer_p tp_next = trie_pointer_copy(ti, PN(t));
            tp_next = trie_insert_rec(ti, tp_next, len-index, &arr[index], value);
            return trie_path_connect(ti, tp, tp_next);
        }
    }

    int key = arr[0];
    trie_pointer_p tp_next = trie_pointer_copy(ti, FN(t, key));
    tp_next = trie_insert_rec(ti, tp_next, len-1, &arr[1], value);
    return trie_fork_connect(ti, tp, key, tp_next);
}

value_p trie_querie_rec(trie_info_p ti, trie_pointer_p tp, char len, char arr[])
{
    if(ti->pointer_is_null(tp)) return NULL;

    trie_p t = ti->get_trie(tp);
    switch (t->type)
    {
        case FORK:;
        trie_pointer_p next = FN(t, arr[0]);
        return trie_querie_rec(ti, next, len-1, &arr[1]);
        
        case PATH:;
        string_p str = PS(t);
        int index = string_cmp(str, arr);
        if(index < str->len) return NULL;

        next = PN(t);
        return trie_querie_rec(ti, next, len-index, &arr[index]);

        case LEAF:
        return LV(t);
    }
    assert(FALSE);
} 



void trie_insert(trie_info_p ti, trie_pointer_p *tp, char arr[], value_p value)
{
    printf("\ntrie_insert");
    *tp = (ti->vi->value_is_null(value)) 
        ? trie_delete_rec(ti, *tp, ti->len, arr)
        : trie_insert_rec(ti, *tp, ti->len, arr, value);
}

void trie_delete(trie_info_p ti, trie_pointer_p *tp, char arr[])
{
    *tp = trie_delete_rec(ti, *tp, ti->len, arr);
}

value_p trie_querie(trie_info_p ti, trie_pointer_p tp, char arr[])
{
    return trie_querie_rec(ti, tp, ti->len, arr);
}



void trie_free(trie_info_p ti, trie_pointer_p tp)
{
    if(ti->pointer_is_null(tp)) return;

    trie_p t = ti->get_trie(tp);
    switch (t->type)
    {
        case FORK:
        for(int i=0; i < ti->max; i++)
        {
            trie_pointer_p next = FN(t, i);
            trie_free(ti, next);
        }
        break;
    
        case PATH:
        trie_pointer_p next = PN(t);
        trie_free(ti, next);
        break;
    }
    ti->trie_free_single(tp);
}
