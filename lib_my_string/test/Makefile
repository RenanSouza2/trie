LIB=../lib.so

test: test.o
	./test.o 

test.o: test.c $(LIB)
	gcc test.c -o test.o -L.. -l:lib.so -Wl,-rpath,..

$(LIB):
	cd .. &7 $(MAKE)