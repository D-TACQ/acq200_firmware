/*****************************************************************************
 *
 * File: htstream2.cpp - mthread version of htstream
 *
 * $RCSfile: htstream2.cpp,v $
 * 
 * Copyright (C) 2001 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: htstream2.cpp,v 1.7 2003/06/22 13:43:08 pgm Exp $
 * $Log: htstream2.cpp,v $
 * Revision 1.7  2003/06/22 13:43:08  pgm
 * DOCUMENTATION for htstream2
 *
 * Revision 1.6  2003/06/22 12:42:38  pgm
 * local status works with correct data
 *
 * Revision 1.5  2003/06/22 09:45:35  pgm
 * status interrupt -l -l
 *
 * Revision 1.4  2003/06/03 12:18:45  pgm
 * htstream2 - multi thread, interrupt driven, ship it quick
 *
 * Revision 1.3  2003/06/03 10:21:18  pgm
 * holdoff != 0
 *
 * Revision 1.2  2003/06/03 09:04:23  pgm
 * fix channel count bug
 *
 * Revision 1.1  2003/06/02 20:51:26  pgm
 * HTSTREAM V2 single thread good
 *
\*****************************************************************************/

# ifdef DOCUMENTATION
 * Differences to htstream:
 *
 * multi board, multithread.
 *
 * specify boards -b 1234
 *
 * defaults to interrupt driven behaviour. First board is the interruptor
 *
 * to disable interrupts -p 1
 *
 * option for local status buffer: this is a buffer with one status element
 * per sample, allows thread to monitor status WITHOUT polling the board
 * on pci at all. Allows good buffer management, sensitive to overrruns.
 *
 * to enable local status buffer: -l -l
 *
 * in addition, if permitted (ie run as root), threads will run at 
 * SCHED_FIFO (POSIX realtime priority).
 *
 * example operation:

[dt100@cpv5350 dt100]$ cat /proc/cmdline
rw root=/dev/ram0 ramdisk_size=32000 mem=80M acq32_big_buf_base=0x07000000 acq32_big_buf_len=0x01000000 fastboot nomodules nousb init=/linuxrc 

( I use a 128MB system, 16MB bigbuf ).

The driver load picks this up correctly as:
[dt100@cpv5350 dt100]$ cat /proc/.acq32.globs 
MODULE_PARMS

            acq32_big_buf_base:0x07000000
             acq32_big_buf_len:0x01000000


Example operation with 4 boards:

[dt100@cpv5350 dt100]$ for b in 1 2 3 4
> do
> acqcmd -b $b setInternalClock 200000
> done

bash-2.05a# /home/dt100/bin/htstream2 -b 4321 -l -l -r -c32 -s S 0x100000

For my particular system, 4 is a good choice of master board as its interrupt
is not shared. The system will work with shared interrupts, but it may be more
unstable.

For my particular system (PIII-500), 4 boards, 200kHz is safe.
This translates to about 12k Interrupts per second.
I found it to be marginal at 250kHz.

The interrupt handling is not optimal - essentially we are reading from the
"status" device; each interrupt yields a 40+ character line of ascii data;
clearly this could be made much more efficient, most likely by using a
dedicated ioctl instead.

#endif

/*
 * psuedo-code for core operation:

 HTimpl::run() {
 htp->enterHT();         // set up HT mode on ACQ32
 doRun() {
 htp->setBuflen( length );
 htp->setRecycle( recycle );     // continuous or one-shot
 htp->setAddr( cdb );            // set host target base addr

 htp->setArm();                  // start Capture

 do {
 poll status, abort on overrun error

 run sink()              // user data processing
 }
 }
 }

*/

/*
 * BUFFER MANAGEMENT
 *

 Target buffer is located in the HOST PC memory.
 This memory needs to be hidden from LINUX at boot time by using
 a mem= command line statment.

 Experience with RedHat 7.3 (unlike 6.2) shows that a buffer should be left
 between the mem= top boundary and the buffer area.

 In addition, "big_buf" parameters have to be set at load time for acq32-drv
 to identify the location of this buffer.

 Htstream needs to know the same physical memory values, but now is able
 to pick up this information from the driver.

 Example:

 System with 256 MB memory, allocate a 128MB big_buf:

 Commandline: mem=96M

 ( found to be a good value by trial and error ).

 Load parameters (add to acq32.load) :

 acq32_big_buf_base=0x08000000
 acq32_big_buf_len=0x08000000

*/

