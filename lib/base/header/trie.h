#ifndef __BASE_H__
#define __BASE_H__

#include "../../utils/struct.h"

PLACEHOLDER(pointer_info);
PLACEHOLDER(value_info);

STRUCT(trie_info)
{
    int max;
    value_info_p vi;
    pointer_info_p pi;
};

#endif
