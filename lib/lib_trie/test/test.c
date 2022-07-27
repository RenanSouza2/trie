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


void assert_str(trie_p t, int len, char arr[])
{
    assert(TP(t)->str.len == len);
    for(int i=0; i<len; i++)
        assert(TP(t)->str.arr[i] == arr[i]);
}

void assert_fork(trie_p t, int key, trie_p tn)
{
    assert(t != NULL);
    assert(t->type == FORK);
    assert(TF(t)->least <= key);
    if(tn == NULL)  assert(TF(t)->next[key] != NULL);
    else            assert(TF(t)->next[key] == tn);
}

void assert_path(trie_p t, trie_p tn, int len, char arr[])
{
    assert(t != NULL);
    assert(t->type == PATH);
    if(tn != NULL) assert(TP(t)->next == tn);
    assert_str(t, len, arr);
}

void assert_leaf(trie_p t, int value)
{
    assert(t != NULL);
    assert(t->type == LEAF);
    assert(TL(t)->value == value);
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
        assert_path(t, t1, i, arr);
        free(t);
    }

    t = trie_path_create(0, arr, t1);
    assert(t == t1);

    t = trie_path_create(1, arr, t1);
    assert_fork(t, 1, t1);
    assert(TF(t)->connected == 1);
    assert(TF(t)->least == 1);
    free(t);

    t = trie_path_create(2, arr, t1);
    assert_path(t, t1, 2, arr);
    free(t);

    t = trie_leaf_create(1);
    assert_leaf(t, 1);
    free(t);
}

void test_connection()
{
    trie_p t = trie_fork_create();
    
    trie_fork_connect(t, t, 5);
    assert_fork(t, 5, t);
    assert(TF(t)->connected == 1);
    assert(TF(t)->least == 5);
    
    trie_fork_connect(t, t+1, 5);
    assert(TF(t)->connected == 1);
    assert(TF(t)->next[5] == t+1);

    trie_fork_connect(t, t, 8);
    assert_fork(t, 8, t);
    assert(TF(t)->connected == 2);
    assert(TF(t)->least == 5);

    trie_fork_connect(t, t, 3);
    assert_fork(t, 8, t);
    assert(TF(t)->connected == 3);
    assert(TF(t)->least == 3);

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

void test_joinable()
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

void test_path_break()
{
    trie_p tn = T(1);
    char arr[3] = {1, 2, 3};

    trie_p t = trie_path_create(3, arr, tn);
    t = trie_path_break(t, 0);
    assert(t->type == FORK);

    trie_p t1 = TF(t)->next[1];
    assert_path(t1, tn, 2, &arr[1]);
    free(t1);
    free(t);

    t = trie_path_create(3, arr, tn);
    t = trie_path_break(t, 1);
    assert(t->type == FORK);

    t1 = TF(t)->next[1];
    assert_fork(t1, 2, NULL);

    t1 = TF(t1)->next[2];
    assert_fork(t1, 3, NULL);
    assert(TF(t1)->next[3] == tn);
    free(t1);
    free(TF(t)->next[1]);
    free(t);

    t = trie_path_create(3, arr, tn);
    t = trie_path_break(t, 2);
    assert(t->type == PATH);

    t1 = TP(t)->next;
    assert_fork(t1, 3, tn);
    free(t1);
    free(t);
}

void test_fork_convert()
{
    trie_p tn = T(1);
    char key = 5;

    trie_p t = trie_fork_create();
    trie_fork_connect(t, tn, key);
    t = trie_fork_convert(t);
    assert_path(t, tn, 1, &key);
}

void test_join()
{
    trie_p tn1 = T(1);
    trie_p tn2 = T(2);
    char arr[4] = {1, 2, 3, 4};

    trie_p t1 = trie_path_create(2, arr, tn1);
    trie_p t2 = trie_path_create(2, &arr[2], tn2);
    trie_p t = trie_join(t1, t2);
    assert_path(t, tn2, 4, arr);

    t1 = trie_fork_create();
    t2 = trie_path_create(2, &arr[2], tn2);
    trie_fork_connect(t1, t2, 2);
    t = trie_join(t1, t2);
    assert_path(t, tn2, 3, &arr[1]);
    
    t1 = trie_path_create(2, arr, tn1);
    t2 = trie_fork_create();
    trie_fork_connect(t2, tn2, 3);
    t = trie_join(t1, t2);
    assert_path(t, tn2, 3, arr);
}

void test_path_operation()
{
    test_joinable();
    test_path_break();
    test_fork_convert();
    test_join();
}


// trie_p trie_insert_rec(trie_p t, char len, char arr[], int value)
// {
//     if(t == NULL)
//     {
//         t = trie_leaf_create(value);
//         return trie_path_create(len, arr, t);
//     }
//
//     if(t->type == LEAF)
//     {
//         assert(!len);
//         TL(t)->value = value;
//         return t;
//     }
//
//     if(t->type == PATH)
//     {
//         int index = string_cmp(&TP(t)->str, arr);
//         if(index < TP(t)->str.len) t = trie_path_break(t, index);
//         if(t->type == PATH)
//         {
//             TP(t)->next = trie_insert_rec(TP(t)->next, len-index, &arr[index], value);
//             return t;
//         }
//     }
//
//     char key = arr[0];
//     trie_p t_next = trie_insert_rec(TF(t)->next[key], len-1, &arr[1], value);
//     trie_fork_connect(t, t_next, key);
//     return t;
// }


void test_insert()
{
    char arr[8] = {0, 1, 2, 3, 4, 5, 6, 7};

    trie_p t = NULL;
    trie_insert(&t, arr, 1);
    assert_path(t, NULL, 8, arr);

    trie_p t1 = TP(t)->next;
    assert_leaf(t1, 1);

    //////////////////

    trie_insert(&t, arr, 2);
    t1 = TP(t)->next;
    assert_leaf(t1, 2);

    //////////////////

    arr[4] = 0;
    trie_insert(&t, arr, 3);
    assert_path(t, NULL, 4, arr);

    t1 = TP(t)->next;
    assert_fork(t1, 0, NULL);

    t1 = TF(t1)->next[0];
    assert_path(t1, NULL, 3, &arr[5]);
    assert_leaf(TP(t1)->next, 3);
    
    //////////////////

    arr[0] = 1;
    trie_insert(&t, arr, 4);
    assert_fork(t, 1, NULL);

    t1 = TF(t)->next[1];
    assert_path(t1, NULL, 7, &arr[1]);
    assert_leaf(TP(t1)->next, 4);
    
    //////////////////

    arr[0] = 2;
    trie_insert(&t, arr, 5);
    assert_fork(t, 2, NULL);

    t1 = TF(t)->next[2];
    assert_path(t1, NULL, 7, &arr[1]);
    assert_leaf(TP(t1)->next, 5);
}

void test_delete()
{

}

void test_integration()
{
    test_insert();
    test_delete();
}

void test_trie()
{
    test_create();
    test_connection();
    test_path_operation();
    test_integration();
}

int main() {
    setbuf(stdout, NULL);
    test_trie();
    printf("\tTest successful\n");
    return 0;
}
