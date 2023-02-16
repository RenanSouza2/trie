FLAGS = -Wall -Werror -Wfatal-errors

LIBS = base/trie value/int pointer/mem

ROOT = ../..
LIB_PATH = $(ROOT)/lib
LIB_FILES = $(foreach LIB,$(LIBS),$(LIB_PATH)/$(LIB)/lib.o)

run: main.o
	./main.o

build: main.o

main.o: main.c $(LIB_FILES)
	gcc $(FLAGS) -o $@ $^

.PHONY: $(LIB_FILES)
$(LIB_FILES):
	$(MAKE) --directory=$(dir $@)

clean:
	rm -rf *.o
