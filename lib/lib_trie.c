#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "lib_trie.h"
#include "lib_my_string.h"


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



void trie_fork_connect(trie_p t1, trie_p t2, char key)
{
    assert(t1->type == FORK);

    if(!(TF(t1)->next[key]))
    {
        (TF(t1)->connected)++;
        if(key < TF(t1)->least) TF(t1)->least = key;
    }
    TF(t1)->next[key] = t2;
}

void trie_fork_disconnect(trie_p t, char key)
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

trie_p trie_path_break(trie_p t, char len)
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

    char key = TF(t)->least;
    trie_p t_next = TF(t)->next[key];
    return trie_path_create_force(1, &key, t_next);
}

trie_p trie_join(trie_p t1, trie_p t2)
{
    if(t1->type == FORK) t1 = trie_fork_convert(t1);
    if(t2->type == FORK) t2 = trie_fork_convert(t2);

    char len1 = TP(t1)->str.len;
    char len2 = TP(t2)->str.len;
    char len  = len1 + len2;
    t1 = realloc(t1, sizeof(trie_path_t) - 7 + len);
    TP(t1)->str.len = len;
    TP(t1)->next = TP(t2)->next;
    memcpy(&TP(t1)->str.arr[len1], TP(t2)->str.arr, len2);

    free(t2);
    return t1;
}



