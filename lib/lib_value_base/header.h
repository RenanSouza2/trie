#ifndef __VALUE_BASE_H__
#define __VALUE_BASE_H__

#include "../struct.h"

STRUCT(value);
typedef void (*void_value)(value_p);
typedef int(*int_value)(value_p);

STRUCT(value_info)
{
    int size;
    value_p null;

    void_value  value_print;
    int_value   value_is_null;
};

#endif