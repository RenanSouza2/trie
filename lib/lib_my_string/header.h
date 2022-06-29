#ifndef LIB_MY_STRING
#define LIB_MY_STRING

#include "../struct.h"

STRUCT(string)
{
    char len;
    char arr[0];
};

void string_display(string_p str);
int  string_cmp(string_p str, char arr[]);

#endif