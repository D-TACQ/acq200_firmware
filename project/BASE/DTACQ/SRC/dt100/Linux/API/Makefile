# Comment/uncomment the following line to disable/enable debugging
DEBUG = y

# Change it here or specify it on the "make" commandline
INCLUDEDIR = /usr/include

ifeq ($(DEBUG),y)
  DEBFLAGS = -O -g -DPGM_DEBUG # "-O" is needed to expand inlines
else
  DEBFLAGS = -O2
endif

CFLAGS = -Wall $(DEBFLAGS)

DTACQ_INCLUDES		= ../../../include
DT100_INCLUDES		= ../../include
DTACQ_LINUX_INCLUDES	= ../include

CFLAGS += \
	-I$(INCLUDEDIR) \
	-I$(DTACQ_INCLUDES) \
	-I$(DTACQ_LINUX_INCLUDES) \
	-I$(DT100_INCLUDES) \

# Extract version number from headers.

all: .depend dt100test 

release install: all
	cp dt100test ../bin

dt100test: dt100test.o dt100api.o
	$(CC) -o $@ $^ -lpopt

clean:
	rm -f *.o *~ core .depend

depend .depend dep:
	$(CC) $(CFLAGS) -MM *.c > .depend


ifeq (.depend,$(wildcard .depend))
include .depend
endif








