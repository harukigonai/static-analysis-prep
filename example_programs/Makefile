CC=clang

# ARM
# CFLAGS=-g -gdwarf-4 -Wall -target aarch64-linux-gnu $(INCLUDES)

# x86
CFLAGS=-g -gdwarf-4 -Wall -funwind-tables
LDLIBS=./libunwind_test.so ./unwind_and_find_var_addrs/libunwind_and_find_var_addrs.so
LDFLAGS=-g

.PHONY: all
all: clean libunwind_and_find_var_addrs.so libunwind_test.so main.s main bc

.PHONY: clean
clean:
	rm -f main *.s *.o *.bc *.ll && cd unwind_and_find_var_addrs && $(MAKE) clean

.PHONY: libunwind_and_find_var_addrs.so
libunwind_and_find_var_addrs.so:
	cd unwind_and_find_var_addrs && $(MAKE) all

.PHONY: bc
bc:
	$(CC) -g -c -emit-llvm main.c -o main.bc

main.s: main.c
	$(CC) -S $(CFLAGS) main.c $(LDLIBS)

main: main.o
	$(CC) $(LDFLAGS) main.o -o main $(LDLIBS)

main.o: main.c

libunwind_test.so: libunwind_test.o
	clang -g libunwind_test.o -o libunwind_test.so -g -shared ./unwind_and_find_var_addrs/libunwind_and_find_var_addrs.so

libunwind_test.o: libunwind_test.c
	clang -c -g -gdwarf-4 -Wall -fPIC -funwind-tables libunwind_test.c -o libunwind_test.o
