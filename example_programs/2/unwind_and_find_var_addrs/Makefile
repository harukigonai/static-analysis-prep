CC=clang

# ARM
# CFLAGS=-g -gdwarf-4 -Wall -target aarch64-linux-gnu $(INCLUDES)

# x86
CFLAGS=-g -gdwarf-4 -Wall -fPIC -funwind-tables
LDLIBS=-L/usr/local/lib -lunwind-aarch64 -lunwind -ldl
LDFLAGS = -g -shared

libunwind_and_find_var_addrs.so: unwind_and_find_var_addrs.o malloc_2.o
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

unwind_and_find_var_addrs.o: unwind_and_find_var_addrs.c unwind_and_find_var_addrs.h

malloc_2.o: malloc_2.c

.PHONY: all
all: clean libunwind_and_find_var_addrs.so

.PHONY: clean
clean:
	rm -f *.so *.o
