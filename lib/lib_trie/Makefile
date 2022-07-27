build: lib.o

lib.o: code.c
	gcc -o lib.o code.c -c

clean:
	rm -f lib.o
	cd test && $(MAKE) clean

test:
	cd test && $(MAKE)

.PHONY: test
