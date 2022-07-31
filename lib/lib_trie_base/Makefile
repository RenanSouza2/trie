STR_PATH=../lib_my_string
STR_FILE=$(STR_PATH)/lib.o

build: lib.o

lib.o: tmp.o $(STR_FILE)
	ld -r -o lib.o $^
	
tmp.o: code.c
	gcc -o tmp.o code.c -c -Wall -Werror -Wfatal-errors

debug.o: debug_temp.o lib.o
	ld -r -o debug.o $^

debug_temp.o: debug.c
	gcc -o debug_temp.o debug.c -c -Wall -Werror -Wfatal-errors

.PHONY: $(STR_FILE)
$(STR_FILE):
	$(MAKE) --directory=$(STR_PATH)

clean:
	rm -f *.o
	$(MAKE) clean --directory=$(STR_PATH)

