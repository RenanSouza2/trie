NAME = main
BIN=bin
SRCF=src

SRC = $(SRCF)/$(NAME).c
OBJ = obj/main.o
TGT = $(BIN)/$(NAME)

LIB_FLAGS = -L./obj -l:lib_trie -l:lib_my_string -Wl,-rpath=obj

runClear: 
	clear
	$(MAKE) run

run: $(TGT)
	./$(TGT)

build: $(TGT)

$(TGT): $(SRC) obj/lib_trie.so obj/lib_my_string.so
	# gcc -Iobj -o $(OBJ) $(SRC)  $(LIB_FLAGS)
	# # gcc -o $(TGT) $(OBJ) $(LIB_FLAGS)
	gcc src/main.c -o bin/main.o -L./obj -l:lib_trie.so -l:lib_my_string.so

clean:
	rm -f obj/* $(BIN)/*
	cd lib/lib_my_string && $(MAKE) clean
	cd lib/lib_trie && $(MAKE) clean

rebuild:
	$(MAKE) clean 
	clear
	$(MAKE) build

obj/%.so: 
	echo copying ./lib/$*/implementation.c
	cd ./lib/$* && $(MAKE)

time: $(TGT)
	clear
	time ./$(TGT)
