#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "../../lib_value_int/header.h"
#include "../../lib_pointer_mem/header.h"
#include "../../lib_trie_info/header.h"
#include "../code.c"

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

    trie_p t_next = PI->get(FN(t, key));
    if(ptr) assert(t_next == (void*)ptr);
    else    assert(t_next != NULL);
}

void assert_path(pointer_p tp, long ptr, int len, char arr[])
{
    assert(tp != NULL);

    trie_p t = PI->get(tp);
    assert(t != NULL);
    assert(t->type == PATH);

    trie_p t_next = PI->get(PN(t));
    if(ptr) assert(t_next == (void*)ptr);
    else    assert(t_next != NULL);
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

    printf("\n\ttest_create\t");
    long ptr = 1;
    pointer_p tp_next = get_pointer(ptr);
    pointer_p tp = trie_fork_create(ti, 0, tp_next);
    trie_p t = PI->get(tp);
    assert(t->connected == 1);
    assert_fork(tp, 0, ptr);
    for(int i=1; i<MAX; i++)
        assert(*(void**)FN(t, i) == NULL);
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
    assert(tp == tp_next);

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
    long ptr = 1;
    pointer_p tp_next =  get_pointer(ptr);

    printf("\n\ttest_connection\t");
    pointer_p tp = trie_fork_create(ti, 5, tp_next);
    
    trie_p t = PI->get(tp);
    assert(t->connected == 1);
    assert_fork(tp, 5, ptr);
    
    ptr = 2;
    tp_next =  get_pointer(ptr);
    tp = trie_fork_create(ti, 5, tp_next);
    t = PI->get(tp);
    assert(t->connected == 1);
    assert_fork(tp, 5, ptr);

    ptr = 3;
    tp_next =  get_pointer(ptr);
    tp = trie_fork_connect(ti, tp, 8, tp_next);
    assert_fork(tp, 8, ptr);
    t = PI->get(tp);
    assert(t->connected == 2);

    ptr = 4;
    tp_next =  get_pointer(ptr);
    tp = trie_fork_connect(ti, tp, 3, tp_next);
    assert_fork(tp, 3, ptr);
    t = PI->get(tp);
    assert(t->connected == 3);

    tp = trie_fork_disconnect(ti, tp, 3);
    t = PI->get(tp);
    assert(t->connected == 2);
    assert(PI->get(FN(t, 3)) == NULL);
    
    tp = trie_fork_disconnect(ti, tp, 8);
    t = PI->get(tp);
    assert(t->connected == 1);
    assert(PI->get(FN(t, 8)) == NULL);

    tp = trie_fork_disconnect(ti, tp, 5);
    assert(tp == NULL);
}

void test_joinable()
{
    printf("\n\ttest_joinable\t");

    long ptr = 1;
    pointer_p tp_next = get_pointer(ptr);

    pointer_p tp = trie_fork_create(ti, 8, tp_next);
    trie_p t = PI->get(tp);
    assert(trie_joinnable(t) == TRUE);

    ptr = 2;
    tp_next = get_pointer(ptr);
    trie_fork_connect(ti, tp, 6, tp_next);
    assert(trie_joinnable(t) == FALSE);
    PI->free(tp);

    char arr[2] = {1, 2};
    ptr = 3;
    get_pointer(ptr);
    tp = trie_path_create(ti, 2, arr, tp_next);
    t  = PI->get(tp);
    assert(trie_joinnable(t) == TRUE);
    PI->free(tp);

    int value = 1;
    value_p vp = set_int(value);
    tp = trie_leaf_create(ti, vp);
    assert(trie_joinnable(t) == FALSE);
    PI->free(tp);
}

void test_path_break()
{
    printf("\n\ttest_path_break\t");
    long ptr = 1;
    pointer_p tp_next = get_pointer(ptr);
    char arr[3] = {1, 2, 3};

    pointer_p tp = trie_path_create(ti, 3, arr, tp_next);
    tp = trie_path_break(ti, tp, 0);
    assert_fork(tp, 1, 0);

    trie_p t = PI->get(tp);
    pointer_p tp_1 = FN(t, 1);
    assert_path(tp_1, ptr, 2, &arr[1]);
    tp_1 = pointer_copy(PI, tp_1);
    PI->free(tp_1);
    PI->free(tp);

    /////////////////////////////////

    ptr = 2;
    tp_next = get_pointer(ptr);
    tp = trie_path_create(ti, 3, arr, tp_next);
    tp = trie_path_break(ti, tp, 1);
    assert_fork(tp, 1, 0);

    t = PI->get(tp);
    tp_1 = FN(t, 1);
    assert_fork(tp_1, 2, 0);

    t = PI->get(tp_1);
    tp_1 = FN(t, 2);
    assert_fork(tp_1, 3, 0);

    tp_1 = pointer_copy(PI, tp_1);
    PI->free(tp_1);
    
    t = PI->get(tp);
    tp_1 = pointer_copy(PI, FN(t, 1));
    PI->free(tp_1);

    PI->free(tp);

    /////////////////////////////////

    ptr = 3;
    tp_next = get_pointer(ptr);
    tp = trie_path_create(ti, 3, arr, tp_next);
    tp = trie_path_break(ti, tp, 2);
    assert_path(tp, 0, 2, arr);

    t = PI->get(tp);
    tp_1 = PN(t);
    assert_fork(tp_1, 3, 0);
    tp_1 = pointer_copy(PI, tp_1);
    PI->free(tp_1);
    PI->free(tp);
}

