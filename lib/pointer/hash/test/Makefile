FLAGS=-Wall -Werror -Wfatal-errors -D DEBUG

LIB_PATH=../
LIB_FILE=$(LIB_PATH)/debug.o

TRIE_PATH=../../../base/trie/
TRUE_FILE=$(TRIE_PATH)/debug.o

INT_PATH="../../../value/int/"

test: test.o
	./test.o 

build: test.o

test.o: tmp.o lib_tmp.o
	gcc $(FLAGS) -o test.o $^

.INTERMEDIATE: tmp.o
tmp.o: test.c ../code.c
	gcc -c $(FLAGS) -o tmp.o test.c

.INTERMEDIATE: lib_tmp.o
lib_tmp.o: $(STR_FILE) $(INT_FILE) $(MEM_FILE)
	ld -o lib_tmp.o $^ -r

.PHONY: $(STR_FILE)
$(STR_FILE):
	$(MAKE) debug.o --directory=$(STR_PATH)

.PHONY: $(INT_FILE)
$(INT_FILE):
	$(MAKE) debug.o --directory=$(INT_PATH)

.PHONY: $(MEM_FILE)
$(MEM_FILE):
	$(MAKE) debug.o --directory=$(MEM_PATH)

.PHONY: clean
clean:
	rm -f *.o
