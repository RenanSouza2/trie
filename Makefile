LIBS=lib_trie lib_my_string

TRIE_PATH=lib/lib_trie
TRIE_FILE=$(TRIE_PATH)/lib.o

STR_PATH=lib/lib_my_string
STR_FILE=$(STR_PATH)/lib.o

run: main.o
	./main.o

build: main.o

main.o: main.c $(TRIE_FILE) $(STR_FILE)
	gcc -o main.o $^

$(TRIE_FILE):
	cd $(TRIE_PATH) && $(MAKE)


$(STR_FILE):
	cd $(STR_PATH) && $(MAKE)

lib/%/lib.o:
	cd lib/${%} && $(MAKE) 

clean:
	rm -rf main.o
	cd lib && $(MAKE) clean

rebuild:
	$(MAKE) clean 
	$(MAKE) build

test:
	cd lib && $(MAKE) test
