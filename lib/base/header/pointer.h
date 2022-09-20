#ifndef __BASE_TRIE_H__
#define __BASE_TRIE_H__

#include "../../utils/struct.h"

typedef void *handler_p;
PLACEHOLDER(pointer);
PLACEHOLDER(value_info);

typedef int(*int_ptr_f)(pointer_p);
typedef void(*void_ptr_f)(pointer_p);
typedef handler_p(*value_ptr_f)(pointer_p);
typedef pointer_p(*ptr_value_int_f)(handler_p,int);

STRUCT(pointer_info)
{
    int size;
    void_ptr_f  display;

    value_ptr_f     get;
    ptr_value_int_f set;
    void_ptr_f      free;

};

#endif
