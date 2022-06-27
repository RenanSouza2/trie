NAME = main
SRC = $(NAME).c
TGT = $(NAME).o

runClear: 
	clear
	make run

run: $(TGT)
	./$(TGT)

build: $(TGT)

$(TGT): $(SRC)
	gcc $(SRC) -o $(TGT) -Wall -g -Wno-char-subscripts

clean:
	rm -f $(TGT)

rebuild:
	make clean build

time: $(TGT)
	clear
	time ./$(TGT)