/*
 * HTSTREAM protocol
 *

 Uses simple messaging with the mailbox registers on the ACQ32PCI
 Please see acq32busprot.h

 Buffer length needs to be an integer number of MB
 HTSTREAM is capable of operating in a cyclic buffer mode.

 The acquired data is available from two sources:

 #1 Post capture, by using the /dev/acq32/acq32.b.cc nodes as usual
 #2 During and after capture, by mapping /dev/acq32/acq32.b.host

 the mmap utility is suitable for this purpose.

 NB NB NB: When using mmap, the channel sequence is the ACQ32 RAW
 physical channel sequence, so the following mapping has to be applied:

 #define PHYSCHAN( ilogpair )  ((((ilogpair)&0x3)<<2)+(((ilogpair)&0xc)>>2))

 #!/usr/bin/perl

 sub phys {
 my ( $log ) = @_;
 my $ilogpair = int $log/2;
 my $iphyspair = (($ilogpair&0x3)<<2) + (($ilogpair&0xc)>>2);

 return $iphyspair*2 + ($log&1);
 }
 for ( $i = 0; $i <= 31; ++$i ){
 printf( "logical %02d physical %02d\n", $i+1, phys($i)+1 );
 }

 Output:
 [pgm@lewis GASH]$ ./acq32pci.logchans 
 logical 01 physical 01
 logical 02 physical 02
 logical 03 physical 09
 logical 04 physical 10
 logical 05 physical 17
 logical 06 physical 18
 logical 07 physical 25
 logical 08 physical 26
 logical 09 physical 03
 logical 10 physical 04
 logical 11 physical 11
 logical 12 physical 12
 logical 13 physical 19
 logical 14 physical 20
 logical 15 physical 27
 logical 16 physical 28
 logical 17 physical 05
 logical 18 physical 06
 logical 19 physical 13
 logical 20 physical 14
 logical 21 physical 21
 logical 22 physical 22
 logical 23 physical 29
 logical 24 physical 30
 logical 25 physical 07
 logical 26 physical 08
 logical 27 physical 15
 logical 28 physical 16
 logical 29 physical 23
 logical 30 physical 24
 logical 31 physical 31
 logical 32 physical 32

*/

/*
 * htstream
 */
#include "local.h"



#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <math.h>

#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

#include <popt.h>

#include "acq32ioctl.h"
#include "acq32busprot.h"

#include "../LOWLATENCY/llif.h"
#include "htif.h"
#include "htprotocol.h"
#include "sink.h"
#include "sinkfactory.h"
#include "mbox.h"

#define DEFLEN 0x100000


#include "mthread.h"

#define HELP \
"htstream2 $Revision: 1.7 $\n"\
"htstream2 [opts] [length-bytes]\n"\
"\n"\
"opts:\n"\
"    -b boardlist - eg -b 4321, control b1 thru 4, b4 is interruptor\n"\
"    -m msecs - monitor\n"\
"    -r recycle - recycle buffer\n"\
"    -l - use local status\n"\
"    -l -l - use local status buffer\n"\
"    -s keys - select a sink\n"\
"    -k - enumerate available sink keys\n"\
"    -t test mode (no acq32 required)\n"\
"    -M mask - use this interrupt mask\n"\
"    -c channels - num channels (default 16)\n"\
"    -p 1 - polled\n" \
"\n"

/*
 * globals (keep to minimum)
 */
int verbose = 0;

#define VPRINTF if ( verbose ) printf

#define MAXBOARDS 12

#define BYTES_PER_SAMPLE  (nchannels*2)   

/*
 * signal handling - worktodo
 */




#include <signal.h>

class AbortHandler {

	static struct AbortHandlerData{
		HTController* htc;
		int board;
	} clients[MAXBOARDS];
	static int nclients;
	
public:
	static void handler( int signum ) {
		for ( int iclient = 0; iclient != nclients; ++iclient ){
			int board = clients[iclient].board;
		        HTController* htc = clients[iclient].htc;

			Mailboxes mbx( board );
			mbx.printerr();
			htc->abort();
		}
		exit( signum );
	}
	static void addClient( int _board,  HTController* _htc ) {
		static struct sigaction  def_action = { { handler } };

		assert( nclients < MAXBOARDS );

		clients[nclients].htc = _htc;
		clients[nclients].board = _board;

		sigaction( SIGINT, &def_action, 0 );

		++nclients;
	}

};

