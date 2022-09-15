#ifndef __LIB_VALUE_INT_H__
#define __LIB_VALUE_INT_H__

#include "../../utils/struct.h"

PLACEHOLDER(value);

value_p set_int(int value);
int get_int(value_p value);
void int_print(int size, value_p value);

#endif
