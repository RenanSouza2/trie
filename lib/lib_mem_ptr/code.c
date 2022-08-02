#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "header.h"
#include "../utils/header.h"

#define MTF(POINTER) ((mem_trie_fork_p)(POINTER))
#define MTP(POINTER) ((mem_path_fork_p)(POINTER))

#define MAX 10
#define LEN 8
#define PTR_SIZE 8

#define PADDING sizeof(void*)
#define FORK_SIZE (PADDING + (MAX*PTR_SIZE))
#define PATH_SIZE (PADDING + PTR_SIZE + 1)

STRUCT(mem_trie_fork)
{
    trie_fork_t t;
    trie_p next[MAX];
};

STRUCT(mem_path_fork)
{
    trie_fork_t t;
    trie_p next;
};



trie_p mem_get_trie(trie_pointer_p tp)
{
    return *(void**)tp;
}

trie_pointer_p mem_pointer_create(trie_p t)
{
    void **tp = calloc(1, sizeof(void*));
    *tp = t;
    return (trie_pointer_p)tp;
}

int mem_pointer_is_null(trie_pointer_p tp)
{
    return mem_get_trie(tp) == NULL;
}

void mem_trie_free_single(trie_pointer_p tp)
{
    free(tp);
}

void mem_trie_replace(trie_pointer_p tp, trie_p t)
{
    free(*(void**)tp);
    *(void**)tp = t;
}

trie_info_p mem_trie_info(value_info_p vi)
{
    trie_info_p ti = calloc(1, sizeof(trie_info_t));
    *ti = (trie_info_t) {
        MAX, LEN,
        PTR_SIZE, FORK_SIZE, PATH_SIZE, PADDING + vi->size,
        mem_pointer_create(NULL),

        mem_get_trie,
        mem_pointer_create,
        mem_pointer_is_null,
        (void_pointer)mem_pointer_display,
        mem_trie_replace,
        mem_trie_free_single,

        vi
    };
    return ti;
} 