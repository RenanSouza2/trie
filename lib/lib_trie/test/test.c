#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "../code.c"

// STRUCT(trie_fork)
// {
//     trie_t t;
//     int connected, least;
//     trie_p next[MAX];
// };

// STRUCT(trie_path)
// {
//     trie_t t;
//     trie_p next;
//     string_t str;
// };


int str_test(string_p str, int len, char arr[])
{
    assert(str->len == len);
    for(int i=0; i<len; i++)
        assert(str->arr[i] == arr[i]);
}



void test_create()
{
    trie_p t = trie_fork_create();
    assert(t->type == FORK);
    assert(TF(t)->connected == 0);
    assert(TF(t)->least == MAX);
    for(int i=0; i<MAX; i++)
        assert(TF(t)->next[i] == NULL);
    free(t);

    char arr[2] = {1, 2};
    trie_p t1 = t;
    for(int i=1; i<=2; i++)
    {
        t = trie_path_create_force(i, arr, t1);
        assert(t->type == PATH);
        assert(TP(t)->next == t1);
        str_test(&(TP(t)->str), i, arr);
        free(t);
    }

    t = trie_path_create(0, arr, t1);
    assert(t == t1);

    t = trie_path_create(1, arr, t1);
    assert(t->type == FORK);
    assert(TF(t)->connected == 1);
    assert(TF(t)->least == 1);
    assert(TF(t)->next[1] == t1);
    free(t);

    t = trie_path_create(2, arr, t1);
    assert(t->type == PATH);
    assert(TP(t)->next == t1);
    str_test(&(TP(t)->str), 2, arr);
    free(t);

    t = trie_leaf_create(1);
    assert(t->type == LEAF);
    assert(TL(t)->value == 1);
    free(t);
}

void test_connection()
{
    trie_p t = trie_fork_create();
    
    trie_fork_connect(t, t, 5);
    assert(TF(t)->connected == 1);
    assert(TF(t)->least == 5);
    assert(TF(t)->next[5] == t);
    
    trie_fork_connect(t, t+1, 5);
    assert(TF(t)->connected == 1);
    assert(TF(t)->next[5] == t+1);

    trie_fork_connect(t, t, 8);
    assert(TF(t)->connected == 2);
    assert(TF(t)->least == 5);
    assert(TF(t)->next[8] == t);

    trie_fork_connect(t, t, 3);
    assert(TF(t)->connected == 3);
    assert(TF(t)->least == 3);
    assert(TF(t)->next[3] == t);

    trie_fork_disconnect(t, 3);
    assert(TF(t)->connected == 2);
    assert(TF(t)->least == 5);
    assert(TF(t)->next[3] == NULL);
    
    trie_fork_disconnect(t, 8);
    assert(TF(t)->connected == 1);
    assert(TF(t)->least == 5);
    assert(TF(t)->next[8] == NULL);

    trie_fork_disconnect(t, 5);
    assert(TF(t)->connected == 0);
    assert(TF(t)->least == MAX);
    assert(TF(t)->next[5] == NULL);
    free(t);
}

void test_join()
{
    trie_p t = trie_leaf_create(1);
    assert(trie_joinnable(t) == FALSE);
    free(t);

    t = trie_fork_create();
    trie_fork_connect(t, t, 5);
    assert(trie_joinnable(t) == TRUE);

    trie_fork_connect(t, t, 6);
    assert(trie_joinnable(t) == FALSE);
    free(t);

    char arr[2] = {1, 2};
    t = trie_path_create(2, arr, t);
    assert(trie_joinnable(t) == TRUE);
    free(t);
}

void test_trie()
{
    test_create();
    test_connection();
    test_join();
}

int main() {
    setbuf(stdout, NULL);
    test_trie();
    printf("\nTest successful\n");
    return 0;
}
