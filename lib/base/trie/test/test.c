#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "../../../value/int/header.h"
#include "../../../pointer/mem/header.h"
#include "../code.c"

pointer_info_p pi_test;
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
    printf("\n\ttest_create\t\t");
    long ptr = 1;
    pointer_p tp_next = get_pointer(ptr);

    pointer_p tp = trie_fork_create(ti, 0, tp_next);
    trie_p t = PI->get(tp);
    assert(t->connected == 1);
    assert_fork(tp, 0, ptr);
    for(int i=1; i < ti->max; i++)
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

    assert_memory();
}

void test_connection()
{
    printf("\n\ttest_connect\t\t");

    printf("\n\t\ttest_connect_1\t\t");

    long ptr = 1;
    pointer_p tp_next =  get_pointer(ptr);
    pointer_p tp = trie_fork_create(ti, 5, tp_next);
    trie_p t = PI->get(tp);
    assert(t->connected == 1);
    assert_fork(tp, 5, ptr);

    /////////////////////////////////
    printf("\n\t\ttest_connect_2\t\t");
    
    ptr = 2;
    tp_next = get_pointer(ptr);
    tp = trie_fork_connect(ti, tp, 5, tp_next);
    t = PI->get(tp);
    assert(t->connected == 1);
    assert_fork(tp, 5, ptr);
    
    /////////////////////////////////
    printf("\n\t\ttest_connect_3\t\t");

    ptr = 3;
    tp_next =  get_pointer(ptr);
    tp = trie_fork_connect(ti, tp, 8, tp_next);
    assert_fork(tp, 8, ptr);
    t = PI->get(tp);
    assert(t->connected == 2);

    /////////////////////////////////
    printf("\n\t\ttest_connect_4\t\t");

    ptr = 4;
    tp_next =  get_pointer(ptr);
    tp = trie_fork_connect(ti, tp, 3, tp_next);
    assert_fork(tp, 3, ptr);
    t = PI->get(tp);
    assert(t->connected == 3);

    /////////////////////////////////
    printf("\n\ttest_disconnect\t\t");

    printf("\n\t\ttest_disconnect_1\t\t");

    tp = trie_fork_disconnect(ti, tp, 3);
    t = PI->get(tp);
    assert(t->connected == 2);
    assert(PI->get(FN(t, 3)) == NULL);

    /////////////////////////////////
    printf("\n\t\ttest_disconnect_2\t\t");
    
    tp = trie_fork_disconnect(ti, tp, 8);
    t = PI->get(tp);
    assert(t->connected == 1);
    assert(PI->get(FN(t, 8)) == NULL);

    /////////////////////////////////
    printf("\n\t\ttest_disconnect_3\t\t");
    
    tp = trie_fork_disconnect(ti, tp, 5);
    assert(tp == NULL);
    
    assert_memory();
}

void test_connection_2()
{
    printf("\n\ttest_connection_2\t\t");
    
    printf("\n\t\ttest_connection_2_1\t\t");

    long ptr = 1;
    pointer_p tp_next = get_pointer(ptr);
    pointer_p tp = trie_fork_create(ti, 5, tp_next);
    
    tp = trie_fork_connect(ti, tp, 5, NULL);
    assert(tp == NULL);

    /////////////////////////////////
    printf("\n\t\ttest_connection_2_2\t\t");

    char arr[] = {1, 2};
    tp_next =  get_pointer(ptr);
    tp = trie_path_create(ti, 2, arr, tp_next);
    
    ptr = 2;
    tp_next = get_pointer(ptr);
    tp = trie_path_connect(ti, tp, tp_next);

    trie_p t = PI->get(tp);
    assert(*(long*)PN(t) == ptr);

    /////////////////////////////////
    printf("\n\t\ttest_connection_2_3\t\t");

    tp = trie_path_connect(ti, tp, NULL);
    assert(tp == NULL);

}

