# Comment/uncomment the following line to disable/enable debugging
DEBUG = y

# Change it here or specify it on the "make" commandline
INCLUDEDIR = /usr/include

ifeq ($(DEBUG),y)
  DEBFLAGS = -O -g # "-O" is needed to expand inlines
else
  DEBFLAGS = -O2
endif

DTINC = ../../../../include
DTLINC= ../../include

CPPFLAGS =  -Wall $(DEBFLAGS)
CPPFLAGS += -I$(INCLUDEDIR) -I${DTINC} -I${DTLINC}

all: .depend acq32dump

release install: all
	cp acq32dump ../../bin

clean:
	rm -f *.o acq32dump .depend

acq32dump: acq32dump.o
	$(LINK.c) -o $@ $^ -lpopt

depend .depend dep:
	$(CC) $(CPPFLAGS) -MM *.c > .depend

ifeq (.depend,$(wildcard .depend))
include .depend
endif

