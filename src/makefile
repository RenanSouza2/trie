DIRS := $(wildcard */.)

.PHONY: run
run:
	for DIR in $(DIRS) ; do \
		$(MAKE) --directory=$$DIR ; \
	done

.PHONY: clean
clean:
	for DIR in $(DIRS) ; do \
		$(MAKE) clean --directory=$$DIR ; \
	done
