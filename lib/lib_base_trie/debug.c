#include <stdio.h>

#include "header.h"

#define NP(POINTER) ((char*)(TF(POINTER) + 1))
#define VP(POINTER) ((value_p)(NP(POINTER)))

trie_pointer_p trie_fork_next(trie_info_p ti, trie_p t, int index);
trie_pointer_p trie_path_next(trie_p t);
string_p trie_path_str(trie_info_p ti, trie_p t);

void trie_display_single(trie_info_p ti, trie_pointer_p tp) {
    printf("\ntrie: ");
    ti->pointer_display(tp);
    if(ti->pointer_is_null(tp)) return;

    trie_p t = ti->get_trie(tp);
    switch (t->type)
    {
        case FORK:
        printf("\t(FORK)");
        printf("\nconnected: %d", TF(t)->connected);
        for(int i=0; i < ti->max; i++)
        {
            trie_pointer_p next = trie_fork_next(ti, t, i);
            if(ti->pointer_is_null(next)) continue;

            printf("\n\t%d: ", i);
            ti->pointer_display(next);
        }
        break;
    
        case PATH:;
        printf("\t(PATH)");

        string_p str = trie_path_str(ti, t);
        string_display(str);

        printf("\nnext: ");
        trie_pointer_p next = trie_path_next(t);
        ti->pointer_display(next);
        break;

        case LEAF:
        printf("\t(LEAF)");
        ti->vi->value_print(VP(t));
        break;
    }
    printf("\n");
}

void trie_display_structure_rec(trie_info_p ti, trie_pointer_p tp)
{
    if(ti->pointer_is_null(tp)) return;
    trie_display_single(ti, tp);

    trie_p t = ti->get_trie(tp);
    switch (t->type)
    {
        case FORK:
        for(int i=0; i < ti->max; i++)
        {
            trie_pointer_p next = trie_fork_next(ti, t, i);
            trie_display_structure_rec(ti, next);
        }
        break;

        case PATH:;
        trie_pointer_p next = trie_path_next(t);
        trie_display_structure_rec(ti, next);
        break;
    }
}

void trie_display_structure(trie_info_p ti, trie_pointer_p tp)
{
    if(ti->pointer_is_null(tp)) printf("\nEmpty trie");
    else                        trie_display_structure_rec(ti, tp);
}
