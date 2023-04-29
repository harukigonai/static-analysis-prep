CC=wllvm
CFLAGS=-g -gdwarf-4 -Wall -funwind-tables
LDLIBS=./fake_lib.so
LDFLAGS=-g

.PHONY: all
all: clean libfunc fake_lib.so main init_2 bc

.PHONY: clean
clean:
	rm -f *.o *.so *~ init_2 main && cd lib_wrapper && $(MAKE) clean

.PHONY: bc
bc:
	extract-bc fake_lib.so && extract-bc main

init_2: init_2.o
	$(CC) -static $(CFLAGS) $^ -o $@ 

init_2.o: init_2.c

main: main.o
	$(CC) main.o -o main $(CFLAGS) $(LDLIBS)

main.o: main.c
	$(CC) -c main.c -o main.o $(CFLAGS) $(LDLIBS)

fake_lib.so: fake_lib.o
	$(CC) $(LDFLAGS) -shared $^ -o $@

fake_lib.o: fake_lib.c fake_lib.h
	$(CC) -c fake_lib.c -o fake_lib.o $(CFLAGS) -fPIC

libfunc:
	cd lib_wrapper && $(MAKE) all
