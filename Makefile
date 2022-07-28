SRC=src/main.c
OBJ=bin/main.o

LIBS=lib_trie lib_my_string

TRIE_PATH=lib/lib_trie
TRIE_FILE=$(TRIE_PATH)/lib.o

run: $(OBJ)
	./$(OBJ)

build: $(OBJ)
	
$(OBJ): $(SRC) $(TRIE_FILE)
	gcc -o $(OBJ) $^ -Wall

.PHONY: $(TRIE_FILE)
$(TRIE_FILE):
	cd $(TRIE_PATH) && $(MAKE)

clean:
	rm -rf $(OBJ)
	cd lib && $(MAKE) clean

rebuild:
	$(MAKE) clean 
	$(MAKE) build

.PHONY: test
test: 
	cd lib && $(MAKE) test
