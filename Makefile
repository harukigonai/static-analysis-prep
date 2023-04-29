.PHONY: all
all: clean example

.PHONY: clean
clean:
	cd example_programs && $(MAKE) clean

.PHONY: example
example:
	cd example_programs && $(MAKE)

.PHONY: libbacktrace
libbacktrace:
	cd libbacktrace && ./configure CC="clang" && $(MAKE)
