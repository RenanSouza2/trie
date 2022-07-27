LIB_STRING=../lib_my_string/lib.so

build: lib.so

lib.so: code.c $(LIB_STRING)
	gcc -o lib.so -fpic -shared code.c -L.. -l:lib_my_string/lib.so -Wl,-rpath,..

$(LIB_STRING):
	cd ../lib_my_string && $(MAKE)

clean:
	rm -f lib.so
	cd ../lib_my_string && $(MAKE) clean

test:
	cd lib_my_string && $(MAKE) test
