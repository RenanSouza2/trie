STR_PATH=../../lib_my_string
STR_FILE=$(STR_PATH)/lib.o

test: test.o
	./test.o 

test.o: test.c ../code.c $(STR_FILE)
	gcc -o test.o test.c $(STR_FILE) -Wall

$(STR_FILE):
	cd $(STR_PATH) && $(MAKE)

clean:
	rm -f test.o
