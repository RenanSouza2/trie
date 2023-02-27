DIRS := $(wildcard */.)

.PHONY: clean
clean:
	for DIR in $(DIRS) ; do \
		( $(MAKE) clean --directory=$$DIR ) || exit $$? \
	done

.PHONY: test
test:
	for DIR in $(DIRS) ; do \
		( $(MAKE) test --directory=$$DIR ) || exit $$? ; \
	done
