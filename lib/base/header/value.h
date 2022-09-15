#ifndef __BASE_VALUE_H__
#define __BASE_VALUE_H__

#include "../../utils/struct.h"

STRUCT(value)
{
    int size;
    handler_p ptr;
};

typedef int(*int_value_f)(value_p);
typedef void(*void_value_f)(value_p);

STRUCT(value_info)
{
    int_value_f     size;
    int_value_f     is_null;
    void_value_f    print;
};

#endif
