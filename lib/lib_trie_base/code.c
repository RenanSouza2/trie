#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "header.h"
#include "../lib_my_string/header.h"

#define T(POINTER)  ((trie_p)(POINTER))
#define FN(POINTER, INDEX) ((trie_pointer_p)(HP(POINTER) + INDEX * ti->pointer_size))
#define PN(POINTER) ((trie_pointer_p)(HP(POINTER)))
#define PS(POINTER) ((string_p)(HP(POINTER) + ti->pointer_size))

#define P(POINTER) ((trie_pointer_p)(POINTER))

#define FALSE 0
#define TRUE  1


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
        memcpy(&res[len], str->arr, str->len);

        trie_pointer_p next = PN(t);
        trie_display_rec(ti, next, len + str->len, res);
        break;

        case LEAF:
        printf("\n");
        for(int i=0; i<len; i++)
            printf(" %2d", res[i]);
        printf("\t->\t"); ti->vi->value_print(LV(t));
        break;
    }
}

void trie_display(trie_info_p ti, value_info_p vi, trie_pointer_p p)
{
    char res[ti->len];
    if(ti->pointer_is_null(p))  
        printf("\nEMPTY TRIE");
    else                        
        trie_display_rec(ti, p, 0, res);
    printf("\n");
}



trie_pointer_p trie_fork_create(trie_info_p ti, trie_pointer_p tp_next, int key)
{
    trie_p t = malloc(ti->fork_size);
    assert(t);

    t->type = FORK;
    TF(t)->connected = ti->max;

    for(int i=0; i < ti->max; i++)
    {
        trie_pointer_p next = FN(t, i);
        memcpy(next, ti->null, ti->pointer_size);
    }

    trie_pointer_p tp = ti->get_pointer(t);
    return ti->trie_fork_connect(tp, key, tp_next);
}

trie_pointer_p trie_path_create_force(trie_info_p ti, char len, char arr[], trie_pointer_p tp_next)
{
    assert(len > 0);

    trie_p t = malloc(sizeof(trie_fork_p) + ti->pointer_size + len);
    assert(t);

    string_p str = PS(t);
    t->type = PATH;
    str->len = len;
    memcpy(str->arr, arr, len);

    trie_pointer_p next = PN(t);
    memcpy(next, ti->null, ti->pointer_size);

    trie_pointer_p tp = ti->get_pointer(t);
    return ti->trie_path_connect(tp, tp_next);
}

trie_pointer_p trie_path_create(trie_info_p ti, char len, char arr[], trie_pointer_p tp_next)
{
    switch (len)
    {
        case 0: return tp_next;
        case 1: return trie_fork_create(ti, tp_next, arr[0]);
    }

    return trie_path_create_force(ti, len, arr, tp_next);
}

trie_pointer_p trie_leaf_create(trie_info_p ti, value_p value)
{
    trie_p t = malloc(sizeof(trie_fork_p) + ti->vi->size);
    assert(t);

    value_p _value = LV(t);
    memcpy(_value, ti->vi->null, ti->vi->size);

    trie_pointer_p tp = ti->get_pointer(t);
    return ti->trie_leaf_set_value(ti->vi, tp, value);
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
    string_p str = PS(t);
    assert(len < str->len);

    char *arr = str->arr;

    trie_pointer_p t_aux = PN(t);
    t_aux = trie_path_create(ti, str->len - len-1, &arr[len+1], t_aux);
    t_aux = trie_path_create(ti, 1, &arr[len], t_aux);
    t_aux = trie_path_create(ti, len, arr, t_aux);

    ti->trie_free_single(tp);
    return t_aux;
}

trie_pointer_p trie_fork_convert(trie_info_p ti, trie_pointer_p tp)
{
    trie_p t = ti->get_trie(tp);
    if(t->type == PATH) return tp;

    assert(t->type == FORK);
    assert(TF(t)->connected == 1);

    char key = trie_fork_first_key(ti, t);
    trie_pointer_p next = FN(t, key);
    ti->trie_free_single(tp);

    char arr[] = {key};
    return trie_path_create_force(ti, 1, arr, next);
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
        trie_pointer_p next = FN(t, key);
        tp_next = trie_delete_rec(ti, next, len-1, &arr[1]);

        tp = ti->trie_fork_connect(tp, key, tp_next);
        if(ti->pointer_is_null(tp)) return ti->null;
        
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

        next = PN(t);
        tp_next = trie_delete_rec(ti, next, len-path_len, &arr[path_len]);

        if(ti->pointer_is_null(tp_next))
        {
            ti->trie_free_single(tp);
            return ti->null;
        }

        tp = ti->trie_path_connect(tp, tp_next);        
        break;

        case LEAF:
        assert(len == 0);
        ti->trie_free_single(tp);
        return ti->null;
    }

    trie_p t_next = ti->get_trie(tp_next);
    return (trie_joinnable(t_next)) ? trie_join(ti, tp, tp_next) : tp;
}

trie_pointer_p trie_insert_rec(trie_info_p ti, trie_pointer_p tp, char len, char arr[], value_p value)
{
    if(ti->pointer_is_null(tp))
    {
        tp = trie_leaf_create(ti, value);
        return trie_path_create(ti, len, arr, tp);
    }

    trie_p t = ti->get_trie(tp);
    if(t->type == LEAF)
    {
        assert(len == 0);
        return ti->trie_leaf_set_value(ti->vi, tp, value);
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
            trie_pointer_p next = PN(t);
            next = trie_insert_rec(ti, next, len-index, &arr[index], value);
            return ti->trie_path_connect(tp, next);
        }
    }

    int key = arr[0];
    trie_pointer_p next = FN(t, key);
    next = trie_insert_rec(ti, next, len-1, &arr[1], value);
    return ti->trie_fork_connect(tp, key, next);
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
    *tp = (ti->vi->value_is_null(value)) 
        ? trie_insert_rec(ti, *tp, ti->len, arr, value) 
        : trie_delete_rec(ti, *tp, ti->len, arr);
}

void trie_delete(trie_info_p ti, trie_pointer_p *tp, char arr[])
{
    *tp = trie_delete_rec(ti, *tp, ti->len, arr);
}

value_p trie_querie(trie_info_p ti, trie_pointer_p *tp, char arr[])
{
    return trie_querie_rec(ti, *tp, ti->len, arr);
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