void test_joinable()
{
    printf("\n\ttest_joinable\t\t");

    printf("\n\t\ttest_joinable_1\t\t");

    long ptr = 1;
    pointer_p tp_next = get_pointer(ptr);

    pointer_p tp = trie_fork_create(ti, 8, tp_next);
    trie_p t = PI->get(tp);
    assert(trie_joinnable(t) == TRUE);

    /////////////////////////////////
    printf("\n\t\ttest_joinable_2\t\t");

    ptr = 2;
    tp_next = get_pointer(ptr);
    tp = trie_fork_connect(ti, tp, 6, tp_next);
    assert(trie_joinnable(t) == FALSE);
    PI->free(tp);

    /////////////////////////////////
    printf("\n\t\ttest_joinable_3\t\t");

    char arr[2] = {1, 2};
    ptr = 3;
    tp_next = get_pointer(ptr);
    tp = trie_path_create(ti, 2, arr, tp_next);
    t  = PI->get(tp);
    int res = trie_joinnable(t);
    assert(res == TRUE);
    PI->free(tp);

    /////////////////////////////////
    printf("\n\t\ttest_joinable_4\t\t");

    int value = 1;
    value_p vp = set_int(value);
    tp = trie_leaf_create(ti, vp);
    assert(trie_joinnable(t) == FALSE);
    PI->free(tp);
    
    assert_memory();
}

void test_path_break()
{
    printf("\n\ttest_path_break\t\t");
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
    
    assert_memory();
}

void test_fork_convert()
{
    printf("\n\ttest_fork_convert\t\t");

    long ptr = 1;
    pointer_p tp_next = get_pointer(ptr);
    
    char key = 5;
    pointer_p tp = trie_fork_create(ti, key, tp_next);
    tp = trie_fork_convert(ti, tp);
    assert_path(tp, 1, 1, &key);
    PI->free(tp);
    
    assert_memory();
}

void test_join()
{
    printf("\n\ttest_join\t\t");

    char arr[4] = {1, 2, 3, 4};

    long ptr = 1;
    pointer_p tp_next = get_pointer(ptr);
    pointer_p tp_1 = trie_path_create(ti, 2, arr, tp_next);
    
    ptr = 2;
    tp_next = get_pointer(ptr);
    pointer_p tp_2 = trie_path_create(ti, 2, &arr[2], tp_next);
    
    pointer_p tp = trie_join(ti, tp_1, tp_2);
    assert_path(tp, 2, 4, arr);
    
    free(tp_2);
    DEC(pointer);
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
    
    free(tp_2);
    DEC(pointer);
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
    
    free(tp_2);
    DEC(pointer);
    PI->free(tp);

    assert_memory();
}

void test_unit()
{
    printf("\ntest_unit\t\t");

    test_create();
    test_connection();
    test_connection_2();
    test_joinable();
    test_path_break();
    test_fork_convert();
    test_join();

    assert_memory();
}



void test_insert()
{
    printf("\n\ttest_insert\t\t");
    char arr[8] = {0, 1, 2, 3, 4, 5, 6, 7};
    root_p r = root_init(pi_test, 8, 10);

    printf("\n\t\ttest_insert_1\t\t");
    value_p value = set_int(1);
    root_insert(r, "01234567", value);
    assert_path(r->tp, 0, 8, arr);

    trie_p t = PI->get(r->tp);
    assert_leaf(PN(t), 1);

    //////////////////

    printf("\n\t\ttest_insert_2\t\t");
    value = set_int(2);
    root_insert(r, "01234567", value);
    t = PI->get(r->tp);
    assert_leaf(PN(t), 2);
    
    //////////////////

    printf("\n\t\ttest_insert_3\t\t");
    arr[4] = 0;
    value = set_int(3);
    root_insert(r, "01230567", value);
    assert_path(r->tp, 0, 4, arr);

    t = PI->get(r->tp);
    pointer_p tp_aux = PN(t);
    assert_fork(tp_aux, 0, 0);

    t = PI->get(tp_aux);
    tp_aux = FN(t, 0);
    assert_path(tp_aux, 0, 3, &arr[5]);

    t = PI->get(tp_aux);
    assert_leaf(PN(t), 3);
    
    //////////////////

    printf("\n\t\ttest_insert_4\t\t");
    arr[0] = 1;
    value = set_int(4);
    root_insert(r, "11230567", value);
    assert_fork(r->tp, 1, 0);

    t = PI->get(r->tp);
    tp_aux = FN(t, 1);
    assert_path(tp_aux, 0, 7, &arr[1]);

    t = PI->get(tp_aux);
    assert_leaf(PN(t), 4);

    //////////////////

    printf("\n\t\ttest_insert_5\t\t");
    arr[0] = 2;
    value = set_int(5);
    root_insert(r, "21230567", value);
    assert_fork(r->tp, 2, 0);

    t = PI->get(r->tp);
    tp_aux = FN(t, 2);
    assert_path(tp_aux, 0, 7, &arr[1]);
    
    t = PI->get(tp_aux);
    assert_leaf(PN(t), 5);

    root_free(r);
    assert_memory();
}

