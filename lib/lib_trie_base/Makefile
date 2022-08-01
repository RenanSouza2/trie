STR_PATH=../lib_my_string
STR_FILE=$(STR_PATH)/lib.o

FLAGS=-c -Wall -Werror -Wfatal-errors

build: lib.o

lib.o: tmp.o $(STR_FILE)
	ld -r -o lib.o $^

.INTERMEDIATE: tmp.o
tmp.o: code.c
	gcc $(FLAGS) -o tmp.o code.c

debug.o: debug_temp.o lib.o
	ld -r -o debug.o $^

.INTERMEDIATE: debug_temp.o
debug_temp.o: debug.c
	gcc $(FLAGS) -o debug_temp.o debug.c

.PHONY: $(STR_FILE)
$(STR_FILE):
	$(MAKE) --directory=$(STR_PATH)

clean:
	rm -f *.o
	$(MAKE) clean --directory=$(STR_PATH)

