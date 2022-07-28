SRC=src/main.c
OBJ=bin/main.o

LIBS=lib_trie lib_my_string

TRIE_PATH=lib/lib_trie
TRIE_FILE=$(TRIE_PATH)/lib.o

STR_PATH=lib/lib_my_string
STR_FILE=$(STR_PATH)/lib.o

default:
	clear
	$(MAKE) rebuild
	$(MAKE) run

run: $(OBJ)
	./$(OBJ)

build: $(OBJ)
	

$(OBJ): $(SRC) $(TRIE_FILE) $(STR_FILE)
	gcc -o $(OBJ) $^ -Wall

$(TRIE_FILE):
	cd $(TRIE_PATH) && $(MAKE)


$(STR_FILE):
	cd $(STR_PATH) && $(MAKE)

clean:
	rm -rf $(OBJ)
	cd lib && $(MAKE) clean

rebuild:
	$(MAKE) clean 
	$(MAKE) build

test:
	$(MAKE) rebuild
	cd lib && $(MAKE) test
