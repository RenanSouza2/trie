#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "header.h"
#include "../lib_my_string/header.h"

#define T(POINTER)  ((trie_p)(POINTER))
#define TF(POINTER) ((trie_fork_p)(POINTER))
#define TP(POINTER) ((trie_padding_p)(POINTER))
#define HP(POINTER) ((char*)(TP(POINTER) + 1))
#define NP(POINTER) ((char*)(TF(POINTER) + 1))
#define VP(POINTER) ((value_p)(HP(POINTER)))

#define P(POINTER) ((trie_pointer_p)(POINTER))

#define FALSE 0
#define TRUE  1

#define FORK 0
#define PATH 1
#define LEAF 2

STRUCT(trie_fork)
{
    trie_t t;
    int connected;
};

STRUCT(trie_padding)
{
    trie_t t;
    int padding;
};

#ifdef DEBUG

void pointer_display(void *p)
{
    if(p == NULL)   printf("NULL");
    else            printf("%p", p);
}

void trie_display_single(trie_info_p ti, trie_pointer_p tp) {
    printf("\ntrie: ");
    ti->pointer_display(tp);
    if(ti->pointer_is_null(tp)) return;

    trie_p t = ti->get_trie(tp);
    switch (t->type)
    {
        case FORK:
        printf("\t(FORK)");
        printf("\nconnected: %d", TF(t)->connected);
        for(int i=0; i < ti->max; i++)
        {
            trie_pointer_p next = trie_fork_next(ti, t, i);
            if(ti->pointer_is_null(next)) continue;

            printf("\n\t%d: ", i);
            ti->pointer_display(next);
        }
        break;
    
        case PATH:;
        printf("\t(PATH)");

        string_p str = trie_path_str(ti, t);
        string_display(str);

        printf("\nnext: ");
        trie_pointer_p next = trie_path_next(t);
        ti->pointer_display(next);
        break;

        case LEAF:
        printf("\t(LEAF)");
        ti->vi.value_print(HP(t));
        break;
    }
    printf("\n");
}

void trie_display_structure_rec(trie_info_p ti, trie_pointer_p tp)
{
    if(ti->pointer_is_null(tp)) return;

    trie_p t = ti->get_trie(tp);
    trie_display_single(ti, t);
    switch (t->type)
    {
        case FORK:
        for(int i=0; i < ti->max; i++)
        {
            trie_pointer_p next = trie_fork_next(ti, t, i);
            trie_display_structure_rec(ti, next);
        }
        break;

        case PATH:;
        trie_pointer_p next = trie_path_next(t);
        trie_display_structure_rec(ti, next);
        break;
    }
}

void trie_display_structure(trie_info_p ti, trie_pointer_p tp)
{
    if(ti->pointer_is_null(tp)) printf("\nEmpty trie");
    else                        trie_display_structure_rec(vi, tp);
}

#endif 


trie_pointer_p trie_fork_next(trie_info_p ti, trie_p t, int index)
{
    return P(NP(t) + index * ti->pointer_size);
}

trie_pointer_p trie_path_next(trie_p t)
{
    return P(HP(t));
}

string_p trie_path_str(trie_info_p ti, trie_p t)
{
    return (string_p)(HP(t) + ti->pointer_size);
}

