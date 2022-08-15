#ifndef __BASE_VALUE_H__
#define __BASE_VALUE_H__

#include "../../utils/struct.h"

PLACEHOLDER(value);

typedef int(*int_value_f)(value_p);
typedef void(*void_value_f)(value_p);

STRUCT(value_info)
{
    int_value_f     size;
    int_value_f     is_null;
    void_value_f    print;
};

#endif
