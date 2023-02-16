FLAGS = -Wall -Werror -Wfatal-errors
DEBUG_FILE = ../debug.o

test: test.o
	./test.o 

test.o: test.c $(DEBUG_FILE)
	gcc $(FLAGS) -o $@ $^ -D DEBUG

.PHONY: $(DEBUG_FILE)
$(DEBUG_FILE):
	$(MAKE) debug.o --directory=$(dir $@)

clean:
	rm -rf test.o
