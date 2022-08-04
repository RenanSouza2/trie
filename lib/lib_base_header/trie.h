#ifndef __BASE_TRIE_H__
#define __BASE_TRIE_H__

#include "../struct.h"

PLACEHOLDER(value);
typedef int(*int_value_f)(value_p);
typedef void(*void_value)(value_p);

STRUCT(trie_info)
{
    
    int_value_f value_size;
    void_value value_print;
};

#endif