#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "header.h"
#include "../lib_my_string/header.h"
#include "../lib_base_header/trie.h"
#include "../lib_base_header/value.h"
#include "../lib_base_header/pointer.h"

#define HP(POINTER) ((char*)(((trie_p)(POINTER))+1))
#define FN(POINTER, INDEX) ((pointer_p)(HP(POINTER) + (INDEX) * ti->pi->size))
#define PS(POINTER) ((string_p)(HP(POINTER) + ti->pi->size))
#define PN(POINTER) ((pointer_p)HP(POINTER))
#define LV(POINTER) ((value_p)HP(POINTER))

#define FALSE 0
#define TRUE  1

#define FORK 0
#define PATH 1
#define LEAF 2

#define MAX 10
#define LEN 8


#define DEBUG //remove after
#ifdef DEBUG

STRUCT(trie)
{
    int type, connected;
};

int pointer_is_null(pointer_info_p pi, pointer_p p)
{
    return (p == NULL) ? TRUE : pi->is_null(p);
}

void trie_display_single(trie_info_p ti, pointer_p tp) 
{
    printf("\ntrie: ");
    ti->pi->display(tp);
    if(ti->pi->is_null(tp)) return;

    trie_p t = (trie_p)ti->pi->get_value(tp);
    switch (t->type)
    {
        case FORK:
        printf("\t(FORK)");
        printf("\nconnected: %d", t->connected);
        for(int i=0; i<MAX; i++)
        {
            pointer_p next = FN(t, i);
            if(ti->pi->is_null(next)) continue;

            printf("\n\t%d: ", i);
            ti->pi->display(next);
        }
        break;
    
        case PATH:
        printf("\t(PATH)");
        string_display(PS(t));
        printf("\nnext: ");
        ti->pi->display(PN(t));
        break;

        case LEAF:
        printf("\t(LEAF)");
        ti->vi->print(LV(t));
        break;
    }
    printf("\n");
}

void trie_display_structure_rec(value_info_p vi, trie_p t)
{
    if(t == NULL) return;

    trie_display_single(vi, t);
    switch (t->type)
    {
        case FORK:
        for(int i=0; i<MAX; i++)
            trie_display_structure_rec(vi, TF(t)->next[i]);
        break;

        case PATH:
        trie_display_structure_rec(vi, TP(t)->next);
        break;
    }
}

void trie_display_structure(value_info_p vi, trie_p t)
{
    if(t == NULL)
    {
        printf("\nEmpty trie");
        return;
    }

    trie_display_structure_rec(vi, t);
}

#else
#define trie_free_single(POINTER) free(POINTER)
#endif 



void trie_display_rec(value_info_p vi, trie_p t, int len, char res[])
{
    switch(t->type)
    {
        case FORK:
        for(int i=0; i<MAX; i++)
        if(TF(t)->next[i])
        {
            res[len] = i;
            trie_display_rec(vi, TF(t)->next[i], len+1, res);
        }
        break;

        case PATH:
        memcpy(&res[len], &(TP(t)->str.arr), TP(t)->str.len);
        trie_display_rec(vi, TP(t)->next, len + TP(t)->str.len, res);
        break;

        case LEAF:
        printf("\n");
        for(int i=0; i<len; i++)
            printf(" %2d", res[i]);
        printf("\t->\t"); vi->value_print(VP(t));
        break;
    }
}

void trie_display(value_info_p vi, trie_p t)
{
    char res[LEN];
    if(t == NULL)   printf("\nEMPTY TRIE");
    else            trie_display_rec(vi, t, 0, res);
    printf("\n");
}



trie_p trie_fork_connect(trie_p t1, trie_p t2, int key)
{
    assert(t1->type == FORK);

    if(!(TF(t1)->next[key]))
    {
        (TF(t1)->connected)++;
        if(key < TF(t1)->least) TF(t1)->least = key;
    }
    TF(t1)->next[key] = t2;
    return t1;
}

trie_p trie_fork_disconnect(trie_p t, int key)
{
    assert(t->type == FORK);
    assert(TF(t)->next[key] != NULL);

    TF(t)->next[key] = NULL;
    (TF(t)->connected)--;
    if(key != TF(t)->least) return t;

    int i;
    for(i=TF(t)->least; i<MAX && !(TF(t)->next[i]); i++);
    TF(t)->least = i;
    return t;
}



