build: lib.o

lib.o: code.c
	gcc -o lib.o code.c -c -Wall -Werror -Wfatal-errors

clean:
	rm -f lib.o $(OBJ)
	$(MAKE) clean --directory=test

.PHONY: test
test:
	$(MAKE) --directory=test