struct AbortHandler::AbortHandlerData AbortHandler::clients[MAXBOARDS];
int AbortHandler::nclients;



class Monitor {

	static Mailboxes* a;
	static Mailboxes* b;
public:
	static int quit;
	static void handler( int signum )
	{
		Mailboxes* t;

		if ( !a->equals( *b )  || verbose > 1 ){
			a->printerr();
			SWAP( a, b, t );
		}
		if ( quit ){
			setup( 0, 0 );
			fprintf( stderr, "monitor quitting\n" );
		}           
	}

	static void setup( int board, int millisec )
	{
		static struct sigaction monitor_action = { { handler } };
		static struct itimerval new_timer = {};

		a = new Mailboxes( board );
		b = new Mailboxes( board );

		new_timer.it_value.tv_usec    =
			new_timer.it_interval.tv_usec = millisec*1000;
    
		sigaction( SIGALRM, &monitor_action, 0 );
		setitimer( ITIMER_REAL, &new_timer, 0 );
	}
};

Mailboxes* Monitor::a;
Mailboxes* Monitor::b;
int Monitor::quit;



class HTCSlaveThread: public SlaveThread {
	HTController& htc;
		
	const char* makeName( int board ) {
		static char buf[80];
		sprintf( buf, "Slave %d\n", board );
		return buf;
	}

public:
	HTCSlaveThread( int board, HTController& _htc ) :
		SlaveThread( makeName( board ) ),
		htc( _htc )
	{}
	unsigned getCurrentAddr() {
		return htc.getCurrentAddr();
	}
	virtual void exec() {
		htc.start();
		SlaveThread::exec();
	}
	virtual void exec_payload() {
		// approx true - need a one shot run method
		htc.processAvailableData();
	}	       
	friend class HTMasterThread;
	friend class HTInterruptMasterThread;
};

class HTMasterThread : public MasterThread {

protected:
	int holdoff;
	int status_board;
	HTCSlaveThread** slaves;
	int nslaves;
	

	// Singleton
protected:
	HTMasterThread() :
		MasterThread( "HTMasterThread" ),
		holdoff(0), status_board( 0 )
	{
		slaves = new HTCSlaveThread* [MAXBOARDS];
		nslaves = 0;
	}
public:
	virtual void addSlave( SlaveThread* slave ) {

		slaves[nslaves++] = (HTCSlaveThread*)slave;

		if ( nslaves == 1 ){
			status_board = slaves[0]->htc.getBoard();
		}
		MasterThread::addSlave( slave );
	}

	void setHoldoff( int _holdoff ) {
		holdoff = _holdoff;
	}

	static HTMasterThread* instance( int polled );
};

class HTInterruptMasterThread : public HTMasterThread {



	const char* makeStatusName() {
		static char buf[80];
		sprintf( buf, "/dev/acq32/acq32.%d.stat", status_board );
		return buf;
	}
public:
	virtual void addSlave( SlaveThread* slave ) {
		HTMasterThread::addSlave( slave );
		if ( nslaves == 1 ){
			slaves[0]->htc.setHoldoff( holdoff );
			status_board = slaves[0]->htc.getBoard();
		}
	}
	virtual void exec() {

		/* open status device */
		FILE* fp = fopen( makeStatusName(), "r" );
		char buf[80];

		assert( fp );

		while( fgets( buf, 80, fp ) ){
			PRINTF(2)( "Status Change %s", buf );
			broadcast();
		}

		fclose( fp );
		/* signal every status change */
	}
};

class HTPollingMasterThread : public HTMasterThread {
public:
	virtual void exec() {

		unsigned old_addr = 0xdeadbeef;

		while (1) {
			while( old_addr == slaves[0]->getCurrentAddr() ){
				usleep( 1000 );
			}
			broadcast();
		}
		/* signal every status change */
	}
};

HTMasterThread* HTMasterThread::instance( int polled ) {
	if ( polled ){
		return new HTPollingMasterThread;
	}else{
		return new HTInterruptMasterThread;
	}
}




