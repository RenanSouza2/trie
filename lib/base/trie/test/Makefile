FLAGS=-Wall -Werror -Wfatal-errors -D DEBUG

LIB_PATH=..
LIB_FILE=$(LIB_PATH)/tmp_libs.o

INT_PATH=../../../value/int
INT_FILE=$(INT_PATH)/debug.o

MEM_PATH=../../../pointer/mem
MEM_FILE=$(MEM_PATH)/debug.o

TRIE_PATH=../../../base/lib_trie_info
TRIE_FILE=$(TRIE_PATH)/debug.o

test: test.o
	./test.o 

build: test.o

test.o: tmp.o tmp_trie.o
	gcc $(FLAGS) -o test.o $^

.INTERMEDIATE: tmp.o
tmp.o: test.c ../code.c
	gcc -c $(FLAGS) -o tmp.o test.c

.INTERMEDIATE: tmp_trie.o
tmp_trie.o: $(LIB_FILE) $(INT_FILE) $(MEM_FILE) $(TRIE_FILE)
	ld -o $@ $^ -r

.PHONY: $(LIB_FILE)
$(LIB_FILE):
	$(MAKE) debug.o --directory=$(LIB_PATH)

.PHONY: $(INT_FILE)
$(INT_FILE):
	$(MAKE) debug.o --directory=$(INT_PATH)

.PHONY: $(MEM_FILE)
$(MEM_FILE):
	$(MAKE) debug.o --directory=$(MEM_PATH)

.PHONY: $(TRIE_FILE)
$(TRIE_FILE):
	$(MAKE) debug.o --directory=$(TRIE_PATH)

.PHONY: clean
clean:
	rm -f *.o
