#ifndef __LIB_VALUE_INT_H__
#define __LIB_VALUE_INT_H__

#include "../struct.h"

PLACEHOLDER(value);
PLACEHOLDER(value_info);

value_p value_int(int value);
int int_value(value_p value);
value_info_p get_int_value_info();

#endif