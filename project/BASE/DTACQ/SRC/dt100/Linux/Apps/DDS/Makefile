#CC=arm-xscale-linux-gnu-gcc
#CFLAGS=-I../include -I../libtommath 
CFLAGS=-I../include -I../libtommath -I ../../../../include/
LFLAGS=-L../lib

LTOMMATH=../libtommath/libtommath.a

all: .links dds

DDSOBJS=dds.o

ptest: ptest.o
	$(CC) $(LFLAGS) -o ptest ptest.o  $(LTOMMATH) -lpopt

dds: dds.o
	$(CC) $(LFLAGS) -o dds dds.o  $(LTOMMATH) -lpopt

clean:
	rm -f *.o dds *~ .links

release:
	cp dds ../../rootfs/extra/local/bin/

.links:
	ln -s ../BASE/DTACQ/SRC/include/local.h local.h
	ln -s ../BASE/DTACQ/SRC/include/popt.h popt.h
	echo links >.links
