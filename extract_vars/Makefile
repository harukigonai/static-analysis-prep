CC = g++
CFLAGS = -g -Wall
LDFLAGS = -g -Wall
LDLIBS = -L/usr/lib/llvm-14/lib -lLLVM
INCLUDE = -I/usr/lib/llvm-14/include

main: ProfileVar.o SaveSrcInfo.o ValueFlow.o
	$(CC) $(LDFLAGS) ProfileVar.o SaveSrcInfo.o ValueFlow.o -o main $(INCLUDE) $(LDLIBS)

ProfileVar.o: ProfileVar.cpp ProfileVar.hpp
	$(CC) $(CFLAGS) -c ProfileVar.cpp -o ProfileVar.o $(INCLUDE) $(LDLIBS)

SaveSrcInfo.o: SaveSrcInfo.cpp ProfileVar.hpp
	$(CC) $(CFLAGS) -c SaveSrcInfo.cpp -o SaveSrcInfo.o $(INCLUDE) $(LDLIBS)

ValueFlow.o: ValueFlow.cpp ProfileVar.hpp
	$(CC) $(CFLAGS) -c ValueFlow.cpp -o ValueFlow.o $(INCLUDE) $(LDLIBS)

.PHONY: clean
clean:
	rm -f *.o main
