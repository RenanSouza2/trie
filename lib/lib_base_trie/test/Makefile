FLAGS=-Wall -Werror -Wfatal-errors

STR_PATH=../../lib_string
STR_FILE=$(STR_PATH)/lib.o

INT_PATH=../../lib_value_int
INT_FILE=$(INT_PATH)/lib.o

MEM_PATH=../../lib_pointer_mem
MEM_FILE=$(MEM_PATH)/lib.o

TRIE_PATH=../../lib_trie_info
TRIE_FILE=$(TRIE_PATH)/lib.o

test: test.o
	./test.o 

build: test.o

test.o: tmp.o lib_tmp.o
	gcc $(FLAGS) -o test.o $^

.INTERMEDIATE: tmp.o
tmp.o: test.c ../code.c
	gcc -c $(FLAGS) -o tmp.o test.c

#  $(STR_FILE) $(INT_FILE) $(MEM_FILE) $(TRIE_FILE)
.INTERMEDIATE: lib_tmp.o
lib_tmp.o: $(STR_FILE) $(INT_FILE) $(MEM_FILE) $(TRIE_FILE)
	ld -o lib_tmp.o $^ -r

.PHONY: $(STR_FILE)
$(STR_FILE):
	$(MAKE) --directory=$(STR_PATH)

.PHONY: $(INT_FILE)
$(INT_FILE):
	$(MAKE) --directory=$(INT_PATH)

.PHONY: $(MEM_FILE)
$(MEM_FILE):
	$(MAKE) --directory=$(MEM_PATH)

.PHONY: $(TRIE_FILE)
$(TRIE_FILE):
	$(MAKE) --directory=$(TRIE_PATH)

clean:
	rm -f test.o
