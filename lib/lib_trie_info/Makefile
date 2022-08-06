FLAGS=-c -Wall -Werror -Wfatal-errors

build: lib.o

lib.o: tmp.o
	gcc $(FLAGS) -o tmp.o code.c

clean:
	rm -f *.o
