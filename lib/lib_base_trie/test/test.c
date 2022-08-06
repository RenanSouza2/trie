#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "../../lib_value_int/header.h"
#include "../../lib_pointer_mem/header.h"
#include "../../lib_trie_info/header.h"
#include "../code.c"

#define PTR_CMP(POINTER1, POINTER2) (memcmp(POINTER1, POINTER2, PI->size) == 0)

trie_info_p ti;

void assert_str(string_p str, int len, char arr[])
{
    assert(str->len == len);
    for(int i=0; i<len; i++)
        assert(str->arr[i] == arr[i]);
}

void assert_fork(pointer_p tp, int key, long ptr)
{
    assert(tp != NULL);

    trie_p t = PI->get(tp);
    assert(t != NULL);
    assert(t->type == FORK);

    pointer_p tp_next = PI->get(FN(t, key));
    if(ptr) assert(tp_next == (void*)ptr);
    else    assert(tp_next != NULL);
}

void assert_path(pointer_p tp, long ptr, int len, char arr[])
{
    assert(tp != NULL);

    trie_p t = PI->get(tp);
    assert(t != NULL);
    assert(t->type == PATH);

    pointer_p tp_next = PI->get(PN(t));
    if(ptr) assert(tp_next == (void*)ptr);
    else    assert(tp_next != NULL);
    assert_str(PS(t), len, arr);
}

void assert_leaf(pointer_p tp, int value)
{
    assert(tp != NULL);

    trie_p t = PI->get(tp);
    assert(t != NULL);
    assert(t->type == LEAF);
    assert(*(int*)LV(t) == value);
}



pointer_p get_pointer(long i)
{
    void *v;
    v = (void*)i;
    return PI->set(v, 0);
}

void test_create()
{

    printf("\n\ttest_create");
    long ptr = 1;
    pointer_p tp_next = get_pointer(ptr);
    pointer_p tp = trie_fork_create(ti, 0, tp_next);
    trie_p t = PI->get(tp);
    assert(t->connected == 1);
    assert_fork(tp, 0, ptr);
    for(int i=1; i<MAX; i++)
        assert(PTR_CMP(FN(t, i), PI->null));
    PI->free(tp);

    char arr[2] = {1, 2};
    for(int i=1; i<=2; i++)
    {
        ptr = 2;
        tp_next = get_pointer(ptr);
        tp = trie_path_create_force(ti, i, arr, tp_next);
        assert_path(tp, ptr, i, arr);
        PI->free(tp);
    }

    ptr = 3;
    tp_next = get_pointer(ptr);
    tp = trie_path_create(ti, 0, arr, tp_next);
    assert(tp == (pointer_p)ptr);

    ptr = 4;
    tp_next = get_pointer(ptr);
    tp = trie_path_create(ti, 1, arr, tp_next);
    assert_fork(tp, 1, ptr);
    PI->free(tp);

    ptr = 5;
    tp_next = get_pointer(ptr);
    tp = trie_path_create(ti, 2, arr, tp_next);
    assert_path(tp, ptr, 2, arr);
    PI->free(tp);

    int value = 1;
    value_p v = set_int(value);
    tp = trie_leaf_create(ti, v);
    assert_leaf(tp, value);
    PI->free(tp);
}

void test_connection()
{
    printf("\n\ttest_connection");
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
    PI->free(t);
}

void test_joinable()
{
    printf("\n\ttest_joinable");
    int value = 1;

    value_info_p vi = get_int_value_info();
    value_p vp = set_int(value);
    trie_p t = trie_leaf_create(vi, vp);
    assert(trie_joinnable(t) == FALSE);
    PI->free(t);

    t = trie_fork_create();
    trie_fork_connect(t, t, 5);
    assert(trie_joinnable(t) == TRUE);

    trie_fork_connect(t, t, 6);
    assert(trie_joinnable(t) == FALSE);
    PI->free(t);

    char arr[2] = {1, 2};
    t = trie_path_create(2, arr, t);
    assert(trie_joinnable(t) == TRUE);
    PI->free(t);
}

