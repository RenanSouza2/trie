#ifndef __BASE_VALUE_H__
#define __BASE_VALUE_H__

#include "../../utils/struct.h"

STRUCT(value)
{
    int size;
    handler_p ptr;
};

#endif
