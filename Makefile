LIB_TRIE=lib_trie/lib.so

debug: 
	cd lib_trie && $(MAKE)
	./main.o

run: main.o
	./main.o

build: main.o

main.o: main.c $(LIB_TRIE)
	gcc main.c -o main.o -L. -l:$(LIB_TRIE) -Wl,-rpath,.

$(LIB_TRIE):
	cd lib_trie && $(MAKE)

clean:
	rm -rf main.o
	cd lib_trie && $(MAKE) clean

rebuild:
	$(MAKE) clean 
	$(MAKE) build