void test_path_break()
{
    printf("\n\ttest_path_break");
    trie_p tn = T(1);
    char arr[3] = {1, 2, 3};

    trie_p t = trie_path_create(3, arr, tn);
    t = trie_path_break(t, 0);
    assert(t->type == FORK);

    trie_p t1 = TF(t)->next[1];
    assert_path(t1, tn, 2, &arr[1]);
    PI->free(t1);
    PI->free(t);

    t = trie_path_create(3, arr, tn);
    t = trie_path_break(t, 1);
    assert(t->type == FORK);

    t1 = TF(t)->next[1];
    assert_fork(t1, 2, NULL);

    t1 = TF(t1)->next[2];
    assert_fork(t1, 3, NULL);
    assert(TF(t1)->next[3] == tn);
    PI->free(t1);
    PI->free(TF(t)->next[1]);
    PI->free(t);

    t = trie_path_create(3, arr, tn);
    t = trie_path_break(t, 2);
    assert(t->type == PATH);

    t1 = TP(t)->next;
    assert_fork(t1, 3, tn);
    PI->free(t1);
    PI->free(t);
}

void test_fork_convert()
{
    printf("\n\ttest_fork_convert");

    trie_p tn = T(1);
    char key = 5;

    trie_p t = trie_fork_create();
    trie_fork_connect(t, tn, key);
    t = trie_fork_convert(t);
    assert_path(t, tn, 1, &key);
    PI->free(t);
}

void test_join()
{
    printf("\n\ttest_join");

    trie_p tn1 = T(1);
    trie_p tn2 = T(2);
    char arr[4] = {1, 2, 3, 4};

    trie_p t1 = trie_path_create(2, arr, tn1);
    trie_p t2 = trie_path_create(2, &arr[2], tn2);
    trie_p t = trie_join(t1, t2);
    assert_path(t, tn2, 4, arr);
    PI->free(t);

    t1 = trie_fork_create();
    t2 = trie_path_create(2, &arr[2], tn2);
    trie_fork_connect(t1, t2, 2);
    t = trie_join(t1, t2);
    assert_path(t, tn2, 3, &arr[1]);
    PI->free(t);
    
    t1 = trie_path_create(2, arr, tn1);
    t2 = trie_fork_create();
    trie_fork_connect(t2, tn2, 3);
    t = trie_join(t1, t2);
    assert_path(t, tn2, 3, arr);
    PI->free(t);
}

void test_unit()
{
    printf("\ntest_unit");
    test_create();
    test_connection();
    test_joinable();
    test_path_break();
    test_fork_convert();
    test_join();

    assert_memory();
}



void test_insert()
{
    printf("\n\ttest_insert");
    char arr[8] = {0, 1, 2, 3, 4, 5, 6, 7};
    value_info_p vi = get_int_value_info();

    trie_p t = NULL;

    value_p value = set_int(1);
    trie_insert(vi, &t, arr, value);
    assert_path(T(t), NULL, 8, arr);

    trie_p t1 = TP(t)->next;
    assert_leaf(t1, 1);

    //////////////////

    value = set_int(2);
    trie_insert(vi, &t, arr, value);
    t1 = TP(t)->next;
    assert_leaf(t1, 2);

    //////////////////

    arr[4] = 0;
    value = set_int(3);
    trie_insert(vi, &t, arr, value);
    assert_path(T(t), NULL, 4, arr);

    t1 = TP(t)->next;
    assert_fork(t1, 0, NULL);

    t1 = TF(t1)->next[0];
    assert_path(t1, NULL, 3, &arr[5]);
    assert_leaf(TP(t1)->next, 3);
    
    //////////////////

    arr[0] = 1;
    value = set_int(4);
    trie_insert(vi, &t, arr, value);
    assert_fork(T(t), 1, NULL);

    t1 = TF(t)->next[1];
    assert_path(t1, NULL, 7, &arr[1]);
    assert_leaf(TP(t1)->next, 4);
    
    //////////////////

    arr[0] = 2;
    value = set_int(5);
    trie_insert(vi, &t, arr, value);
    assert_fork(T(t), 2, NULL);

    t1 = TF(t)->next[2];
    assert_path(t1, NULL, 7, &arr[1]);
    assert_leaf(TP(t1)->next, 5);

    trie_free(t);
    free(vi);
}

