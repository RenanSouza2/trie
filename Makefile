BIN=bin
SRC=src

CODE_1=code1
SRC_1=$(SRC)/$(CODE_1)
OBJ_1=$(BIN)/$(CODE_1).o

run1: $(OBJ_1)
	./$(OBJ_1)

build1: $(OBJ_1)
	
.PHONY: $(OBJ_1)
$(OBJ_1): 
	$(MAKE) --directory=$(SRC_1)

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
