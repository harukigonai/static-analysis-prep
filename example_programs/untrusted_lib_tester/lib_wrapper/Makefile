CC = gcc
CFLAGS = -g -Wall -fPIC
LDFLAGS = -g -shared
LDLIBS = -ldl

.PHONY: all
all: clean libfunc.so

.PHONY: clean
clean:
	rm -f *.o *.so *~

%.so:
	$(LINK.c) $(LDFLAGS) $^ -o $@ $(LDLIBS)

libfunc.so: libfunc.o

libfunc.o: libfunc.c

