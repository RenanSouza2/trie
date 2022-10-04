FLAGS=-c -Wall -Werror -Wfatal-errors

MEM_PATH=../mem
MEM_FILE=$(MEM_PATH)/lib.o

build: lib.o

lib.o: tmp.o $(MEM_FILE)
	ld -r -o $@ $^

debug.o: code.c $(MEM_FILE)
	gcc $(FLAGS) -o debug.o $^ -D DEBUG

tmp.o: code.c
	gcc $(FLAGS) -o tmp.o $^

.PHONE: $(MEM_FILE)
$(MEM_FILE):
	$(MAKE) lib.o --directory=$(MEM_PATH)

.PHONY: clean
clean:
	rm -f *.o
	$(MAKE) clean --directory=$(MEM_PATH)
