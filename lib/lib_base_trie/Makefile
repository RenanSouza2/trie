STR_PATH=../lib_string
STR_FILE=$(STR_PATH)/lib.o

build: lib.o

lib.o: tmp.o $(STR_FILE)
	ld -r -o lib.o $^
	
tmp.o: code.c
	gcc -o tmp.o code.c -c -Wall -Werror -Wfatal-errors

.PHONY: $(STR_FILE)
$(STR_FILE):
	$(MAKE) --directory=$(STR_PATH)

.PHONY: clean
clean:
	rm -f *.o
	$(MAKE) clean --directory=$(STR_PATH)
	$(MAKE) clean --directory=test

.PHONY: test
test:
	$(MAKE) --directory=test
