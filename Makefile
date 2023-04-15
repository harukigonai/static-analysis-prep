.PHONY: all
all: example_1

.PHONY: example_1
example_1:
	cd example_programs/1 && $(MAKE)

.PHONY: libbacktrace
libbacktrace:
	cd libbacktrace && ./configure CC="clang" && $(MAKE)