void test_fork_convert()
{
    printf("\n\ttest_fork_convert\t");

    long ptr = 1;
    pointer_p tp_next = get_pointer(ptr);
    
    char key = 5;
    pointer_p tp = trie_fork_create(ti, key, tp_next);
    tp = trie_fork_convert(ti, tp);
    assert_path(tp, 1, 1, &key);
    PI->free(tp);
}

void test_join()
{
    printf("\n\ttest_join");


    char arr[4] = {1, 2, 3, 4};

    long ptr = 1;
    pointer_p tp_next = get_pointer(ptr);
    pointer_p tp_1 = trie_path_create(ti, 2, arr, tp_next);

    ptr = 2;
    tp_next = get_pointer(ptr);
    pointer_p tp_2 = trie_path_create(ti, 2, &arr[2], tp_next);


    pointer_p tp = trie_join(ti, tp_1, tp_2);
    assert_path(tp, 2, 4, arr);
    PI->free(tp);

    /////////////////////////////////

    ptr = 1;
    tp_next = get_pointer(ptr);
    tp_1 = trie_path_create(ti, 1, &arr[1], tp_next);
    
    ptr = 2;
    tp_next = get_pointer(ptr);
    tp_2 = trie_path_create(ti, 2, &arr[2], tp_next);

    tp = trie_join(ti, tp_1, tp_2);
    assert_path(tp, 2, 3, &arr[1]);
    PI->free(tp);

    /////////////////////////////////

    ptr = 1;
    tp_next = get_pointer(ptr);
    tp_1 = trie_path_create(ti, 2, arr, tp_next);

    ptr = 2;
    tp_next = get_pointer(ptr);
    tp_2 = trie_path_create(ti, 1, &arr[2], tp_next);
    
    tp = trie_join(ti, tp_1, tp_2);
    assert_path(tp, 2, 3, arr);
    PI->free(tp);
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
}



void test_insert()
{
    printf("\n\ttest_insert");
    char arr[8] = {0, 1, 2, 3, 4, 5, 6, 7};

    pointer_p tp = NULL;

    value_p value = set_int(1);
    trie_insert(ti, &tp, arr, value);
    assert_path(tp, 0, 8, arr);

    trie_p t = PI->get(tp);
    assert_leaf(PN(t), 1);

    //////////////////

    value = set_int(2);
    trie_insert(ti, &tp, arr, value);
    t = PI->get(tp);
    assert_leaf(PN(t), 2);

    //////////////////

    arr[4] = 0;
    value = set_int(3);
    trie_insert(ti, &tp, arr, value);
    assert_path(tp, 0, 4, arr);

    t = PI->get(tp);
    pointer_p tp_aux = PN(t);
    assert_fork(tp_aux, 0, 0);

    t = PI->get(tp_aux);
    tp_aux = FN(t, 0);
    assert_path(tp_aux, 0, 3, &arr[5]);

    t = PI->get(tp_aux);
    assert_leaf(PN(t), 3);

    //////////////////

    arr[0] = 1;
    value = set_int(4);
    trie_insert(ti, &tp, arr, value);
    assert_fork(tp, 1, 0);

    t = PI->get(tp);
    tp_aux = FN(t, 1);
    assert_path(tp_aux, 0, 7, &arr[1]);

    t = PI->get(tp_aux);
    assert_leaf(PN(t), 4);

    //////////////////

    arr[0] = 2;
    value = set_int(5);
    trie_insert(ti, &tp, arr, value);
    assert_fork(tp, 2, 0);

    t = PI->get(tp);
    tp_aux = FN(t, 2);
    assert_path(tp_aux, 0, 7, &arr[1]);
    
    t = PI->get(tp_aux);
    assert_leaf(PN(t), 5);

    trie_free(ti, tp);
}

