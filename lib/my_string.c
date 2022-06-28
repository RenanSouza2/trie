#include <stdio.h>

#include "lib_my_string.h"

void string_display(string_p str) {
    printf("\nstr: %d |", str->len);
    for(int i=0; i<str->len; i++)
        printf(" %d", str->arr[i]);
}

int string_cmp(string_p str1, char len, char arr[])
{
    int i;
    if(str1->len < len) len = str1->len;
    for(i=0; i<len && str1->arr[i] == arr[i]; i++);
    return i;
}
