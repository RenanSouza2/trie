SRC = src
LIB = lib

run:
	$(MAKE) --directory=$(SRC) 

build:
	$(MAKE) build --directory=$(SRC)
	
clean:
	$(MAKE) clean --directory=$(SRC)
	$(MAKE) clean --directory=$(LIB)
	
test: 
	$(MAKE) test --directory=$(LIB)
