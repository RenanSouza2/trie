SRC=src/main.c
OBJ=bin/main.o

TRIE_PATH=lib/lib_mem_int_trie
TRIE_FILE=$(TRIE_PATH)/lib.o

run: $(OBJ)
	./$(OBJ)

build: $(OBJ)
	
$(OBJ): $(SRC) $(TRIE_FILE)
	gcc -o $(OBJ) $^ -Wall -Wall -Werror -Wfatal-errors

.PHONY: $(TRIE_FILE)
$(TRIE_FILE):
	$(MAKE) --directory=$(TRIE_PATH)

clean:
	rm -rf $(OBJ)
	$(MAKE) clean --directory=lib

rebuild:
	$(MAKE) clean 
	$(MAKE) build

.PHONY: test
test: 
	$(MAKE) test --directory=lib
