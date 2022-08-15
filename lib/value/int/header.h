#ifndef __LIB_VALUE_INT_H__
#define __LIB_VALUE_INT_H__

#include "../../utils/struct.h"

PLACEHOLDER(value);
PLACEHOLDER(value_info);

value_p set_int(int value);
int get_int(value_p value);
value_info_p get_int_value_info();

#endif
