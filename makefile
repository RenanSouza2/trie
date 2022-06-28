NAME = main
SRC = $(NAME).c
TGT = $(BIN)/$(NAME).o
LIB=lib
OBJ=obj
BIN=bin

runClear: 
	clear
	make run

run: $(TGT)
	./$(TGT)

build: $(TGT)

$(TGT): $(SRC) $(OBJ)/lib_my_string.so
	gcc $(SRC) -fpic -o $(TGT) -Wall -Wno-char-subscripts -L./$(OBJ) -l_my_string -Wl,-rpath=./obj

clean:
	rm -f $(OBJ)/* $(BIN)/*

rebuild:
	make clean build

$(OBJ)/lib_my_string.so: $(OBJ)/my_string.o
	gcc -shared -o $(OBJ)/lib_my_string.so $(OBJ)/my_string.o

$(OBJ)/my_string.o: $(LIB)/my_string.c
	gcc -fpic -c $(LIB)/my_string.c -o $(OBJ)/my_string.o

time: $(TGT)
	clear
	time ./$(TGT)
