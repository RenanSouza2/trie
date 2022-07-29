SRC=src/main.c
OBJ=bin/main.o

LIBS=lib_trie lib_my_string

TRIE_PATH=lib/lib_value_int
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