void test_querie()
{
    printf("\n\ttest_querie");
    char arr[8] = {0, 1, 2, 3, 4, 5, 6, 7};
    pointer_p tp = NULL;

    value_p value = trie_querie(ti, tp, arr);
    int res = get_int(value);
    assert(res == 0);

    value = set_int(1);
    trie_insert(ti, &tp, arr, value);
    value = trie_querie(ti, tp, arr);
    res = get_int(value);
    assert(res == 1);

    arr[4] = 0;
    value = set_int(2);
    trie_insert(ti, &tp, arr, value);
    value = trie_querie(ti, tp, arr);
    res = get_int(value);
    assert(res == 2);

    arr[2] = 0;
    value = trie_querie(ti, tp, arr);
    res = get_int(value);
    assert(res == 0);

    trie_free(ti, tp);
}

void test_delete_1()
{
    printf("\n\ttest_delete_1");
    pointer_p tp = NULL;
    char arr[8] = {0, 1, 2, 3, 4, 5, 6, 7};
    value_p value = set_int(1);
    trie_insert(ti, &tp, arr, value);

    arr[4] = 0;
    value = set_int(2);
    trie_insert(ti, &tp, arr, value);

    arr[5] = 0;
    value = set_int(3);
    trie_insert(ti, &tp, arr, value);

    arr[6] = 0;
    value = set_int(4);
    trie_insert(ti, &tp, arr, value);

    arr[6] = 1;
    value = set_int(5);
    trie_insert(ti, &tp, arr, value);
    trie_delete(ti, &tp, arr);
    arr[6] = 0;

    trie_p t = PI->get(tp);
    pointer_p tp_aux = PN(t);
    t = PI->get(tp_aux);
    tp_aux = FN(t, 0);
    t = PI->get(tp_aux);
    tp_aux = FN(t, 0);
    assert_fork(tp_aux, 0, 0);

    //////////////////

    trie_delete(ti, &tp, arr);
    arr[6] = 6;

    t = PI->get(tp);
    tp_aux = PN(t);
    t = PI->get(tp_aux);
    tp_aux = FN(t, 0);
    t = PI->get(tp_aux);
    tp_aux = FN(t, 0);
    assert_path(tp_aux, 0, 2, &arr[6]);

    //////////////////

    trie_delete(ti, &tp, arr);
    arr[5] = 5;
    assert_path(tp, 0, 4, arr);

    t = PI->get(tp);
    tp_aux = PN(t);
    assert_fork(tp_aux, 0, 0);

    t = PI->get(tp_aux);
    assert(t->connected == 2);

    tp_aux = FN(t, 0);
    assert_path(tp_aux, 0, 3, &arr[5]);

    //////////////////

    trie_delete(ti, &tp, arr);
    arr[4] = 4;
    assert_path(tp, 0, 8, arr);

    //////////////////

    arr[4] = 1;
    trie_delete(ti, &tp, arr);
    arr[4] = 4;
    assert_path(tp, 0, 8, arr);

    //////////////////

    trie_delete(ti, &tp, arr);
    assert(tp == NULL);

    trie_delete(ti, &tp, arr);
    assert(tp == NULL);
}

void test_delete_2()
{
    printf("\n\ttest_delete_2");
    char arr[8] = {0, 0, 0, 0, 0, 0, 0, 0};

    pointer_p tp = NULL;
    value_p value = set_int(1);
    trie_insert(ti, &tp, arr, value);

    arr[5] = 1;
    value = set_int(2);
    trie_insert(ti, &tp, arr, value);

    arr[7] = 1;
    value = set_int(3);
    trie_insert(ti, &tp, arr, value);
    trie_delete(ti, &tp, arr);

    assert_path(tp, 0, 5, arr);

    trie_p t = PI->get(tp);
    pointer_p tp_aux = PN(t);
    assert_fork(tp_aux, 1, 0);

    t = PI->get(tp_aux);
    tp_aux = FN(t, 1);
    assert_path(tp_aux, 0, 2, arr);

    trie_free(ti, tp);
}

void test_integration()
{
    printf("\n\ntest_integration");
    test_insert();
    test_querie();

    test_delete_1();
    test_delete_2();
}



void test_trie()
{
    printf("\ntest_trie");
    test_unit();
    test_integration();
}



int main() 
{
    setbuf(stdout, NULL);

    value_info_p vi = get_int_value_info();
    pointer_info_p pi = get_mem_info();
    ti = get_trie_info(vi, pi);

    test_trie();
    printf("\n\n\t\tTest successful\n\n");
    return 0;
}
