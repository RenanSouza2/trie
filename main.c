#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

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

STRUCT(string)
{
    char len;
    char arr[0];
};

void string_cpy(string_p str1, string_p str2)
{
    memcpy(str1, str2, str2->len + 1);
}

int string_cmp(string_p str1, string_p str2)
{
    int i, len = (str1->len < str2->len) ? str1->len : str2->len;
    for(i=0; i<len && str1->arr[i] == str2->arr[i]; i++);
    return i;
}

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

void trie_fork_display(trie_p t)
{
    printf("\ntrie (FORK): %p", t);
    printf("\nconnected: %d\tleast: %d", TF(t)->connected, TF(t)->least);
    for(int i=0; i<MAX; i++)
    if(TF(t)->next[i])
    {
        printf("\n\t%d: %p", i, TF(t)->next[i]);
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

trie_p trie_fork_create()
{
    trie_fork_p t = calloc(1, sizeof(trie_fork_t));
    assert(t);
    return T(t);
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

trie_p trie_path_create_force(string_p str, trie_p next)
{
    trie_path_p t = malloc(sizeof(trie_path_t) - 7 + str->len);
    assert(t);

    t->t.type = PATH;
    t->next = next;
    string_cpy(&(t->str), str);
    return T(t);
}

trie_p trie_path_create(string_p str, trie_p next)
{
    if(str->len == 0) return next;

    if(str->len == 1)
    {
        trie_p t = trie_fork_create();
        trie_fork_connect(t, next, str->arr[0]);
        return t;
    }

    return trie_path_create_force(str, next);
}

trie_p trie_leaf_create(int value)
{
    trie_leaf_p t = malloc(sizeof(trie_leaf_t));
    assert(t);

    *t = (trie_leaf_t){{LEAF}, value};
    return (trie_p)t;
}

trie_p trie_path_break(trie_p t, int len)
{
    string_p str = &(TP(t)->str);
    char str_aux[2] = {1, str->arr[len]};
    str->arr[len] = str->len - len - 1;
    str->len = len;

    trie_p t_aux = trie_path_create((string_p)(&(str->arr[len])),TP(t)->next);
    t_aux = trie_path_create((string_p)str_aux, t_aux);
    return  trie_path_create(str, t_aux);
}

trie_p trie_delete_rec(trie_p t, string_p str)
{
    return NULL;
}

trie_p trie_insert_rec(trie_p t, string_p str, int value)
{
    if(t == NULL)
    {
        t = trie_leaf_create(value);
        return trie_path_create(str, t);
    }

    if(t->type == PATH)
    {
        int index = string_cmp(&(TP(t)->str), str);
        t = trie_path_break(t, index);
        if(t->type == PATH)
        {
            str->arr[index-1] = str->len - index;
            TP(t)->next = trie_insert_rec(TP(t)->next, (string_p)&(str->arr[index-1]), value);
            return t;
        }
    }

    int key = str->arr[0];
    str->arr[0] = str->len - 1;
    TF(t)->next[key] = trie_insert_rec(TF(t)->next[key], (string_p)&(str->arr), value);
    return t;
}

void trie_insert(trie_p *t, string_p str, int value)
{
    if(value == 0)  *t = trie_delete_rec(*t, str);
    else            *t = trie_insert_rec(*t, str, value);
}



int main()
{
    trie_p t = NULL;

    char arr[] = {8, 1, 2, 3, 4, 5, 6, 7, 8};
    trie_insert(&t, (string_p)arr, 1);
    
    arr[3] = 0;
    trie_insert(&t, (string_p)arr, 2);
    
    arr[5] = 0;
    trie_insert(&t, (string_p)arr, 3);
    
    arr[3] = 3;
    trie_insert(&t, (string_p)arr, 4);

    trie_display(t);


    printf("\n");
    return 0;
}