CC=clang
CFLAGS=-g -Wall
LDLIBS=libbacktrace/.libs/libbacktrace.a

# main.s: main.c
# 	$(CC) -S $(CFLAGS) $(LDLIBS) main.c

main: main.o

main.o: main.c
