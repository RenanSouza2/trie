#ifndef __BASE_H__
#define __BASE_H__

#include "../struct.h"

PLACEHOLDER(pointer_info);
PLACEHOLDER(value_info);

STRUCT(trie_info)
{
    pointer_info_p pi;
    value_info_p vi;
};

#endif
