#include <stdio.h>
#include <assert.h>

#include "../header.h"

int main()
{
    value_info_p vi = int_value_info();

    printf("\nHello: ");

    int value = 4;
    vi->value_print((value_p)&value);

    assert(!vi->value_is_null((value_p)&value));

    value = 0;
    assert(vi->value_is_null((value_p)&value));

    printf("\n");
    return 0;
}