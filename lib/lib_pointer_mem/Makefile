FLAGS= -c -Wall -Werror -Wfatal-errors

build: lib.o

lib.o: code.c
	gcc $(FLAGS) -o lib.o code.c

clean:
	rm -f *.o


