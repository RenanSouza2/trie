#ifndef __BASE_TRIE_H__
#define __BASE_TRIE_H__

#include "../struct.h"

PLACEHOLDER(pointer);
PLACEHOLDER(value_info);

typedef int(*int_ptr_f)(pointer_p);
typedef void(*void_ptr_f)(pointer_p);
typedef void*(*value_ptr_f)(pointer_p);
typedef pointer_p(*ptr_value_int_f)(void*,int);
typedef pointer_p(*ptr_ptr_value_int_f)(pointer_p,void*,int);

STRUCT(pointer_info)
{
    int size;
    pointer_p null;
    
    void_ptr_f  display;
    int_ptr_f   is_null;

    value_ptr_f         get;
    ptr_value_int_f     set;
    void_ptr_f          delete;
    ptr_ptr_value_int_f replace;

    void_ptr_f  free;
};

#endif