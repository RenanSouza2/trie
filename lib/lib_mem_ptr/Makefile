UTILS_PATH=../utils
UTILS_FILE=$(UTILS_PATH)/lib.o

FLAGS=-c -Wall -Werror -Wfatal-errors

build: lib.o

lib.o: tmp.o $(UTILS_FILE)
	ld -r -o lib.o $^

.INTERMEDIATE: tmp.o
tmp.o: code.c
	gcc $(FLAGS) -o tmp.o code.c 

.PHONY: $(UTILS_FILE)
$(UTILS_FILE):
	$(MAKE) --directory=$(UTILS_PATH)

clean:
	rm -f *.o
	$(MAKE) clean --directory=$(UTILS_PATH)


