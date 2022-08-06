STR_PATH=../../lib_string
STR_FILE=$(STR_PATH)/lib.o

test: test.o
	./test.o 

build: test.o

test.o: test.c ../code.c $(STR_FILE)
	gcc -o test.o test.c $(STR_FILE) -Wall -Werror -Wfatal-errors

.PHONY: $(STR_FILE)
$(STR_FILE):
	$(MAKE) --directory=$(STR_PATH)

clean:
	rm -f test.o
