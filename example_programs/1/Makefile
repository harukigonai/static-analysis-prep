CC=clang
INCLUDES=-I../../libbacktrace
CFLAGS=-g -Wall $(INCLUDES)
LDLIBS=../../libbacktrace/.libs/libbacktrace.a
LDFLAGS=-g

.PHONY: all
all: clean main.s main

.PHONY: clean
clean:
	rm -f main *.s *.o

main.s: main.c
	$(CC) -S $(CFLAGS) main.c $(LDLIBS)

main: main.o

main.o: main.c

