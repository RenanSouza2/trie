STR_PATH=../lib_my_string
STR_FILE=$(STR_PATH)/lib.o

build: lib.o

lib.o: lib_tmp.o $(STR_FILE)
	ld -r -o lib.o $^

lib_tmp.o: code.c
	gcc -o lib_tmp.o code.c -c -Wall

$(STR_FILE):
	cd $(STR_PATH) && $(MAKE)

clean:
	rm -f *.o
	cd test && $(MAKE) clean

test:
	cd test && $(MAKE)

.PHONY: test
