build: lib.o

lib.o: code.c
	gcc -o lib.o code.c -c -Wall

clean:
	rm -f lib.o $(OBJ)
	$(MAKE) clean --directory=lib_trie

.PHONY: test
test:
	$(MAKE) --directory=lib_trie

