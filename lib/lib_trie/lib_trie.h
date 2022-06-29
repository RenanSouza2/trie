#include "../struct.h"
#include "../lib_my_string/lib_my_string.h"

#define T(POINTER)  ((trie_p)(POINTER))
#define TF(POINTER) ((trie_fork_p)(POINTER))
#define TP(POINTER) ((trie_path_p)(POINTER))
#define TL(POINTER) ((trie_leaf_p)(POINTER))

#define FALSE 0
#define TRUE  1

#define FORK 0
#define PATH 1
#define LEAF 2

#define MAX 10
#define LEN 8

STRUCT(trie)
{
    int type;
};

STRUCT(trie_fork)
{
    trie_t t;
    int connected, least;
    trie_p next[MAX];
};

STRUCT(trie_path)
{
    trie_t t;
    trie_p next;
    string_t str;
};

STRUCT(trie_leaf)
{
    trie_t t;
    int value;
};

void trie_display(trie_p t);
void trie_delete(trie_p *t, char arr[]);
void trie_insert(trie_p *t, char arr[], int value);

int trie_querie(trie_p t, char arr[]);