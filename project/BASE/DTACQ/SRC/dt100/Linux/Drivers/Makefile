# Comment/uncomment the following line to disable/enable debugging
DEBUG = n

# Kernel headers:
# This assumes that kernel sources install to convention:
# /usr/src/linux-2.4.20-8/include
# If this directory doesn't exist, we recommend setting it up via a soft link

INCLUDEDIR = "/usr/src/linux-`uname -r`/include"
#INCLUDEDIR = "/usr/src/linux-2.4.20-8/include"

ifeq ($(DEBUG),y)
  DEBFLAGS = -O -g -DPGM_DEBUG # "-O" is needed to expand inlines
else
  DEBFLAGS = -O2
endif

CFLAGS = -D__KERNEL__ -Wall $(DEBFLAGS) -DLINUX_NEW_PCI

DTACQ_INCLUDES		= ../../../include
DT100_INCLUDES		= ../../include
DTACQ_LINUX_INCLUDES	= ../include

CFLAGS += \
	-I$(INCLUDEDIR) \
	-I$(DTACQ_INCLUDES) \
	-I$(DTACQ_LINUX_INCLUDES) \
	-I$(DT100_INCLUDES) \

# Extract version number from headers.
VER = $(shell awk -F\" '/REL/ {print $$2}' $(INCLUDEDIR)/linux/version.h)

TARGET = acq32-drv
OBJS = $(TARGET).o 
SRC = \
	acq32_core.c \
	acq32_simul.c \
	acq32_utils.c \
	acq32_command.c \
	acq32_fungen.c \
	acq200_bigbuf_alloc.c \
	debug.c \
	strtol.c 

ACQ200SRC = \
	$(SRC) \
	acq200_module.c \
	acq200.c \
	wavegen.c \

ACQ32SRC = \
	$(SRC) \
	acq32_mmap.c \
	acq32.c \
	acq32_specific.c

DTLBIN= ../bin
MODULES=../modules

APPS = reset-driver acq32_debug acqcmd irq acq2sh
MODS = \
	acq32-drv.o \
	acq200-drv.o \
	acq32-mknod.pl \
	acq32.load \
	acq200.load \
	cmdline_lookup \
	getBoards.pm \
	acq32-init.pl

DC := $(shell date +%y%m%d%H%M)

#all: .depend $(TARGET).o $(APPS)
all: .depend acq32-drv.o acq200-drv.o $(APPS)

release install: all
	@-mkdir $DTLBIN
	cp $(APPS) $(DTLBIN)
	@-mkdir $(MODULES)
	cp $(MODS) $(MODULES)
	echo $(VER) >../linux_version


#$(TARGET).o: $(SRC:.c=.o)
#	$(LD) -o $@ -r $^

acq32-drv.o: $(ACQ32SRC:.c=.o)
	$(LD) -o $@ -r $^

acq200-drv.o: $(ACQ200SRC:.c=.o)
	$(LD) -o $@ -r $^

reset-driver : reset-driver.o
	$(CC) -o $@ $^ -lpopt -lc

acqcmd: acqcmd.o
	$(CC) -o $@ $^ -lpopt -lc

bigread: bigread.o
	$(CC) -o $@ $^ -lpopt -lc

acq2sh: acq2sh.o
	$(CC) -o $@ $^ -lpopt -lc



install_direct:
	mkdir -p /lib/modules/$(VER)/misc /lib/modules/misc
	install -c $(TARGET).o /lib/modules/$(VER)/misc
	install -c $(TARGET).o /lib/modules/misc


clean:
	rm -f $(APPS)
	rm -f *.o *~ core .depend

depend .depend dep:
	$(CC) $(CFLAGS) -MM *.c > .depend


copy: $(SRC:.c=.o)
	echo copy $^


gat-errors:
	cat make.log

PACKAGE_FILES=*.[ch] Makefile *.load *.pl

package:
	tar cvzf acq32-drv.$(DC).tgz $(PACKAGE_FILES)
	cvs tag acq32_drv_$(DC)
	echo package acq32-drv.$(DC).tgz created

ifeq (.depend,$(wildcard .depend))
include .depend
endif









