CC=clang
INCLUDES=-I../../libbacktrace

# ARM
# CFLAGS=-g -gdwarf-4 -Wall -target aarch64-linux-gnu $(INCLUDES)

# x86
CFLAGS=-g -gdwarf-4 -Wall
LDLIBS=./unwind_and_find_var_addrs/libunwind_and_find_var_addrs.so

LDFLAGS=-g

.PHONY: bc
bc:
	$(CC) -g -c -emit-llvm main.c -o main.bc

.PHONY: all
all: clean main.s libunwind_and_find_var_addrs.so main bc

.PHONY: clean
clean:
	rm -f main *.s *.o *.bc *.ll

.PHONY: libunwind_and_find_var_addrs.so
libunwind_and_find_var_addrs.so:
	cd unwind_and_find_var_addrs && $(MAKE) all

main.s: main.c
	$(CC) -S $(CFLAGS) main.c $(LDLIBS)

main: main.o
	$(CC) $(LDFLAGS) main.o -o main $(LDLIBS)

main.o: main.c
