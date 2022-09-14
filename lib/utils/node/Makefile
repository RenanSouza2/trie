FLAGS=-c -Wall -Werror -Wfatal-errors

build: lib.o

lib.o debug.o: code.c
	gcc $(FLAGS) -o $@ code.c

.PHONY: clean
clean:
	rm -f *.o
