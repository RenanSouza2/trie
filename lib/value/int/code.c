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

void value_print(value_p value)
{
    printf("%d", *(int*)value);
}

value_p set_int(int value)
{
    value_p vp = malloc(sizeof(int));
    assert(value);
    INC(value);

    *(int*)vp = value;
    return vp;
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

value_info_p get_int_value_info()
{
    value_info_p vi = malloc(sizeof(value_info_p));
    assert(vi);

    *vi = (value_info_t){
        int_size, 
        int_is_null,
        value_print
    };
    return vi;
}
