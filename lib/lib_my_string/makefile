NAME = lib_my_string
SRC = implementation.c
OBJ = $(NAME).o
TGT = $(NAME).so

copy: $(TGT)
	cp $(TGT) ../../obj

build: $(TGT)

$(TGT): $(SRC)
	gcc -fpic -c -o $(OBJ) $(SRC)
	gcc -shared -o $(TGT) $(OBJ)

clean:
	rm -f $(TGT) $(OBJ)

rebuild:
	make clean build