int main( int argc, const char* argv[] )
{
	int rc;
	unsigned length = DEFLEN;
	const char* arg;
	char* outfname = NULL;
	char* sink_keys = NULL;

	int fd_out = -1;
	char* boards = "1";
	int monitor = 0;
	int use_local_status = 0;
	int recycle = 0;
	int nchannels = 12;
	unsigned channel_mask = 0x1; // potentially used by sinks
	unsigned mask = 0;
	int polling_master = 0;

	// meanwhile, last sink wins

	struct poptOption opt_table[] = {
		{ "help",       'h', POPT_ARG_NONE,   0,             'h'  },
		{ "verbose",    'v', POPT_ARG_INT,    &verbose,       1   },
		{ "output",     'o', POPT_ARG_STRING, &outfname,     'o'  },
		{ "sink",       's', POPT_ARG_STRING, &sink_keys,    's'  },
		{ "keys",       'k', POPT_ARG_NONE,   0,             'k'  },
		{ "board",      'b', POPT_ARG_STRING, &boards,        1   },
		{ "monitor",    'm', POPT_ARG_INT,    &monitor,       0   },
		{ "local",      'l', POPT_ARG_NONE,   0,              'l' },
		{ "recycle",    'r', POPT_ARG_NONE,   0,              'r' },
		{ "mask-ints",  'M', POPT_ARG_INT,    &mask,          0   },
		{ "channels",   'c', POPT_ARG_INT,    &nchannels,     0   },
		{ "channel-mask",'m', POPT_ARG_INT,   &channel_mask,  0   },
		{ "polling-master", 'p', POPT_ARG_INT,&polling_master, 0 },
		{ }
	};

	poptContext opt_context;

	opt_context = poptGetContext( argv[0], argc, argv, opt_table, 0 );

	while ( (rc = poptGetNextOpt( opt_context )) > 0 ){
		switch( rc ) {
		case 'h':
			fprintf( stderr, HELP );
			return 1;
		case 'o':
			if ( outfname[0] == '-' ){
				fd_out = 1;                // use stdout
			}else{
				fd_out = open( outfname, 
					       O_WRONLY|O_CREAT|O_TRUNC, 0666 );

				if ( fd_out < 0 ){
					perror( "failed to open output file" );
					return fd_out;
				}
			}
			break;
		case 'k':
			SinkFactory::inst().print();
			return 0;
		case 'l':
			use_local_status++;
			break;
		case 'r':
			recycle = 1;
			break;
		case 's':
			// wait till all the prams are in
			break;
		}
	}

	if ( (arg = poptGetArg( opt_context )) != NULL ){
		length = strtoul( arg, 0, 0 );
	}
	fprintf( stderr, "length %u\n", length );


	HTMasterThread* master = HTMasterThread::instance( polling_master );

	for ( int board, ib = 0; 
	      boards[ib] && (board=boards[ib]-'0') &&
		      IN_RANGE( board, 1, MAXBOARDS );
	      ++ib ){

		Sink* my_sink = SinkFactory::inst().instantiateSink( 
					sink_keys, nchannels, channel_mask );

		//    my_sink->init( length );
		my_sink->init();


		ContiguousDmaBuffer& cdb = 
			ContiguousDmaBuffer::getBuffer( board, length+1024*2 );

		HTController& htc = HTController::getHTController( board );

		htc.setRunLength( length );
		htc.setBuffer( &cdb );
		htc.setBytesPerSample( BYTES_PER_SAMPLE );
		htc.requestLocalStatus( use_local_status );
		htc.setRecycle( recycle );

		if ( ib == 0 ){
			if ( mask ){
				fprintf( stderr, 
					 "WARNING:setting int mask %08x\n", 
					 mask );
				htc.setInterruptMask( mask );
			}
			if ( !mask && monitor ){
				Monitor::setup( board, monitor );
			}
			if ( verbose ) {
				my_sink->print();
			}
		}
		AbortHandler::addClient( board, &htc );
		htc.registerSink( my_sink );
		
		master->addSlave( new HTCSlaveThread( board, htc ) );
	}

	master->create();
	master->join();

	Monitor::quit = 1;
	return 0;
}
