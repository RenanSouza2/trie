NAME = main
BIN=bin
LIB=lib
OBJ=obj
SRCF=src

SRC = $(SRCF)/$(NAME).c
TGT = $(BIN)/$(NAME).o

LIB_FLAGS = -L./$(OBJ) -l_my_string -Wl,-rpath=./obj

runClear: 
	clear
	make run

run: $(TGT)
	./$(TGT)

build: $(TGT)

$(TGT): $(SRC) $(OBJ)/lib_my_string.so
	gcc $(SRC) -fpic -o $(TGT) -Wall -Wno-char-subscripts $(LIB_FLAGS)

clean:
	rm -f $(OBJ)/* $(BIN)/*

rebuild:
	make clean build

$(OBJ)/%.so: $(OBJ)/%.o
	gcc -shared -o $@ $<

$(OBJ)/%.o: $(LIB)/%.c
	gcc -fpic -c -o $@ $< $(LIB_FLAGS)

time: $(TGT)
	clear
	time ./$(TGT)
