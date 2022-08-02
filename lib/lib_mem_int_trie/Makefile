INT_PATH=../lib_int_value
INT_FILE=$(INT_PATH)/lib.o

MEM_PATH=../lib_mem_ptr
MEM_FILE=$(MEM_PATH)/lib.o

BASE_PATH=../lib_base_trie
BASE_FILE=$(BASE_PATH)/lib.o

FLAGS=-c -Wall -Werror -Wfatal-errors

build: lib.o

lib.o: tmp.o $(BASE_FILE) $(INT_FILE) $(MEM_FILE)
	ld -r -o lib.o $^

.INTERMEDIATE: tmp.o
tmp.o: code.c
	gcc $(FLAGS) -o tmp.o code.c

debug.o: debug_temp.o lib.o
	ld -r -o debug.o $^

.INTERMEDIATE: debug_temp.o
debug_temp.o: debug.c
	gcc $(FLAGS) -o debug_temp.o debug.c

.PHONY: $(BASE_FILE)
$(BASE_FILE):
	$(MAKE) --directory=$(BASE_PATH)

.PHONY: $(INT_FILE)
$(INT_FILE):
	$(MAKE) --directory=$(INT_PATH)

.PHONY: $(MEM_FILE)
$(MEM_FILE):
	$(MAKE) --directory=$(MEM_PATH)

clean:
	rm -f *.o
	$(MAKE) clean --directory=$(STR_PATH)
