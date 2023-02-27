FLAGS = -Wall -Werror -Wfatal-errors
DEBUG_FILE = ../debug.o

LIBS = value/int pointer/mem

LIB_ROOT = ../../..
LIB_DIR = $(foreach LIB,$(LIBS),$(LIB_ROOT)/$(LIB))
DBG_FILES = $(foreach LIB,$(LIB_DIR),$(LIB)/debug.o)

test: test.o
	./test.o 

test.o: test.c $(DEBUG_FILE) $(DBG_FILES)
	gcc $(FLAGS) -o $@ $^ -D DEBUG

.PHONY: $(DEBUG_FILE) $(DBG_FILES)
$(DEBUG_FILE) $(DBG_FILES):
	$(MAKE) debug.o --directory=$(dir $@)

clean:
	rm -f *.o
	for DIR in $(LIB_DIR) ; do \
		$(MAKE) clean --directory=$$DIR ; \
	done