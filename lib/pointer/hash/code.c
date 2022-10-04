#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <openssl/sha.h>

#include "../../base/header/pointer.h"
#include "../../base/trie/header.h"
#include "../mem/header.h"

#ifdef DEBUG

int decrease(int created, int freed);

extern int pointer_created;
extern int pointer_free;

extern int fork_created;
extern int fork_free;

extern int path_created;
extern int path_free;

extern int leaf_created;
extern int leaf_free;

#define INC(INT) INT##_created++;
#define DEC(INT) INT##_free = decrease(INT##_created, INT##_free)
#else
#define INC(INT)
#define DEC(INT)
#endif

typedef char hash_t[65];

root_p get_database()
{
    static root_p r = NULL;
    if(r) return r;

    pointer_info_p pi = get_mem_info();
    r = root_init(pi, 8, 16);
    return r;
}

void hash_func(hash_t out, unsigned char in[], int size)
{
    unsigned char *out_heap = SHA256(in, size, NULL);
    for(int i=0; i<32; i++)
        sprintf(&out[2*i], "%02x", out_heap[i]);
    out[64] = '\0';
}


handler_p hash_get(pointer_p p)
{
    root_p r = get_database();
    return root_querie(r, (char*)p);
}

void hash_display(pointer_p p)
{
    if(p == NULL)
    {
        printf("NULL");
        return;
    }
    
    hash_t hash;
    memcpy(hash, p, sizeof(hash));
    hash[64] = '\0';
    printf("%s", hash);
}

pointer_p hash_set(handler_p h, int size)
{
    hash_t hash;
    hash_func(hash, h, size);
    pointer_p p = malloc(sizeof(handler_p));
    assert(p);
    INC(pointer);

    *(handler_p*)p = h;
    return p;
}

void hash_free(pointer_p p)
{

#ifdef DEBUG
    handler_p h = hash_get(p);
    switch (*(int*)h)
    {
        case 0: DEC(fork); break;
        case 1: DEC(path); break;
        case 2: DEC(leaf); break;
    }
#endif

    free(p);
    DEC(pointer);
}

pointer_info_p pi = NULL;

pointer_info_p get_hash_info()
{
    if(pi) return pi;

    pi = malloc(sizeof(pointer_info_t));
    assert(pi);

    *pi = (pointer_info_t) {
        64,

        hash_display,

        hash_get,
        hash_set,
        hash_free
    };

    return pi;
}
