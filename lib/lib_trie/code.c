#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "header.h"
#include "../lib_my_string/header.h"


void pointer_display(void *p)
{
    if(p == NULL)   printf("NULL");
    else            printf("%p", p);
}



void trie_display_single(trie_p t) {
    printf("\ntrie: ");pointer_display(t);
    if(t == NULL) return;

    switch (t->type)
    {
        case FORK:
        printf("\t(FORK)");
        printf("\nconnected: %d\tleast: %d", TF(t)->connected, TF(t)->least);
        for(int i=0; i<MAX; i++)
        if(TF(t)->next[i])  
            printf("\n\t%d: %p", i, TF(t)->next[i]);
        break;
    
        case PATH:
        printf("\t(PATH)");
        string_display(&TP(t)->str);
        printf("\nnext: ");pointer_display(TP(t)->next);
        break;

        case LEAF:
        printf("\t(LEAF)");
        printf("\nvalue: %d", TL(t)->value);
        break;
    }
}

void trie_display_rec(trie_p t, int len, int res[])
{
    switch(t->type)
    {
        case FORK:
        for(int i=0; i<MAX; i++)
        if(TF(t)->next[i])
        {
            res[len] = i;
            trie_display_rec(TF(t)->next[i], len+1, res);
        }
        break;

        case PATH:
        {
            for(int i=0; i<TP(t)->str.len; i++)
                res[len + i] = TP(t)->str.arr[i];
            trie_display_rec(TP(t)->next, len + TP(t)->str.len, res);
        }
        break;

        case LEAF:
        printf("\n");
        for(int i=0; i<len; i++)
            printf(" %2d", res[i]);
        printf("\t->\t%d", TL(t)->value);
        break;
    }
}

void trie_display(trie_p t)
{
    if(t == NULL)
    {
        printf("\nEMPTY TRIE");
    }
    else
    {
        int res[LEN];
        trie_display_rec(t, 0, res);
    }
    printf("\n");
}



void trie_fork_connect(trie_p t1, trie_p t2, int key)
{
    assert(t1->type == FORK);

    if(!(TF(t1)->next[key]))
    {
        (TF(t1)->connected)++;
        if(key < TF(t1)->least) TF(t1)->least = key;
    }
    TF(t1)->next[key] = t2;
}

void trie_fork_disconnect(trie_p t, int key)
{
    assert(t->type == FORK);
    assert(TF(t)->next[key] != NULL);

    TF(t)->next[key] = NULL;
    (TF(t)->connected)--;
    if(key != TF(t)->least) return;

    int i;
    for(i=TF(t)->least; i<MAX && !(TF(t)->next[i]); i++);
    TF(t)->least = i;
}



trie_p trie_fork_create()
{
    trie_fork_p t = calloc(1, sizeof(trie_fork_t));
    assert(t);
    t->least = MAX;
    return T(t);
}

trie_p trie_path_create_force(char len, char arr[], trie_p next)
{
    assert(len > 0);

    trie_path_p t = malloc(sizeof(trie_path_t) - 7 + len);
    assert(t);

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

trie_p trie_leaf_create(int value)
{
    trie_leaf_p t = malloc(sizeof(trie_leaf_t));
    assert(t);

    *t = (trie_leaf_t){{LEAF}, value};
    return (trie_p)t;
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

    free(t);
    return t_aux;
}

trie_p trie_fork_convert(trie_p t)
{
    assert(t->type == FORK);
    assert(TF(t)->connected == 1);

    int key = TF(t)->least;
    trie_p t_next = TF(t)->next[key];
    free(t);

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

    free(t2);
    return t1;
}



trie_p trie_delete_rec(trie_p t, char len, char arr[])
{
    if(t == NULL) return NULL;

    trie_p t_next;
    switch (t->type)
    {
        case FORK:;
        int key = arr[0];
        trie_p t_bef = TF(t)->next[key];
        trie_p t_aft = trie_delete_rec(t_bef, len-1, &arr[1]);
        if(t_bef == t_aft) return t;

        if(t_aft == NULL)  
        {
            trie_fork_disconnect(t, key);
            if(!TF(t)->connected)
            {
                free(t);
                return NULL;
            }
        }
        
        TF(t)->next[key] = t_aft;
        t_next = TF(t)->next[TF(t)->least];
        break;
    
        case PATH:;
        int path_len = TP(t)->str.len;
        int index = string_cmp(&TP(t)->str, arr);
        if(index < path_len) return t;

        t_bef = TP(t)->next;
        t_aft = trie_delete_rec(t_bef, len-path_len, &arr[path_len]);
        if(t_bef == t_aft) return t;

        if(t_aft == NULL)
        {
            free(t);
            return NULL;
        }
        t_next = TP(t)->next = t_aft;
        break;

        case LEAF:
        assert(!len);
        free(t);
        return NULL;
    }

    if(trie_joinnable(t))
    if(trie_joinnable(t_next))
        return trie_join(t, t_next);
    return t;
}

trie_p trie_insert_rec(trie_p t, char len, char arr[], int value)
{
    if(t == NULL)
    {
        t = trie_leaf_create(value);
        return trie_path_create(len, arr, t);
    }

    if(t->type == LEAF)
    {
        assert(!len);
        TL(t)->value = value;
        return t;
    }

    if(t->type == PATH)
    {
        int index = string_cmp(&TP(t)->str, arr);
        if(index < TP(t)->str.len) t = trie_path_break(t, index);
        if(t->type == PATH)
        {
            TP(t)->next = trie_insert_rec(TP(t)->next, len-index, &arr[index], value);
            return t;
        }
    }

    int key = arr[0];
    trie_p t_next = trie_insert_rec(TF(t)->next[key], len-1, &arr[1], value);
    trie_fork_connect(t, t_next, key);
    return t;
}

int trie_querie_rec(trie_p t, char len, char arr[])
{
    if(t == NULL) return 0;

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
        return TL(t)->value;
    }
    assert(FALSE);
}



void trie_insert(trie_p *t, char arr[], int value)
{
    *t = (value != 0) 
        ? trie_insert_rec(*t, LEN, arr, value) 
        : trie_delete_rec(*t, LEN, arr);
}

void trie_delete(trie_p *t, char arr[])
{
    *t = trie_delete_rec(*t, LEN, arr);
}

int trie_querie(trie_p t, char arr[])
{
    return trie_querie_rec(t, LEN, arr);
}
