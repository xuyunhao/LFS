CXX = g++
CC = gcc
CFLAGS = -Wall -D_FILE_OFFSET_BITS=64
LIBS = -lfuse
PROG = mklfs
NOMAIN = -c

all: flash.o inode.o ifile.o log.o superblock.o checkpoint.o block.o mklfs lfs lfsck
flash.o: flash.c flash.h
	$(CC) $(NOMAIN) $(CFLAGS) flash.c flash.h $(LIBS)
ifile.o: ifile.cc ifile.h
	$(CXX) $(NOMAIN) $(CFLAGS) ifile.cc ifile.h flash.o lld.h $(LIBS)
inode.o: inode.cc inode.h
	$(CXX) $(NOMAIN) $(CFLAGS) inode.cc inode.h flash.o lld.h $(LIBS)
log.o: log.cc log.h
	$(CXX) $(NOMAIN) $(CFLAGS) log.cc log.h flash.o lld.h $(LIBS)
superblock.o: superblock.cc superblock.h
	$(CXX) $(NOMAIN) $(CFLAGS) superblock.cc superblock.h log.o lld.h $(LIBS)
checkpoint.o: checkpoint.cc checkpoint.h
	$(CXX) $(NOMAIN) $(CFLAGS) checkpoint.cc checkpoint.h log.o lld.h $(LIBS)
block.o: block.cc block.h
	$(CXX) $(NOMAIN) $(CFLAGS) block.cc block.h lld.h $(LIBS)
segment.o: segment.cc segment.h
	$(CXX) $(NOMAIN) $(CFLAGS) segment.cc segment.h lld.h $(LIBS)
mklfs: mklfs.cc mklfs.h flash.o log.o superblock.o checkpoint.o
	$(CXX) $(CFLAGS) mklfs.cc mklfs.h flash.o log.o superblock.o checkpoint.o -o mklfs lld.h $(LIBS)
lfs: lfs.cc lfs.h flash.o log.o superblock.o checkpoint.o
	$(CXX) $(CFLAGS) lfs.cc lfs.h flash.o log.o superblock.o checkpoint.o -o lfs 
lfsck: lfsck.cc lfsck.h flash.o log.o superblock.o checkpoint.o
	$(CXX) $(CFLAGS) lfsck.cc lfsck.h flash.o log.o superblock.o checkpoint.o -o lfsck 

segtest: segment.o flash.o block.o
	$(CXX) $(FLAGS) segtest.cc segment.o flash.o block.o -o segtest $(LIBS)
clean:
	rm -f *.o $(PROG)
