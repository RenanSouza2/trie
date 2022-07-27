build: lib.so

lib.so: code.c
	gcc -o lib.so -fpic -shared code.c

clean:
	rm -f lib.so $(OBJ)
	cd test && $(MAKE) clean

test:
	cd test && $(MAKE)

.PHONY: test
