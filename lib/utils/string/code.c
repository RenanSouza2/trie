#include <stdio.h>

#include "header.h"

void string_display(string_p str) {
    printf("\nstr: %d | {", str->len);
    for(int i=0; i<str->len; i++)
        printf(" %d", str->arr[i]);
    printf(" }");
}

int string_cmp(string_p str, char arr[])
{
    int i, len = str->len;
    for(i=0; i<len && str->arr[i] == arr[i]; i++);
    return i;
}

int string_size(int length)
{
    return sizeof(short) + length;
}
