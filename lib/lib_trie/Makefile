STR_PATH=../lib_my_string
STR_FILE=$(STR_PATH)/lib.o

build: lib.o

lib.o: tmp.o $(STR_FILE)
	ld -r -o lib.o $^
	
test.o: tmp_test.o $(STR_FILE)
	ld -r -o lib.o $^

tmp.o: code.c
	gcc -o tmp.o code.c -c -Wall

tmp_test.o:
	gcc -o tmp.o code.c -c -Wall -D DEBUG

.PHONY: $(STR_FILE)
$(STR_FILE):
	$(MAKE) --directory=$(STR_PATH)

clean:
	rm -f *.o
	$(MAKE) clean --directory=test

.PHONY: test
test:
	$(MAKE) --directory=test

