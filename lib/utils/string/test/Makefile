.PHONY: test
test: test.o
	./test.o 

test.o: test.c ../code.c
	gcc test.c -o test.o -Wall -Werror -Wfatal-errors

.PHONY: clean
clean:
	rm -rf test.o
