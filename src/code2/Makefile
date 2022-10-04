SRC=main.c
OBJ=../../bin/code2.o

FLAGS=-Wall -Wall -Werror -Wfatal-errors

LIBS=lib_trie lib_my_string

TRIE_PATH=../../lib/base/trie
TRIE_FILE=$(TRIE_PATH)/lib.o

INT_PATH=../../lib/value/int
INT_FILE=$(INT_PATH)/lib.o

HASH_PATH=../../lib/pointer/hash
HASH_FILE=$(HASH_PATH)/lib.o

build: $(OBJ)
	
$(OBJ): $(SRC) libs.o
	gcc $(FLAGS) -o $(OBJ) $^ -lcrypto

.INTERMEDIARY: libs.o
libs.o: $(TRIE_FILE) $(INT_FILE) $(HASH_FILE)
	ld -r -o $@ $^ 

.PHONY: $(TRIE_FILE)
$(TRIE_FILE):
	$(MAKE) --directory=$(TRIE_PATH)
	
.PHONY: $(INT_FILE)
$(INT_FILE):
	$(MAKE) --directory=$(INT_PATH)

.PHONY: $(HASH_FILE)
$(HASH_FILE):
	$(MAKE) --directory=$(HASH_PATH)

clean:
	$(MAKE) clean --directory=../../

rebuild:
	$(MAKE) clean 
	$(MAKE) build