void test_querie()
{
    printf("\n\ttest_querie");
    value_info_p vi = get_int_value_info();
    char arr[8] = {0, 1, 2, 3, 4, 5, 6, 7};
    trie_p t = NULL;

    value_p value = trie_querie(t, arr);
    int res = get_int(value);
    assert(res == 0);

    value = set_int(1);
    trie_insert(vi, &t, arr, value);
    value = trie_querie(t, arr);
    res = get_int(value);
    assert(res == 1);

    arr[4] = 0;
    value = set_int(2);
    trie_insert(vi, &t, arr, value);
    value = trie_querie(t, arr);
    res = get_int(value);
    assert(res == 2);

    arr[2] = 0;
    value = trie_querie(t, arr);
    res = get_int(value);
    assert(res == 0);
    
    trie_free(t);
    free(vi);
}

void test_delete_1()
{
    printf("\n\ttest_delete_1");
    value_info_p vi = get_int_value_info();
    trie_p t = NULL;
    char arr[8] = {0, 1, 2, 3, 4, 5, 6, 7};
    value_p value = set_int(1);
    trie_insert(vi, &t, arr, value);

    arr[4] = 0;
    value = set_int(2);
    trie_insert(vi, &t, arr, value);

    arr[5] = 0;
    value = set_int(3);
    trie_insert(vi, &t, arr, value);

    arr[6] = 0;
    value = set_int(4);
    trie_insert(vi, &t, arr, value);

    arr[6] = 1;
    value = set_int(5);
    trie_insert(vi, &t, arr, value);
    trie_delete(vi, &t, arr);
    arr[6] = 0;

    trie_p t1 = TP(t)->next;
    t1 = TF(t1)->next[0];
    t1 = TF(t1)->next[0];
    assert_fork(t1, 0, NULL);
    assert(TF(t1)->next[1] == NULL);
    
    trie_delete(vi, &t, arr);
    arr[6] = 6;
    
    t1 = TP(t)->next;
    t1 = TF(t1)->next[0];
    t1 = TF(t1)->next[0];
    assert_path(t1, NULL, 2, &arr[6]);

    trie_delete(vi, &t, arr);
    arr[5] = 5;
    assert_path(T(t), NULL, 4, arr);

    t1 = TP(t)->next;
    assert_fork(t1, 0, NULL);
    assert(TF(t1)->connected == 2);

    t1 = TF(t1)->next[0];
    assert_path(t1, NULL, 3, &arr[5]);

    trie_delete(vi, &t, arr);
    arr[4] = 4;
    assert_path(T(t), NULL, 8, arr);

    arr[4] = 1;
    trie_delete(vi, &t, arr);
    arr[4] = 4;
    assert_path(T(t), NULL, 8, arr);

    trie_delete(vi, &t, arr);
    assert(t == NULL);

    trie_delete(vi, &t, arr);
    assert(t == NULL);
    free(vi);
}

void test_delete_2()
{
    printf("\n\ttest_delete_2");
    value_info_p vi = get_int_value_info();
    char arr[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    
    trie_p t = NULL;
    value_p value = set_int(1);
    trie_insert(vi, &t, arr, value);

    arr[5] = 1;
    value = set_int(2);
    trie_insert(vi, &t, arr, value);

    arr[7] = 1;
    value = set_int(3);
    trie_insert(vi, &t, arr, value);
    trie_delete(vi, &t, arr);

    assert_path(T(t), NULL, 5, arr);
    trie_p t1 = TP(t)->next;
    assert_fork(t1, 1, NULL);
    t1 = TF(t1)->next[1];
    assert_path(t1, NULL, 2, arr);
    
    trie_free(t);
}

void test_integration()
{
    printf("\n\ntest_integration");
    test_insert();
    test_querie();

    test_delete_1();
    test_delete_2();

    assert_memory();
}



void test_trie()
{
    printf("\ntest_trie");
    test_unit();
    test_integration();

    assert_memory();
}



int main() 
{
    setbuf(stdout, NULL);

    value_info_p vi = get_int_value_info();
    pointer_info_p pi = get_mem_info();
    ti = get_trie_info(vi, pi);

    test_trie();
    assert_memory();
    printf("\n\n\t\tTest successful\n\n");
    return 0;
}
