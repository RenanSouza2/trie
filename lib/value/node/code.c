#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "header.h"
#include "../../base/header/value.h"
#include "../../utils/node/header.h"

// STRUCT(value_info)
// {
//     int_value_f     size;
//     void_value_f    print;
// };  

trie_info_p ti;

int node_size(value_p value)
{
    switch (((trie_p)value)->type)
    {
        case FORK: return FORK_SIZE;
        case PATH: return PATH_SIZE(PS(value)->len);
        case LEAF: return LEAF_SIZE(VI->size(LV(value)));
    }
    assert(0);
}

