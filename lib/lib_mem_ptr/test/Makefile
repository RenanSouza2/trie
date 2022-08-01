FLAGS=-c -Wall -Werror -Wfatal-errors
LIB=../lib.o

test: test.o
	./test.o 

test.o: tmp.o $(LIB)
	gcc -o test.o $^

.PHONY: $(LIB)
$(LIB):
	$(MAKE) --directory=..

.INTERMEDIATE: tmp.o
tmp.o: test.c
	gcc $(FLAGS) -o tmp.o test.c

clean:
	rm -rf test.o