void test_querie()
{
    printf("\n\ttest_querie\t\t");
    root_p r = root_init(pi_test, 8, 10);

    value_p value = root_querie(r, "01234567");
    int res = get_int(value);
    assert(res == 0);

    value = set_int(1);
    root_insert(r, "01234567", value);
    value = root_querie(r, "01234567");
    res = get_int(value);
    assert(res == 1);

    value = set_int(2);
    root_insert(r, "01230567", value);
    value = root_querie(r, "01230567");
    res = get_int(value);
    assert(res == 2);

    value = root_querie(r, "01030567");
    res = get_int(value);
    assert(res == 0);

    root_free(r);
    assert_memory();
}

void test_delete_1()
{
    printf("\n\ttest_delete_1\t\t");
    root_p r = root_init(pi_test, 8, 10);

    
    value_p value = set_int(1);
    root_insert(r, "01234567", value);

    value = set_int(2);
    root_insert(r, "01230567", value);

    value = set_int(3);
    root_insert(r, "01230067", value);

    value = set_int(4);
    root_insert(r, "01230007", value);

    value = set_int(5);
    root_insert(r, "01230017", value);

    printf("\n\t\ttest_delete_1_1\t\t");
    root_delete(r, "01230017");
    
    char arr[8] = { 0, 1, 2, 3, 0, 0, 0, 7 };

    trie_p t = PI->get(r->tp);
    pointer_p tp = PN(t);
    t = PI->get(tp);
    tp = FN(t, 0);
    t = PI->get(tp);
    tp = FN(t, 0);
    assert_fork(tp, 0, 0);

    //////////////////

    printf("\n\t\ttest_delete_1_2\t\t");
    root_delete(r, "01230007");
    arr[6] = 6;

    t = PI->get(r->tp);
    tp = PN(t);
    t = PI->get(tp);
    tp = FN(t, 0);
    t = PI->get(tp);
    tp = FN(t, 0);
    assert_path(tp, 0, 2, &arr[6]);
    

    //////////////////

    printf("\n\t\ttest_delete_1_3\t\t");
    root_delete(r, "01230067");
    arr[5] = 5;
    assert_path(r->tp, 0, 4, arr);

    t = PI->get(r->tp);
    tp = PN(t);
    assert_fork(tp, 0, 0);

    t = PI->get(tp);
    assert(t->connected == 2);

    tp = FN(t, 0);
    assert_path(tp, 0, 3, &arr[5]);

    //////////////////

    printf("\n\t\ttest_delete_1_4\t\t");
    root_delete(r, "01230567");
    arr[4] = 4;
    assert_path(r->tp, 0, 8, arr);

    //////////////////

    printf("\n\t\ttest_delete_1_5\t\t");
    arr[4] = 1;
    root_delete(r, "01231567");

    arr[4] = 4;
    assert_path(r->tp, 0, 8, arr);

    //////////////////

    printf("\n\t\ttest_delete_1_6\t\t");
    root_delete(r, "01234567");
    assert(r->tp == NULL);

    printf("\n\t\ttest_delete_1_7\t\t");
    root_delete(r, "01234567");
    assert(r->tp == NULL);
    
    assert_memory();
}

void test_delete_2()
{
    printf("\n\ttest_delete_2\t\t");
    root_p r = root_init(pi_test, 8, 10);
    


    value_p value = set_int(1);
    root_insert(r, "00000000", value);

    value = set_int(2);
    root_insert(r, "00000100", value);

    value = set_int(3);
    root_insert(r, "00000110", value);
    root_delete(r, "00000110");

    char arr[8] = {0, 0, 0, 0, 0, 1, 1, 0};
    assert_path(r->tp, 0, 5, arr);

    trie_p t = PI->get(r->tp);
    pointer_p tp = PN(t);
    assert_fork(tp, 1, 0);

    t = PI->get(tp);
    tp = FN(t, 1);
    assert_path(tp, 0, 2, arr);

    root_free(r);
    assert_memory();
}

void test_integration()
{
    printf("\n\ntest_integration\t\t");
    test_insert();
    test_querie();

    test_delete_1();
    test_delete_2();
}



void test_trie()
{
    assert_memory();

    test_unit();
    test_integration();
    
    assert_memory();
}



int main() 
{
    
    setbuf(stdout, NULL);

    pi_test = get_mem_info();
    ti = get_trie_info(10, pi_test);

    test_trie();

    printf("\n\n\t\tTest successful\n\n");
    return 0;
}
