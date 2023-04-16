.PHONY: all
all: clean example_1

.PHONY: clean
clean:
	cd example_programs/1 && $(MAKE) clean

.PHONY: example_1
example_1:
	cd example_programs/1 && $(MAKE)

.PHONY: libbacktrace
libbacktrace:
	cd libbacktrace && ./configure CC="clang" && $(MAKE)
