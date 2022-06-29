NAME = lib_trie

IMP = implementation.c
OBJ = object.o
TGT = library.so
END = $(LIB_FLDR)/$(NAME).so

LIB_FLDR = ../../obj

copy: $(TGT)
	cp $(TGT) $(END)

build: $(TGT)

$(TGT): $(IMP) $(LIB)
	gcc -fpic -o $(OBJ) -c $(IMP) 
	gcc -fpic -o $(TGT) $(OBJ) -shared -L. -Wl,-soname,$(NAME)

$(LIB): 
	cd ../lib_my_string && $(MAKE)

clean:
	rm -f $(TGT) $(OBJ)

rebuild:
	$(MAKE) clean
	$(MAKE) build