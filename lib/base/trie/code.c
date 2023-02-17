#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "debug.h"
#include "../../utils/string/header.h"



#define PTR_CPY(POINTER1, POINTER2) (memcpy(POINTER1, POINTER2, PI->size));
#define PTR_CLEAN(POINTER) (memset(POINTER, 0, PI->size))
#define PTR_NULL(POINTER) (memory_is_null(POINTER, PI->size))

#define FORK_SIZE (sizeof(trie_t) + ti->max * PI->size)
#define PATH_SIZE(LENGTH) (sizeof(trie_t) + PI->size + string_size(LENGTH))
#define LEAF_SIZE(SIZE) (sizeof(trie_t) + SIZE)

#define FALSE 0
#define TRUE  1



#ifdef DEBUG

#define DEF_C(TYPE) \
    int TYPE##_created = 0; \
    int TYPE##_freed = 0

DEF_C(value);
DEF_C(pointer);
DEF_C(fork);
DEF_C(path);
DEF_C(leaf);

#define DIFF(TYPE) (TYPE##_created - TYPE##_freed)

#define PRINT_DIFF(TYPE) printf("\n%s_diff\t: %3d\t", #TYPE, DIFF(TYPE))

#define PRINT(TYPE) {                                           \
        printf("\n");                                           \
        printf("\n%s_created : %3d"  , #TYPE, TYPE##_created);  \
        printf("\n%s_freed   : %3d\t", #TYPE, TYPE##_freed);    \
        printf("\n----------------------");                     \
        printf("\n%s_diff    : %3d\t", #TYPE, DIFF(TYPE));      \
    }
#define ASSERT(TYPE) assert(DIFF(TYPE) == 0);

void assert_memory()
{
    ASSERT(value);
    ASSERT(pointer);
    ASSERT(fork);
    ASSERT(path);
    ASSERT(leaf);
}

void snapshot(char s[])
{
    printf("\n\n\n\tsnapshot: %s", s);

    PRINT_DIFF(pointer);
    PRINT_DIFF(value);
    PRINT_DIFF(fork);
    PRINT_DIFF(path);
    PRINT_DIFF(leaf);
}

void snapshot_complete(char s[])
{
    printf("\n\n\n\tsnapshot: %s", s);

    PRINT(value);
    PRINT(pointer);
    PRINT(fork);
    PRINT(path);
    PRINT(leaf);
}
#endif

int memory_is_null(handler_p h, int len)
{
    for(int i=0; i<len; i++)
        if(((char*)h)[i])
            return FALSE;

    return TRUE;
}

pointer_p pointer_copy(pointer_info_p pi, pointer_p p)
{
    pointer_p p_new = malloc(pi->size);
    assert(p_new);
    INC(pointer);

    memcpy(p_new, p, pi->size);
    return p_new;
}

int trie_fork_first_key(trie_info_p ti, trie_p t)
{
    for(int i=0; i < ti->max; i++)
    {
        pointer_p next = FN(t, i);
        if(!PTR_NULL(next)) return i;
    }
    assert(FALSE);
}

void char_display(unsigned char c)
{
    printf("%x%x", c >> 4, c & 15);
}

void bits_display(int size, char bits[])
{
    for(int i=0; i<size; i++)
        char_display(bits[i]);
}

typedef void(*print_f)(int,handler_p);

#ifdef DEBUG
void trie_display_single(trie_info_p ti, trie_p t, print_f print) 
{
    printf("\ntrie: %p", (void*)t);
    switch (t->type)
    {
        case FORK:
        printf("\t(FORK)");
        printf("\nconnected: %d", t->connected);
        for(int i=0; i < ti->max; i++)
        {
            pointer_p next = FN(t, i);
            if(PTR_NULL(next)) continue;

            printf("\n\t%d: ", i);
            PI->display(next);
        }
        break;
    
        case PATH:
        printf("\t(PATH)");
        string_display(PS(t));
        printf("\nnext: ");
        PI->display(PN(t));
        break;

        case LEAF:
        printf("\t(LEAF)");
        printf("\nvalue: ");
        print = print ? print : (print_f)bits_display;
        print(t->connected, LV(t)); 
        break;
    }
    printf("\n");
}

void trie_display_structure(trie_info_p ti, pointer_p tp, print_f print)
{
    trie_p t = PI->get(tp);
    trie_display_single(ti, t, print);
    switch (t->type)
    {
        case FORK:
        for(int i=0; i < ti->max; i++)
        {
            pointer_p next = FN(t, i);
            if(PTR_NULL(next)) continue;

            trie_display_structure(ti, next, print);
        }
        break;

        case PATH:
        trie_display_structure(ti, PN(t), print);
        break;
    }
}

void root_display_structure(root_p r, print_f print)
{
    trie_info_p ti = r->ti;
    if(r->tp == NULL || PTR_NULL(r->tp)) printf("\nEmpty trie");
    else                                 trie_display_structure(r->ti, r->tp, print);
}

#endif



char key_to_char(int key)
{
    switch (key)
    {
        case 0 ... 9 : return key + '0';
        default: return key;
    }
}

void trie_display(trie_info_p ti, pointer_p tp, int len, char res[], print_f print)
{
    trie_p t = PI->get(tp);
    switch(t->type)
    {
        case FORK:
        for(int i=0; i < ti->max; i++)
        {
            pointer_p next = FN(t, i);
            if(PTR_NULL(next)) continue;

            res[len] = i;
            trie_display(ti, next, len+1, res, print);
        }
        break;

        case PATH:;
        string_p str = PS(t);
        memcpy(&res[len], str->arr, str->len);
        trie_display(ti, PN(t), len + str->len, res, print);
        break;

        case LEAF:
        printf("\n");
        for(int i=0; i<len; i++)
            printf("%c", key_to_char(res[i]));
        printf("  ->  ");
        print(t->connected, (char*)LV(t));
        break;
    }
}



pointer_p trie_fork_create(trie_info_p ti, char key, pointer_p tp_next)
{
    int size = FORK_SIZE;
    trie_p t = malloc(size);
    assert(t);
    INC(fork);

    t->type = FORK;
    t->connected = 1;

    for(int i=0; i < ti->max; i++)
        PTR_CLEAN(FN(t, i));

    PTR_CPY(FN(t, key), tp_next);
    free(tp_next);
    DEC(pointer);

    return PI->set(t, size);
}

pointer_p trie_path_create_force(trie_info_p ti, char len, char arr[], pointer_p tp_next)
{
    assert(len > 0);

    int size = PATH_SIZE(len);
    trie_p t = calloc(1, size);
    assert(t);
    INC(path);

    t->type = PATH;

    pointer_p next = PN(t);
    PTR_CPY(next, tp_next);
    free(tp_next);
    DEC(pointer);

    string_p str = PS(t);
    str->len = len;
    memcpy(str->arr, arr, len);

    return PI->set(t, size);
}

pointer_p trie_path_create(trie_info_p ti, char len, char arr[], pointer_p tp_next)
{
    switch (len)
    {
        case 0: return tp_next;
        case 1: return trie_fork_create(ti, arr[0], tp_next);
    }

    return trie_path_create_force(ti, len, arr, tp_next);
}

pointer_p trie_leaf_create(trie_info_p ti, value_p value)
{
    int size = LEAF_SIZE(value->size);
    trie_p t = calloc(1, size);
    assert(t);
    INC(leaf);

    t->type = LEAF;
    t->connected = value->size;
    memcpy(LV(t), value->ptr, value->size);
    free(value->ptr);
    free(value);
    DEC(value);

    return PI->set(t, size);
}



pointer_p trie_fork_disconnect(trie_info_p ti, pointer_p tp, int key)
{
    trie_p t_old = PI->get(tp);
    assert(t_old->type == FORK);

    if(PTR_NULL(FN(t_old, key))) return tp;
    if(t_old->connected == 1)
    {
        PI->free(tp);
        return NULL;
    }
    int size = FORK_SIZE;
    trie_p t = malloc(size);
    assert(t);
    INC(fork);
    memcpy(t, t_old, size);
    PI->free(tp);

    t->connected--;
    PTR_CLEAN(FN(t, key));

    return PI->set(t, size);
}

pointer_p trie_fork_connect(trie_info_p ti, pointer_p tp, int key, pointer_p tp_next)
{
    if(tp_next == NULL) return trie_fork_disconnect(ti, tp, key);

    trie_p t_old = PI->get(tp);
    assert(t_old->type == FORK);

    int size = FORK_SIZE;
    trie_p t = malloc(size);
    assert(t);
    INC(fork);
    memcpy(t, t_old, size);
    PI->free(tp);

    pointer_p next = FN(t, key);
    if(PTR_NULL(next)) t->connected++;

    PTR_CPY(next, tp_next);
    free(tp_next);
    DEC(pointer);
    
    return PI->set(t, size);
}

pointer_p trie_path_connect(trie_info_p ti, pointer_p tp, pointer_p tp_next)
{
    if(tp_next == NULL)
    {
        PI->free(tp);
        return NULL;
    }

    trie_p t_old = PI->get(tp);
    int size = PATH_SIZE(PS(t_old)->len);
    trie_p t = malloc(size);
    assert(t);
    INC(path);
    memcpy(t, t_old, size);
    PI->free(tp);

    PTR_CPY(PN(t), tp_next);
    free(tp_next);
    DEC(pointer);

    return PI->set(t, size);
}

pointer_p trie_leaf_set_value(trie_info_p ti, pointer_p tp, value_p value)
{
    PI->free(tp);
    return trie_leaf_create(ti, value);
}




int trie_joinnable(trie_p t)
{
    if(t->type == PATH) return TRUE;
    if(t->type == LEAF) return FALSE;
    return t->connected == 1;
}

pointer_p trie_path_break(trie_info_p ti, pointer_p tp, int len)
{
    trie_p t = PI->get(tp);
    string_p str = PS(t);
    assert(len < str->len);

    char *arr = str->arr;

    pointer_p t_aux = pointer_copy(PI, PN(t));
    t_aux = trie_path_create(ti, str->len - len-1, &arr[len+1], t_aux);
    t_aux = trie_fork_create(ti, arr[len], t_aux);
    t_aux = trie_path_create(ti, len, arr, t_aux);

    PI->free(tp);
    return t_aux;
}

pointer_p trie_fork_convert(trie_info_p ti, pointer_p tp)
{
    trie_p t = PI->get(tp);
    if(t->type == PATH) return tp;

    assert(t->type == FORK);
    assert(t->connected == 1);

    char key = trie_fork_first_key(ti, t);
    pointer_p t_next = pointer_copy(PI, FN(t, key));
    PI->free(tp);

    return trie_path_create_force(ti, 1, &key, t_next);
}

pointer_p trie_join(trie_info_p ti, pointer_p tp1, pointer_p tp2)
{
    tp2 = pointer_copy(PI, tp2);

    tp1 = trie_fork_convert(ti, tp1);
    tp2 = trie_fork_convert(ti, tp2);
    trie_p t1 = PI->get(tp1);
    trie_p t2 = PI->get(tp2);

    string_p str1 = PS(t1);
    string_p str2 = PS(t2);
    int len1 = str1->len;
    int len2 = str2->len;
    int len  = len1 + len2;

    char arr[len];
    memcpy(arr, str1->arr, len1);
    memcpy(&arr[len1], str2->arr, len2);
    
    pointer_p tp_next = pointer_copy(PI, PN(t2));
    pointer_p tp = trie_path_create(ti, len, arr, tp_next);
    PI->free(tp1);
    PI->free(tp2);
    return tp;
}



pointer_p trie_delete(trie_info_p ti, pointer_p tp, char arr[])
{
    if(tp && PTR_NULL(tp))
    {
        free(tp);
        DEC(pointer);
        tp = NULL;
    }

    if(tp == NULL) return NULL;

    pointer_p tp_next;
    trie_p t = PI->get(tp);
    switch (t->type)
    {
        case FORK:;
        int key = arr[0];

        tp_next = pointer_copy(PI, FN(t, key));
        tp_next = trie_delete(ti, tp_next, &arr[1]);
        tp = trie_fork_connect(ti, tp, key, tp_next);
        if(tp == NULL) return NULL;

        t = PI->get(tp);
        if(t->connected > 1) return tp;

        key = trie_fork_first_key(ti, t);
        tp_next = FN(t, key);
        break;
    
        case PATH:;
        string_p str = PS(t);
        int path_len = str->len;
        int index = string_cmp(str, arr);
        if(index < path_len) return tp;

        tp_next = pointer_copy(PI, PN(t));
        tp_next = trie_delete(ti, tp_next, &arr[path_len]);
        tp = trie_path_connect(ti, tp, tp_next);
        if(tp == NULL) return NULL;

        t = PI->get(tp);
        tp_next = PN(t);
        break;

        case LEAF:
        PI->free(tp);
        return NULL;
    }

    t = PI->get(tp_next);
    return (trie_joinnable(t)) ? trie_join(ti, tp, tp_next) : tp;
}

pointer_p trie_insert(trie_info_p ti, pointer_p tp, char len, char arr[], value_p value)
{
    if(tp && PTR_NULL(tp))
    {
        free(tp);
        DEC(pointer);
        tp = NULL;
    }

    if(tp == NULL) 
    {
        tp = trie_leaf_create(ti, value);
        return trie_path_create(ti, len, arr, tp);
    }

    trie_p t = PI->get(tp);
    if(t->type == LEAF) return trie_leaf_set_value(ti, tp, value);

    if(t->type == PATH)
    {
        string_p str = PS(t);
        int index = string_cmp(str, arr);
        if(index < str->len) 
        {
            tp = trie_path_break(ti, tp, index);
            t  = PI->get(tp);
        }
        
        if(t->type == PATH)
        {
            pointer_p tp_next = pointer_copy(PI, PN(t));
            tp_next = trie_insert(ti, tp_next, len-index, &arr[index], value);
            tp = trie_path_connect(ti, tp, tp_next);
            return tp;
        }
    }

    int key = arr[0];
    pointer_p tp_next = pointer_copy(PI, FN(t, key));
    tp_next = trie_insert(ti, tp_next, len-1, &arr[1], value);
    return trie_fork_connect(ti, tp, key, tp_next);
}

handler_p trie_querie(trie_info_p ti, pointer_p tp, char arr[])
{
    if(tp == NULL || PTR_NULL(tp)) return NULL;

    trie_p t = PI->get(tp);
    switch (t->type)
    {
        case FORK:
        return trie_querie(ti, FN(t, arr[0]), &arr[1]);
        
        case PATH:;
        string_p str = PS(t);
        int index = string_cmp(str, arr);
        if(index < str->len) return NULL;

        return trie_querie(ti, PN(t), &arr[index]);

        case LEAF:
        return LV(t);
    }
    assert(FALSE);
}

void trie_free(trie_info_p ti, pointer_p tp)
{
    if(tp == NULL) return;

    trie_p t = PI->get(tp);
    switch (t->type)
    {
        case FORK:
        for(int i=0; i < ti->max; i++)
        {
            pointer_p next = FN(t, i);
            if(PTR_NULL(next)) continue;
            
            next = pointer_copy(PI, next);
            trie_free(ti, next);
        }
        break;
    
        case PATH:;
        pointer_p next = pointer_copy(PI, PN(t));
        trie_free(ti, next);
        break;
    }
    PI->free(tp);
}



char key_to_value(char key)
{
    switch (key)
    {
        case '0' ... '9' : return key - '0';
        case 'a' ... 'z' : return key - 'a';
        case 'A' ... 'Z' : return key - 'A';
        default: assert(FALSE);
    }
}

trie_info_p get_trie_info(int max, pointer_info_p pi)
{
    trie_info_p ti = malloc(sizeof(trie_info_t));
    assert(ti);

    *ti = (trie_info_t) {max, pi};
    return ti;
}



root_p root_init(pointer_info_p pi, int len, int max)
{
    root_p r = malloc(sizeof(root_t));
    assert(r);

    trie_info_p ti = get_trie_info(max, pi);
    *r = (root_t){len, ti, NULL};
    return r;
}

void root_insert(root_p r, char const arr[], value_p value)
{
    char arr_input[r->len];
    for(int i=0; i<r->len; i++)
        arr_input[i] = key_to_value(arr[i]);
    
    r->tp = memory_is_null(value->ptr, value->size) 
        ? trie_delete(r->ti, r->tp, arr_input)
        : trie_insert(r->ti, r->tp, r->len, arr_input, value);
}

void root_delete(root_p r, char const arr[])
{
    char arr_input[r->len];
    for(int i=0; i<r->len; i++)
        arr_input[i] = key_to_value(arr[i]);
    
    r->tp = trie_delete(r->ti, r->tp, arr_input);
}

handler_p root_querie(root_p r, char const arr[])
{
    char arr_input[r->len];
    for(int i=0; i<r->len; i++)
        arr_input[i] = key_to_value(arr[i]);
    
    return trie_querie(r->ti, r->tp, arr_input);
}

void root_display(root_p r, handler_p print)
{
    char res[r->len];
    if(r->tp == NULL) printf("\nEMPTY TRIE");
    else if(print)    trie_display(r->ti, r->tp, 0, res, (print_f)print);
    else              trie_display(r->ti, r->tp, 0, res, (print_f)bits_display);
    printf("\n");
}

void root_free(root_p r)
{
    trie_free(r->ti, r->tp);
    r->tp = NULL;
}
