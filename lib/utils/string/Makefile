build: lib.o

lib.o debug.o: code.c
	gcc -o $@ code.c -c -Wall -Werror -Wfatal-errors

.PHONY: clean
clean:
	rm -f *.o
	$(MAKE) clean --directory=test

.PHONY: test
test:
	$(MAKE) --directory=test
