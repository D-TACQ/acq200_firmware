# Comment/uncomment the following line to disable/enable debugging
DEBUG = y


INCLUDEDIR = /usr/include
DTINC = ../../../../include
DTLINC= ../../include

ifeq ($(DEBUG),y)
  DEBFLAGS = -O -g -DSCULL_DEBUG # "-O" is needed to expand inlines
else
  DEBFLAGS = -O2
endif

CPPFLAGS =  -Wall $(DEBFLAGS)
CPPFLAGS += -I$(INCLUDEDIR) -I$(DTINC) -I$(DTLINC)

all: .depend pfmu
#mu #manu cfidrive

release install: all
	cp pfmu ../../bin

clean:
	rm -f *.o pfmu files.tgz .depend

#	EbsaFlashDevice.o\


OBJS=\
	pfmu.o\
	FlashDevice.o\
	AMDFlashDevice.o\
	ebsa_app_layer.o

pfmu: $(OBJS)
	$(LINK.cpp) -o $@ $^ -lpopt

#fmu: fmu.o flash32.o
fmu: fmu.o 
	$(CC) $(CFLAGS) -o $@ $^ ${HOME}/SANDBOX/EBSA/ebsa_app_layer.o

manu: manu.o flash32.o
	$(CC) $(CFLAGS) -o $@ $^ ${HOME}/SANDBOX/EBSA/ebsa_app_layer.o

depend .depend dep:
	$(CXX) $(CPPFLAGS) -M *.cpp > $@
	$(CC) $(CPPFLAGS) -M *.c >> $@



ifeq (.depend,$(wildcard .depend))
include .depend
endif

