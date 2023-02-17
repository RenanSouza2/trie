FLAGS = -Wall -Werror -Wfatal-errors

LIBS = base/trie value/int pointer/mem

LIB_ROOT = ../../lib
LIB_DIR = $(foreach LIB,$(LIBS),$(LIB_ROOT)/$(LIB))
LIB_FILES = $(foreach LIB,$(LIB_DIR),$(LIB)/lib.o)

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
	for DIR in $(LIB_DIR) ; do \
		$(MAKE) clean --directory=$$DIR ; \
	done