trie_p trie_fork_create()
{
    trie_fork_p t = calloc(1, sizeof(trie_fork_t));
    assert(t);
#ifdef DEBUG            
    trie_allocated++;       
#endif

    t->least = MAX;
    return T(t);
}

trie_p trie_path_create_force(char len, char arr[], trie_p next)
{
    assert(len > 0);

    trie_path_p t = malloc(sizeof(trie_path_t) - 7 + len);
    assert(t);
#ifdef DEBUG            
    trie_allocated++;       
#endif

    t->t.type = PATH;
    t->next = next;
    t->str.len = len;
    memcpy(t->str.arr, arr, len);
    return T(t);
}

trie_p trie_path_create(char len, char arr[], trie_p next)
{
    if(len == 0) return next;

    if(len == 1)
    {
        trie_p t = trie_fork_create();
        trie_fork_connect(t, next, arr[0]);
        return t;
    }

    return trie_path_create_force(len, arr, next);
}

trie_p trie_leaf_create(value_info_p vi, value_p value)
{
    int value_size = vi->value_size(value);
    trie_leaf_p t = malloc(sizeof(trie_leaf_t) + value_size);
    assert(t);
#ifdef DEBUG            
    trie_allocated++;       
#endif

    T(t)->type = LEAF;
    memcpy(VP(t), value, value_size);
    free(value);

    return T(t);
}



int trie_joinnable(trie_p t)
{
    if(t->type == PATH) return TRUE;
    if(t->type == LEAF) return FALSE;
    return TF(t)->connected == 1;
}

trie_p trie_path_break(trie_p t, int len)
{
    assert(len < TP(t)->str.len);

    string_p str = &TP(t)->str;
    char *arr = str->arr;

    trie_p t_aux = TP(t)->next;
    t_aux = trie_path_create(str->len - len-1, &arr[len+1], t_aux);
    t_aux = trie_path_create(1, &arr[len], t_aux);
    t_aux = trie_path_create(len, arr, t_aux);

    trie_free_single(t);
    return t_aux;
}

trie_p trie_fork_convert(trie_p t)
{
    assert(t->type == FORK);
    assert(TF(t)->connected == 1);

    int key = TF(t)->least;
    trie_p t_next = TF(t)->next[key];
    trie_free_single(t);

    char arr[] = {key};
    return trie_path_create_force(1, arr, t_next);
}

trie_p trie_join(trie_p t1, trie_p t2)
{
    if(t1->type == FORK) t1 = trie_fork_convert(t1);
    if(t2->type == FORK) t2 = trie_fork_convert(t2);

    int len1 = TP(t1)->str.len;
    int len2 = TP(t2)->str.len;
    int len  = len1 + len2;
    t1 = realloc(t1, sizeof(trie_path_t) - 7 + len);
    TP(t1)->str.len = len;
    TP(t1)->next = TP(t2)->next;
    memcpy(&TP(t1)->str.arr[len1], TP(t2)->str.arr, len2);

    trie_free_single(t2);
    return t1;
}



trie_p trie_delete_rec(value_info_p vi, trie_p t, char len, char arr[])
{
    if(t == NULL) return NULL;

    trie_p t_next;
    switch (t->type)
    {
        case FORK:;
        int key = arr[0];
        trie_p tn = trie_delete_rec(vi, TF(t)->next[key], len-1, &arr[1]);

        if(tn == NULL)  
        {
            trie_fork_disconnect(t, key);
            if(!TF(t)->connected)
            {
                trie_free_single(t);
                return NULL;
            }
        }
        
        TF(t)->next[key] = tn;
        t_next = TF(t)->next[TF(t)->least];
        break;
    
        case PATH:;
        int path_len = TP(t)->str.len;
        int index = string_cmp(&TP(t)->str, arr);
        if(index < path_len) return t;

        tn = trie_delete_rec(vi, TP(t)->next, len-path_len, &arr[path_len]);

        if(tn == NULL)
        {
            trie_free_single(t);
            return NULL;
        }
        t_next = TP(t)->next = tn;
        break;

        case LEAF:
        assert(!len);
        trie_free_single(t);
        return NULL;
    }

    if(trie_joinnable(t))
    if(trie_joinnable(t_next))
        return trie_join(t, t_next);
    return t;
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
        memcpy(VP(t), value, vi->value_size(value));
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
        assert(!len);
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
