#ifndef LIB_VALUE_INT_H
#define LIB_VALUE_INT_H

typedef struct _trie_int  *trie_int_p;

void trie_int_delete(trie_int_p *t, char arr[]);
void trie_int_insert(trie_int_p *t, char arr[], int value);

void trie_int_display(trie_int_p t);
int  trie_int_querie(trie_int_p t, char arr[]);

void trie_int_free(trie_int_p t);

#endif