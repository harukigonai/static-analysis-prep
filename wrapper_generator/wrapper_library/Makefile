CC=clang
CFLAGS=-g -gdwarf-4 -Wall -fPIC -funwind-tables
LDFLAGS = -g -shared
LDLIBS=-L/usr/local/lib -lunwind-aarch64 -lunwind -ldl

.PHONY: all
all: clean libfunc.so

.PHONY: clean
clean:
	rm -f *.o *.so *~

libfunc.so: unwind_and_find_var_addrs.o malloc_2.o libfunc.o
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

unwind_and_find_var_addrs.o: unwind_and_find_var_addrs.c unwind_and_find_var_addrs.h

malloc_2.o: malloc_2.c malloc_2.h

libfunc.o: libfunc.c

.PHONY: all
all: clean libfunc.so

.PHONY: clean
clean:
	rm -f *.so *.o
