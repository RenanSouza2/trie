#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "lib/lib_my_string.h"

#define STRUCT(NAME)                    \
    typedef struct _##NAME NAME##_t;    \
    typedef NAME##_t *NAME##_p;         \
    struct _##NAME

#define T(POINTER)  ((trie_p)(POINTER))
#define TF(POINTER) ((trie_fork_p)(POINTER))
#define TP(POINTER) ((trie_path_p)(POINTER))
#define TL(POINTER) ((trie_leaf_p)(POINTER))

#define FORK 0
#define PATH 1
#define LEAF 2

#define MAX 10
#define LEN 8


STRUCT(trie)
{
    int type;
};

STRUCT(trie_fork)
{
    trie_t t;
    int connected, least;
    trie_p next[MAX];
};

STRUCT(trie_path)
{
    trie_t t;
    trie_p next;
    string_t str;
};

STRUCT(trie_leaf)
{
    trie_t t;
    int value;
};

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
        printf("\nnext: ");pointer_display(TP(t)->next);
        string_display(&(TP(t)->str));
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

trie_p trie_fork_create()
{
    trie_fork_p t = calloc(1, sizeof(trie_fork_t));
    assert(t);
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
    memcpy(&(t->str.arr), arr, len);
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

trie_p trie_path_break(trie_p t, char len)
{
    assert(len < TP(t)->str.len);

    string_p str = &(TP(t)->str);
    char *arr = str->arr;

    trie_p t_aux = TP(t)->next;
    t_aux = trie_path_create(str->len - len - 1, &(arr[len+1]), t_aux);
    t_aux = trie_path_create(1, &(arr[len]), t_aux);
    return  trie_path_create(len, arr, t_aux);
}



trie_p trie_delete_rec(trie_p t, char len, char arr[])
{
    return NULL;
}

trie_p trie_insert_rec(trie_p t, char len, char arr[], int value)
{
    if(t == NULL)
    {
        t = trie_leaf_create(value);
        return trie_path_create(len, arr, t);
    }

    if(t->type == PATH)
    {
        int index = string_cmp(&(TP(t)->str), len, arr);
        if(index < TP(t)->str.len) t = trie_path_break(t, index);
        if(t->type == PATH)
        {
            TP(t)->next = trie_insert_rec(TP(t)->next, len - index, &(arr[index]), value);
            return t;
        }
    }

    int key = arr[0];
    TF(t)->next[key] = trie_insert_rec(TF(t)->next[key], len - 1, &(arr[1]), value);
    return t;
}

void trie_insert(trie_p *t, char arr[], int value)
{
    if(value == 0)  *t = trie_delete_rec(*t, LEN, arr);
    else            *t = trie_insert_rec(*t, LEN, arr, value);
}



int main()
{
    setbuf(stdout, NULL);
    
    trie_p t = NULL;

    char arr[] = {1, 2, 3, 4, 5, 6, 7, 8};
    trie_insert(&t, arr, 1);
    
    arr[2] = 0;
    trie_insert(&t, arr, 2);
    
    arr[4] = 0;
    trie_insert(&t, arr, 3);
    
    arr[2] = 3;
    trie_insert(&t, arr, 4);

    trie_display(t);


    printf("\n");
    return 0;
}