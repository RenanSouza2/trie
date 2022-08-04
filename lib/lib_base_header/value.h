#ifndef __BASE_VALUE_H__
#define __BASE_VALUE_H__


#include "../struct.h"
#include "../lib_my_string/header.h"

PLACEHOLDER(value);
typedef void (*void_value)(value_p);

STRUCT(value_info)
{
    int size;
    void_value value_print;
};

#endif