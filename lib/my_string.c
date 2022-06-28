#include <stdio.h>

#define STRUCT(NAME)                    \
    typedef struct _##NAME NAME##_t;    \
    typedef NAME##_t *NAME##_p;         \
    struct _##NAME

STRUCT(string)
{
    char len;
    char arr[0];
};

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
