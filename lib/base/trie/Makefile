UTILS_PATH=../../utils

STR_PATH=$(UTILS_PATH)/string
STR_FILE=$(STR_PATH)/lib.o

NODE_PATH=$(UTILS_PATH)/node
NODE_FILE=$(NODE_PATH)/lib.o

build: lib.o

lib.o debug.o: tmp.o $(STR_FILE) $(NODE_FILE)
	ld -r -o $@ $^
	
tmp.o: code.c
	gcc -o tmp.o code.c -c -Wall -Werror -Wfatal-errors

.PHONY: $(STR_FILE)
$(STR_FILE):
	$(MAKE) --directory=$(STR_PATH)

.PHONY: $(NODE_FILE)
$(NODE_FILE):
	$(MAKE) --directory=$(NODE_PATH)

.PHONY: clean
clean:
	rm -f *.o
	$(MAKE) clean --directory=$(STR_PATH)
	$(MAKE) clean --directory=$(NODE_PATH)
	$(MAKE) clean --directory=test

.PHONY: test
test:
	$(MAKE) --directory=test
