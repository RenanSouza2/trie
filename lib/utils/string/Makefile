FLAGS = -c -Wall -Werror -Wfatal-errors

LIBS = 

LIB_ROOT = ..
LIB_DIR = $(foreach LIB,$(LIBS),$(LIB_ROOT)/$(LIB))
LIB_FILES = $(foreach LIB,$(LIB_DIR),$(LIB)/lib.o)
DBG_FILES = $(foreach LIB,$(LIB_DIR),$(LIB)/debug.o)

build: lib.o



lib.o: tmp.o $(LIB_FILES)
	ld -r -o $@ $^

.INTERMEDIATE: tmp.o
tmp.o: code.c
	gcc $(FLAGS) -o $@ $^

.PHONY: $(LIB_FILES)
$(LIB_FILES):
	$(MAKE) --directory=$(dir $@)




debug.o: dbg.o $(DBG_FILES)
	ld -r -o $@ $^

.INTERMEDIATE: dbg.o
dbg.o: code.c
	gcc $(FLAGS) -o $@ $^

.PHONY: $(DBG_FILES)
$(DBG_FILES):
	$(MAKE) debug.o --directory=$(dir $@)



clean:
	rm -f *.o
	$(MAKE) clean --directory=test
	for DIR in $(LIB_DIR) ; do \
		$(MAKE) clean --directory=$$DIR ; \
	done

.PHONY: test
test:
	$(MAKE) --directory=test