trie_p trie_delete_rec(trie_p t, char len, char arr[])
{
    if(!t) return NULL;

    trie_p t_next;
    switch (t->type)
    {
        case FORK:;
        char key = arr[0];
        trie_p t_bef = TF(t)->next[key];
        if(!t_bef) return t;

        trie_p t_aft = trie_delete_rec(t_bef, len-1, &arr[1]);
        if(t_aft)   TF(t)->next[key] = t_aft;
        else       
        {
            trie_fork_disconnect(t, key);
            if(!TF(t)->connected)
            {
                free(t);
                return NULL;
            }
        }

        t_next = TF(t)->next[TF(t)->least];
        break;
    
        case PATH:;
        int path_len = TP(t)->str.len;
        int index = string_cmp(&TP(t)->str, len, arr);
        if(index < path_len) return t;

        t_next = trie_delete_rec(TP(t)->next, len-path_len, &arr[path_len]);
        if(!t_next)
        {
            free(t);
            return NULL;
        }
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

    char key = arr[0];
    trie_p t_next = trie_insert_rec(TF(t)->next[key], len-1, &arr[1], value);
    trie_fork_connect(t, t_next, key);
    return t;
}

void trie_delete(trie_p *t, char arr[])
{
    *t = trie_delete_rec(*t, LEN, arr);
}

void trie_insert(trie_p *t, char arr[], int value)
{
    *t = value ? trie_insert_rec(*t, LEN, arr, value) : trie_delete_rec(*t, LEN, arr);
}

int trie_querie_rec(trie_p t, char len, char arr[])
{
    if(!t) return 0;

    switch (t->type)
    {
        case FORK:
        return trie_querie_rec(TF(t)->next[arr[0]], len-1, &arr[1]);
        
        case PATH:
        int index = string_cmp(&TP(t)->str, arr);
        if(index < TP(t)->str.len) return 0;

        return trie_querie_rec(TP(t)->next, len-index, &arr[index]);

        case LEAF:
        assert(!len);
        return TL(t)->value;
    }
}

int trie_querie(trie_p t, char arr[])
{
    return trie_querie_rec(t, LEN, arr);
}


void assert_trie_path_string(trie_p t, char arr[], char len)
{   
    assert(t->type == PATH);
    assert(TP(t)->str.len == len);
    assert(!memcmp(TP(t)->str.arr, arr, len));
}

void unit_test()
{
    trie_p t;
    t = trie_fork_create();
    assert(t->type == FORK);
    assert(TF(t)->connected == 0);
    assert(TF(t)->least == MAX);
    for(int i=0; i<MAX; i++)
        assert(TF(t)->next[i] == NULL);

    trie_fork_connect(t, t, 5);
    assert(TF(t)->connected == 1);
    assert(TF(t)->least == 5);
    assert(TF(t)->next[5] == t);
    
    trie_fork_connect(t, t, 7);
    assert(TF(t)->connected == 2);
    assert(TF(t)->least == 5);
    assert(TF(t)->next[7] == t);
    
    trie_fork_connect(t, t, 3);
    assert(TF(t)->connected == 3);
    assert(TF(t)->least == 3);
    assert(TF(t)->next[3] == t);

    trie_fork_disconnect(t, 3);
    assert(TF(t)->connected == 2);
    assert(TF(t)->least == 5);
    assert(TF(t)->next[3] == NULL);

    trie_fork_disconnect(t, 7);
    assert(TF(t)->connected == 1);
    assert(TF(t)->least == 5);
    assert(TF(t)->next[7] == NULL);
    
    trie_fork_disconnect(t, 5);
    assert(TF(t)->connected == 0);
    assert(TF(t)->least == MAX);
    assert(TF(t)->next[5] == NULL);
    free(t);

    trie_p t_next = t;
    char arr[] = {0, 1, 2, 3, 4, 5, 6, 7};
    t = trie_path_create(0, arr, t_next);
    assert(t == t_next);

    t = trie_path_create(1, arr, t_next);
    assert(t->type == FORK);
    assert(TF(t)->next[0] == t_next);
    free(t);

    t = trie_path_create(2, arr, t_next);
    assert(t->type == PATH);
    assert(TP(t)->next == t_next);
    assert(TP(t)->str.len == 2);
    assert(!memcmp(TP(t)->str.arr, arr, 2));
    free(t);

    t = trie_leaf_create(1);
    assert(t->type == LEAF);
    assert(TL(t)->value == 1);
    free(t);

    t = trie_path_create(2, arr, t_next);
    assert(trie_joinnable(t) == TRUE);
    assert_trie_path_string(t, arr, 2);
    free(t);

    t = trie_leaf_create(1);
    assert(trie_joinnable(t) == FALSE);
    free(t);

    t = trie_fork_create();
    trie_fork_connect(t, t, 5);
    assert(trie_joinnable(t) == TRUE);

    trie_fork_connect(t, t, 6);
    assert(trie_joinnable(t) == FALSE);
    free(t);

    trie_p t_leaf = trie_leaf_create(1);
    t = trie_path_create(4, arr, t_leaf);
    t = trie_path_break(t, 0);
    assert(t->type == FORK);
    assert(TF(t)->next[0] != NULL);

    trie_p t_aux = TF(t)->next[0];
    assert(t_aux->type == PATH);
    assert(TP(t_aux)->next == t_leaf);
    assert_trie_path_string(t_aux, &arr[1], 3);
    free(t_aux);
    free(t);

    t = trie_path_create(4, arr, t_leaf);
    t = trie_path_break(t, 1);
    assert(t->type == FORK);
    assert(TF(t)->next[0] != NULL);

    t_aux = TF(t)->next[0];
    assert(t_aux->type == FORK);
    assert(TF(t_aux)->next[1] != NULL);

    t_aux = TF(t_aux)->next[1];
    assert(t_aux->type == PATH);
    assert(TP(t_aux)->next == t_leaf);
    assert_trie_path_string(t_aux, &arr[2], 2);
    free(t_aux);
    free(TF(t)->next[0]);
    free(t);

    t = trie_path_create(4, arr, t_leaf);
    t = trie_path_break(t, 2);
    assert(t->type == PATH);
    assert_trie_path_string(t, arr, 2);

    t_aux = TP(t)->next;
    assert(t_aux->type == FORK);
    assert(TF(t_aux)->next[2] != NULL);

    t_aux = TF(t_aux)->next[2];
    assert(t_aux->type == FORK);
    assert(TF(t_aux)->next[3] == t_leaf);
    free(t_aux);
    free(TP(t)->next);
    free(t);

    t = trie_path_create(4, arr, t_leaf);
    t = trie_path_break(t, 3);
    assert(t->type == PATH);
    assert_trie_path_string(t, arr, 3);

    t_aux = TP(t)->next;
    assert(t_aux->type == FORK);
    assert(TF(t_aux)->next[3] == t_leaf);
    free(t_leaf);
    free(t_aux);
    free(t);
}

void integration_test()
{
    trie_p t = NULL;
    char arr[] = {0, 1, 2, 3, 4, 5, 6, 7};


    trie_insert(&t, arr, 1);
    assert(t->type == PATH);
    assert_trie_path_string(t, arr, 8);

    trie_p t_aux = TP(t)->next;
    assert(t_aux->type == LEAF);
    assert(TL(t_aux)->value == 1);

    trie_insert(&t, arr, 2);
    assert(t->type == PATH);
    t_aux = TP(t)->next;
    assert(t_aux->type == LEAF);
    assert(TL(t_aux)->value == 2);

    arr[2] = 4;
    trie_insert(&t, arr, 3);
    assert(t->type == PATH);
    assert_trie_path_string(t, arr, 2);

    t_aux = TP(t)->next;
    assert(t_aux->type == FORK);
    assert(TF(t_aux)->connected == 2);
    assert(TF(t_aux)->next[2] != NULL);
    assert(TF(t_aux)->next[4] != NULL);

    t_aux = TF(t_aux)->next[4];
    assert(t->type == PATH);
    assert_trie_path_string(t_aux, &arr[3], 5);

    t_aux = TP(t_aux)->next;
    assert(t_aux->type == LEAF);
    assert(TL(t_aux)->value == 3);

    arr[2] = 6;
    trie_insert(&t, arr, 4);
    assert(t->type == PATH);
    assert_trie_path_string(t, arr, 2);

    t_aux = TP(t)->next;
    assert(t_aux->type == FORK);
    assert(TF(t_aux)->connected == 3);
    assert(TF(t_aux)->next[2] != NULL);
    assert(TF(t_aux)->next[4] != NULL);
    assert(TF(t_aux)->next[6] != NULL);

    t_aux = TF(t_aux)->next[6];
    assert(t->type == PATH);
    assert_trie_path_string(t_aux, &arr[3], 5);

    t_aux = TP(t_aux)->next;
    assert(t_aux->type == LEAF);
    assert(TL(t_aux)->value == 4);

    arr[1] = 3;
    trie_delete(&t, arr);

    arr[1] = 1;
    assert(t->type == PATH);
    assert_trie_path_string(t, arr, 2);

    arr[2] = 5;
    trie_delete(&t, arr);
    assert_trie_path_string(t, arr, 2);

    t_aux = TP(t)->next;
    assert(t_aux->type == FORK);
    assert(TF(t_aux)->connected == 3);

    arr[2] = 6;
    trie_delete(&t, arr);
    
    t_aux = TP(t)->next;
    assert(t_aux->type == FORK);
    assert(TF(t_aux)->connected == 2);
    assert(TF(t_aux)->next[6] == NULL);

    arr[2] = 4;
    trie_delete(&t, arr);
    assert(t->type == PATH);

    arr[2] = 2;
    assert_trie_path_string(t, arr, 8);

    trie_delete(&t, arr);
    assert(t == NULL);
}

void test()
{
    setbuf(stdout, NULL);

    unit_test();
    integration_test();

    printf("\nTests successfull\n");
    exit(EXIT_SUCCESS);
}



int main()
{
    test();
    setbuf(stdout, NULL);
        
    // arr[4] = 0;
    // trie_insert(&t, arr, 3);
    // trie_display(t);
    
    // arr[2] = 3;
    // trie_insert(&t, arr, 4);
    // trie_display(t);

    // trie_delete(&t, arr);
    // trie_display(t);

    // printf("\n");
    return 0;
}