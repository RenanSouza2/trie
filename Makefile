SRC=src/main.c
OBJ=bin/main.o

FLAGS=-Wall -Wall -Werror -Wfatal-errors

LIBS=lib_trie lib_my_string

TRIE_PATH=lib/base/trie
TRIE_FILE=$(TRIE_PATH)/lib.o

INT_PATH=lib/value/int
INT_FILE=$(INT_PATH)/lib.o

MEM_PATH=lib/pointer/mem
MEM_FILE=$(MEM_PATH)/lib.o

run: $(OBJ)
	./$(OBJ)

build: $(OBJ)
	
$(OBJ): $(SRC) $(TRIE_FILE) $(INT_FILE) $(MEM_FILE)
	gcc $(FLAGS) -o $(OBJ) $^ 

.PHONY: $(TRIE_FILE)
$(TRIE_FILE):
	$(MAKE) --directory=$(TRIE_PATH)
	
.PHONY: $(INT_FILE)
$(INT_FILE):
	$(MAKE) --directory=$(INT_PATH)

.PHONY: $(MEM_FILE)
$(MEM_FILE):
	$(MAKE) --directory=$(MEM_PATH)

clean:
	rm -rf $(OBJ)
	$(MAKE) clean --directory=lib

rebuild:
	$(MAKE) clean 
	$(MAKE) build

.PHONY: test
test: 
	$(MAKE) test --directory=lib
