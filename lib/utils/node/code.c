#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "header.h"
#include "../struct.h"
#include "../string/header.h"

trie_p trie_fork_set(trie_info_p ti, int key, pointer_p tp_next)
{
    int size = FORK_SIZE;
    trie_p t = malloc(size);
    assert(t);
    // INC(fork);

    t->type = FORK;
    t->connected = 1;

    for(int i=0; i < ti->max; i++)
        PTR_CPY(FN(t, i), PI->null);

    PTR_CPY(FN(t, key), tp_next);
    free(tp_next);
    // DEC(pointer);

    return t;
}

trie_p trie_path_set(trie_info_p ti, char len, char arr[], pointer_p tp_next)
{
    assert(len > 0);

    int size = PATH_SIZE(len);
    trie_p t = calloc(1, size);
    assert(t);
    // INC(path);

    t->type = PATH;

    pointer_p next = PN(t);
    PTR_CPY(next, tp_next);
    free(tp_next);
    // DEC(pointer);

    string_p str = PS(t);
    str->len = len;
    memcpy(str->arr, arr, len);

    return t;
}
