CC=arm-xscale-linux-gnu-gcc
CFLAGS+=-Wall -O -D__arm -I../include
LFLAGS=-L${HOME}/PROJECTS/ACQ200/tools/lib -L../lib
#LFLAGS=-L../lib



all: .depend dt100d dt100-stream tblock-stream

dt100d.OBJS = tcpserver.o masterinterp.o

dt100d: $(dt100d.OBJS)
	$(CC) $(LFLAGS) -o $@ $(dt100d.OBJS) -lpopt

dt100-stream.OBJS = dt100-stream.o

dt100-stream: $(dt100-stream.OBJS)
	$(CC) $(LFLAGS) -o $@ $(dt100-stream.OBJS) -lpopt -lacq -lm


tblock-stream.OBJS = tblock-stream.o

tblock-stream: $(tblock-stream.OBJS)
	$(CC) $(LFLAGS) -o $@ $(tblock-stream.OBJS) -lpopt -lacq

clean:
	rm -f *.o *~ dt100d dt100-stream


release: all
	cp dt100d ../../rootfs/extra/local/bin/
	cp dt100-stream ../../rootfs/extra/local/bin/

depend .depend dep:
	$(CC) $(CFLAGS) $(CPPFLAGS) -M *.c >> .depend
