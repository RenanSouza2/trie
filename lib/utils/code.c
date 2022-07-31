#include <stdio.h>

void mem_pointer_display(void *p)
{
    if(p == NULL)   printf("NULL");
    else            printf("%p", p);
}
