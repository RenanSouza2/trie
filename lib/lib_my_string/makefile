NAME = lib_my_string

IMP = implementation.c
OBJ = object.o
TGT = library.so
END = ../../obj/$(NAME).so

copy: $(TGT)
	cp $(TGT) $(END)

build: $(TGT)

$(TGT): $(IMP)
	gcc -fpic -o $(OBJ) -c $(IMP) 
	gcc -fpic -o $(TGT) $(OBJ) -shared -Wl,-soname,$(NAME)

clean:
	rm -f $(TGT) $(OBJ)

rebuild:
	$(MAKE) clean
	$(MAKE) build