void trie_display_rec(trie_info_p ti, trie_pointer_p tp, int len, char res[])
{
    trie_p t = ti->get_trie(tp);
    switch(t->type)
    {
        case FORK:
        for(int i = 0; i < ti->max; i++)
        {
            trie_pointer_p next = trie_fork_next(ti, t, i);
            if(ti->pointer_is_null(next)) continue;
            
            res[len] = i;
            trie_display_rec(ti, next, len+1, res);
        }
        break;

        case PATH:;
        string_p str = trie_path_str(ti, t);
        memcpy(&res[len], str->arr, str->len);

        trie_pointer_p next = trie_path_next(t);
        trie_display_rec(ti, next, len + str->len, res);
        break;

        case LEAF:
        printf("\n");
        for(int i=0; i<len; i++)
            printf(" %2d", res[i]);
        printf("\t->\t"); ti->vi.value_print(VP(t));
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



int trie_joinnable(trie_p t)
{
    if(t->type == PATH) return TRUE;
    if(t->type == LEAF) return FALSE;
    return TF(t)->connected == 1;
}

trie_pointer_p trie_path_break(trie_info_p ti, trie_pointer_p tp, int len)
{
    trie_p t = ti->get_trie(tp);
    string_p str = trie_path_str(ti, t);
    assert(len < str->len);

    char *arr = str->arr;

    trie_pointer_p t_aux = trie_path_next(t);
    t_aux = ti->trie_path_create(str->len - len-1, &arr[len+1], t_aux);
    t_aux = ti->trie_path_create(1, &arr[len], t_aux);
    t_aux = ti->trie_path_create(len, arr, t_aux);

    ti->trie_free_single(tp);
    return t_aux;
}

trie_pointer_p trie_fork_convert(trie_info_p ti, trie_pointer_p tp)
{
    trie_p t = ti->get_trie(tp);
    if(t->type == PATH) return tp;

    assert(t->type == FORK);
    assert(TF(t)->connected == 1);

    char key = ti->trie_fork_first_key(t);
    trie_pointer_p next = trie_fork_next(ti, t, key);
    ti->trie_free_single(tp);

    char arr[] = {key};
    return ti->trie_path_create_force(1, arr, next);
}

trie_pointer_p trie_join(trie_info_p ti, trie_pointer_p tp1, trie_pointer_p tp2)
{
    tp1 = trie_fork_convert(ti, tp1);
    tp2 = trie_fork_convert(ti, tp2);
    trie_p t1 = ti->get_trie(tp1);
    trie_p t2 = ti->get_trie(tp2);

    string_p str1 = trie_path_str(ti, t1);
    string_p str2 = trie_path_str(ti, t2);
    int len1 = str1->len;
    int len2 = str2->len;
    int len  = len1 + len2;

    char arr[len];
    memcpy( arr      , str1->arr, len1);
    memcpy(&arr[len1], str2->arr, len2);

    trie_pointer_p next = trie_path_next(t2);
    trie_pointer_p tp = ti->trie_path_create(len, arr, next);

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
        trie_pointer_p next = trie_fork_next(ti, t, key);
        tp_next = trie_delete_rec(ti, next, len-1, &arr[1]);

        if(ti->pointer_is_null(tp_next) && TF(t)->connected == 1) 
        {
            ti->trie_free_single(tp);
            return ti->null;
        }

        tp = ti->trie_fork_connect(tp, key, tp_next);
        t  = ti->get_trie(tp);
        if(TF(t)->connected > 1) return tp; 

        key = ti->trie_fork_first_key(t);
        tp_next = trie_fork_next(ti, t, key);
        break;
    
        case PATH:;
        string_p str = trie_path_str(ti, t);
        int path_len = str->len;
        int index = string_cmp(str, arr);
        if(index < path_len) return tp;

        next = trie_path_next(t);
        tp_next = trie_delete_rec(ti, next, len - path_len, &arr[path_len]);

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

trie_p trie_insert_rec(value_info_p vi, trie_p t, char len, char arr[], value_p value)
{
    if(t == NULL)
    {
        t = trie_leaf_create(vi, value);
        return trie_path_create(len, arr, t);
    }

    if(t->type == LEAF)
    {
        assert(!len);
        memcpy(HP(t), value, vi->size);
        return t;
    }

    if(t->type == PATH)
    {
        int index = string_cmp(&TP(t)->str, arr);
        if(index < TP(t)->str.len) t = trie_path_break(t, index);
        if(t->type == PATH)
        {
            TP(t)->next = trie_insert_rec(vi, TP(t)->next, len-index, &arr[index], value);
            return t;
        }
    }

    int key = arr[0];
    trie_p t_next = trie_insert_rec(vi, TF(t)->next[key], len-1, &arr[1], value);
    trie_fork_connect(t, t_next, key);
    return t;
}

value_p trie_querie_rec(trie_p t, char len, char arr[])
{
    if(t == NULL) return NULL;

    switch (t->type)
    {
        case FORK:
        return trie_querie_rec(TF(t)->next[(int)arr[0]], len-1, &arr[1]);
        
        case PATH:;
        int index = string_cmp(&TP(t)->str, arr);
        if(index < TP(t)->str.len) return 0;

        return trie_querie_rec(TP(t)->next, len-index, &arr[index]);

        case LEAF:
        return VP(t);
    }
    assert(FALSE);
}



void trie_insert(value_info_p vi, trie_p *t, char arr[], value_p value)
{
    *t = (value != 0) 
        ? trie_insert_rec(vi, *t, LEN, arr, value) 
        : trie_delete_rec(vi, *t, LEN, arr);
}

void trie_delete(value_info_p vi, trie_p *t, char arr[])
{
    *t = trie_delete_rec(vi, *t, LEN, arr);
}

value_p trie_querie(trie_p t, char arr[])
{
    return trie_querie_rec(t, LEN, arr);
}



void trie_free(trie_p t)
{
    if(t == NULL) return;

    switch (t->type)
    {
        case FORK:
        for(int i=0; i<MAX; i++)
            trie_free(TF(t)->next[i]);
        break;
    
        case PATH:
        trie_free(TP(t)->next);
        break;
    }
    trie_free_single(t);
}
