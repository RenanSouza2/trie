FLAGS=-c -Wall -Werror -Wfatal-errors

build: lib.o

lib.o: code.c
	gcc $(FLAGS) -o lib.o code.c

debug.o: code.c
	gcc $(FLAGS) -o debug.o code.c -D DEBUG

clean:
	rm -f *.o
