#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../../base/header/pointer.h"


// STRUCT(pointer_info)
// {
//     int size;
//     pointer_p null;
//    
//     void_ptr_f  display;
//
//     value_ptr_f          get;
//     ptr_value_int_f      set;
//     void_ptr_value_int_f replace;
//     void_ptr_f           free;
//
// };

pointer_p tp_mem

void hash_display(pointer_p hash)
{
    long *vec = (long*) hash;
    printf("0x");
    for(int i=0; i<4; i++)
        printf("%016lx", vec[i]);
}

handler_p hash_get(pointer_p hash)
{

}

int main()
{
    long vec[4] = {11, 12, 13, 14};
    hash_display((pointer_p)vec);
    
    printf("\n");
    return 0;
}