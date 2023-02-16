DIRS := $(wildcard */.)

.PHONY: clean
clean:
	for DIR in $(DIRS) ; do \
		$(MAKE) clean --directory=$$DIR ; \
	done

.PHONY: test
test:
	for DIR in $(DIRS) ; do \
		$(MAKE) test --directory=$$DIR ; \
	done
