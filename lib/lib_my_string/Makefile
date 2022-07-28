build: lib.o

lib.o: code.c
	gcc -o lib.o code.c -c -Wall

clean:
	rm -f lib.o $(OBJ)
	cd test && $(MAKE) clean

.PHONY: test
test:
	cd test && $(MAKE)

