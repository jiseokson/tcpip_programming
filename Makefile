SUBDIRS := $(wildcard */)

.PHONY: clean

clean:
	@for dir in $(SUBDIRS); do \
		if [ -f $$dir/Makefile ]; then \
			echo "Running 'make clean' in $$dir"; \
			$(MAKE) -C $$dir clean; \
		fi \
	done
