TRIE_PATH=../lib_trie_base
TRIE_FILE=$(TRIE_PATH)/lib.o

build: lib.o

lib.o: tmp.o $(TRIE_FILE)
	ld -r -o lib.o $^
	
tmp.o: code.c
	gcc -o tmp.o code.c -c -Wall -Werror -Wfatal-errors

.PHONY: $(TRIE_FILE)
$(TRIE_FILE):
	$(MAKE) --directory=$(TRIE_PATH)

clean:
	rm -f *.o
	$(MAKE) clean --directory=test
	$(MAKE) clean --directory=$(TRIE_PATH)

.PHONY: test
test:
	$(MAKE) --directory=test

