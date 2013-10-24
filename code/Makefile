CXX = g++
CC = gcc
CFLAGS = -Wall -D_FILE_OFFSET_BITS=64
LIBS = -lfuse
PROG = mklfs
NOMAIN = -c

all: mklfs block.o segtest
segtest: segment.o flash.o block.o
	$(CXX) $(FLAGS) segtest.cc segment.o flash.o block.o -o segtest $(LIBS)
mklfs: mklfs.cc mklfs.h flash.o
	$(CXX) $(CFLAGS) mklfs.cc mklfs.h flash.o -o mklfs $(LIBS)
flash.o: flash.c flash.h
	$(CC) $(NOMAIN) $(CFLAGS) flash.c flash.h $(LIBS)
block.o: block.cc block.h
	$(CXX) $(NOMAIN) $(CFLAGS) block.cc block.h $(LIBS)
segment.o: segment.cc segment.h
	$(CXX) $(NOMAIN) $(CFLAGS) segment.cc segment.h $(LIBS)
clean:
	rm -f *.o $(PROG)
