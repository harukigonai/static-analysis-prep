CC = gcc
CFLAGS = -g -Wall
LDLIBS = ./lib_wrapper/libfunc.so

.PHONY: all
all: clean libfunc main init_2

.PHONY: clean
clean:
	rm -f *.o *~ init_2 main && cd lib_wrapper && $(MAKE) clean

init_2: init_2.o
	$(CC) -static $(CFLAGS) $^ -o $@ 

init_2.o: init_2.c

main: main.o
	$(CC) main.o -o main $(CFLAGS) $(LDLIBS)

main.o: main.c
	$(CC) -c main.c -o main.o $(CFLAGS) $(LDLIBS)

libfunc:
	cd lib_wrapper && $(MAKE) all
