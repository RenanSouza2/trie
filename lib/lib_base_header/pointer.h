#ifndef __BASE_TRIE_H__
#define __BASE_TRIE_H__

#include "../struct.h"

PLACEHOLDER(value);
PLACEHOLDER(pointer);
PLACEHOLDER(value_info);

typedef int(*int_pointer_f)(pointer_p);
typedef void(*void_pointer_f)(pointer_p);
typedef value_p(*value_pointer_f)(pointer_p);
typedef pointer_p(*pointer_value_int_f)(value_p,int);

STRUCT(pointer_info)
{
    int size;
    pointer_p null;

    value_pointer_f     get_value;
    pointer_value_int_f set_value;

    void_pointer_f  display;
    int_pointer_f   is_null;
};

#endif