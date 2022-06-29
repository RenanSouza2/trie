NAME = main
BIN=bin
SRCF=src

SRC = $(SRCF)/$(NAME).c
TGT = $(BIN)/$(NAME).o

LIB_FLAGS = -Wl,-rpath=./obj

runClear: 
	clear
	$(MAKE) run

run: $(TGT)
	./$(TGT)

build: $(TGT)

$(TGT): $(SRC) obj/lib_trie.so
	gcc $(LIB_FLAGS) $(SRC) -o $(TGT) -Wall -Wno-char-subscripts 

clean:
	rm -f obj/* $(BIN)/*
	cd lib/lib_my_string && $(MAKE) clean

rebuild:
	$(MAKE) clean 
	clear
	$(MAKE) build

obj/%.so: 
	echo "compiling ./lib/$*/implementation.c"
	cd ./lib/$* && $(MAKE)

time: $(TGT)
	clear
	time ./$(TGT)
