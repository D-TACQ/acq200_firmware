
INCS=-I. -I../../../include -I../../include \
	-I../../../../../../include
CFLAGS=$(INCS)  -D__arm -O2 -Wall -fomit-frame-pointer
#CFLAGS=	-I. -I../../../include -I../../include -D__arm -g -Wall
LFLAGS=-L${HOME}/PROJECTS/ACQ200/tools/lib 
#CC=/usr/local/arm/2.95.3/bin/arm-linux-gcc
CC=arm-xscale-linux-gnu-gcc -D__arm -D__DTACQ_2G__
#CXX=arm-linux-gcc
#CXXFLAGS=$(CFLAGS)


OBJS= \
	main.o \
	d-bug.o \
	acq200_stubs.o \
	acq200_hal.o \
	acq200_i2o.o \
	acq32cpci_drv.o \
	acq32_drv.o \
	processMessage.o \
	acq200_calApi.o \
	diags.o \
	dlog.o \
	getBuildDate.o \
	hal.o \
	mbox-slave.o \
	local-slave.o \
	usc.o \
	readline.o \
	dataHandling.o \
	lowlatency.o


#	dmac.o \



PROGS=acq200control wavcmd acq216_knobs hex transform

all: .depend $(PROGS)

# DO NOT USE TRANSFORM. Simple shell script does it better
transform: transform.o
	$(CC) $(LFLAGS) -o $@ $^ -lpopt

acq200control: $(OBJS)
	$(CC) $(LFLAGS) -o $@ $^ -lpopt -lpthread


hex: hex.o
	$(CC) $(LFLAGS) -o $@ $^ -lpopt

wavcmd : wavcmd.o
	$(CC) $(LFLAGS) -o $@ $^ -lpopt


set.vin.acq216: set.vin.acq216.o
	$(CC) $(LFLAGS) -o $@ $^ -lpopt

acq216_knobs: acq216_knobs.o
	$(CC) $(LFLAGS) -o $@ $^ -lpopt

clean:
	-rm *.o
	-rm acq200control
	-rm .depend


depend .depend dep:
	$(CC) $(CFLAGS) $(CPPFLAGS) -M *.c >> .depend


EXTRA=${HOME}/PROJECTS/ACQ200/rootfs/extra

deploy: all
	arm-xscale-linux-gnu-strip $(PROGS)
	cp $(PROGS) $(EXTRA)/local/bin
