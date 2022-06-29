NAME = lib_trie
SRC = implementation.c
OBJ = $(NAME).o
TGT = $(NAME).so

LIB_STR = ../lib_my_string/lib_my_string.so
LIB_FLAGS = -Wl,-rpath=../../obj

copy: $(TGT)
	cp $(TGT) ../../obj

build: $(TGT)

$(TGT): $(SRC) $(LIB_STR)
	gcc -fpic -c $(LIB_FLAGS) -o $(OBJ) $(SRC)
	gcc -shared -o $(TGT) $(OBJ) 

$(LIB_STR): ../lib_my_string/implementation.c
	cd ../lib_my_string && $(MAKE)

clean:
	rm -f $(TGT) $(OBJ)
	cd ../lib_my_string && $(MAKE) clean

rebuild:
	make clean build
