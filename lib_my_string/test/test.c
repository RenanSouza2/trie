#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "header.h"

int test_string_cmp()
{
    int len = 3;
    string_p str = malloc(sizeof(string_t) + 3);
    str->len = 3;
    str->arr[0] = 1;
    str->arr[1] = 1;
    str->arr[2] = 1;

    char arr[3] = {1, 1, 1};

    for(int i=0; i<3; i++)
    {
        arr[i] = 0;
        int res = string_cmp(str, arr);
        arr[i] = 1;
        
        assert(res == i);
    }
}

int main() {
    setbuf(stdout, NULL);

    test_string_cmp();

    printf("\nTest successful\n");
    return 0;
}
