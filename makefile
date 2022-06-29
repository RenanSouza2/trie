NAME = main
BIN=bin
SRCF=src

SRC = $(SRCF)/$(NAME).c
TGT = $(BIN)/$(NAME).o

LIB_FLAGS = -L./obj -l_my_string -Wl,-rpath=./obj

runClear: 
	clear
	make run

run: $(TGT)
	./$(TGT)

build: $(TGT)

$(TGT): $(SRC) obj/lib_my_string.so
	gcc $(SRC) -fpic -o $(TGT) -Wall -Wno-char-subscripts $(LIB_FLAGS)

clean:
	rm -f obj/* $(BIN)/*
	(cd lib/lib_my_string && $(MAKE) clean)

rebuild:
	$(MAKE) clean 
	clear
	$(MAKE) build

obj/%.so: # ./lib/$*/implementation.c"
	echo "compiling ./lib/$*/implementation.c"
	(cd ./lib/$* && $(MAKE))

time: $(TGT)
	clear
	time ./$(TGT)
