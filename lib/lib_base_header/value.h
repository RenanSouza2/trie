#ifndef __BASE_VALUE_H__
#define __BASE_VALUE_H__


#include "../struct.h"
#include "../lib_my_string/header.h"

PLACEHOLDER(value);
typedef int(*int_value_f)(value_p);
typedef void(*void_value)(value_p);

STRUCT(value_info)
{
    int_value_f value_size;
    void_value value_print;
};

#endif