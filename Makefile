BIN=bin
SRC=src

CODE_1=code1
SRC_1=$(SRC)/$(CODE_1)
OBJ_1=$(BIN)/$(CODE_1).o

CODE_2=code2
SRC_2=$(SRC)/$(CODE_2)
OBJ_2=$(BIN)/$(CODE_2).o


run: $(OBJ_1) $(OBJ_2)
	./$(OBJ_1)
	./$(OBJ_2)

build: $(OBJ_1) $(OBJ_2)
	
.PHONY: $(OBJ_1)
$(OBJ_1): 
	$(MAKE) --directory=$(SRC_1)

.PHONY: $(OBJ_2)
$(OBJ_2): 
	$(MAKE) --directory=$(SRC_2)

.PHONY: $(TRIE_FILE)
$(TRIE_FILE):
	$(MAKE) --directory=$(TRIE_PATH)
	
clean:
	rm -rf $(BIN)/*.o
	$(MAKE) clean --directory=lib

rebuild:
	$(MAKE) clean 
	$(MAKE) build

.PHONY: test
test: 
	$(MAKE) test --directory=lib
