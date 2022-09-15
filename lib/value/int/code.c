#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "header.h"
#include "../../base/header/value.h"

int decrease(int created, int freed);

#ifdef DEBUG
extern int value_created;

#define INC(INT) INT##_created++;
#else
#define INC(INT)
#endif

void int_print(int size, value_p value)
{
    printf("%d", *(int*)value);
}

value_p set_int(int value_int)
{
    handler_p handler = malloc(sizeof(int));
    assert(handler);

    value_p value = malloc(sizeof(value_t));
    assert(value);
    INC(value);

    *(int*)handler = value_int;
    *value = (value_t){4, handler};
    return value;
}

int get_int(value_p value)
{
    return (value == NULL) ? 0 : *(int*)value;
}

int int_size(value_p value)
{
    return 4;
}

int int_is_null(value_p value)
{
    return get_int(value) == 0;
}
