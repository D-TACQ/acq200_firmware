/*****************************************************************************
 *
 * File: htstream.c
 *
 * $RCSfile: htstream.cpp,v $
 * 
 * Copyright (C) 2001 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: htstream.cpp,v 1.22 2003/06/03 09:04:30 pgm Exp $
 * $Log: htstream.cpp,v $
 * Revision 1.22  2003/06/03 09:04:30  pgm
 * fix channel count bug
 *
 * Revision 1.21  2003/06/02 20:51:26  pgm
 * HTSTREAM V2 single thread good
 *
 * Revision 1.20  2003/06/02 09:44:36  pgm
 * new style
 *
 * Revision 1.19  2003/05/31 15:41:54  pgm
 * fully local status
 *
 * Revision 1.18  2003/05/08 11:28:32  pgm
 * add pseudo-code doc
 *
 * Revision 1.17  2003/05/07 21:20:55  pgm
 * further refactor to ensure buffer validation for driver
 *
 * Revision 1.16  2003/05/07 20:56:20  pgm
 * fix big_buf ref, channels, doc
 *
 * Revision 1.15  2002/08/12 11:56:03  pgm
 * streaming works to 1.3MHz, 41MB/sec
 *
 * Revision 1.14  2002/08/12 10:56:07  pgm
 * streaming works, display from bigdump buf
 *
 * Revision 1.13  2002/04/14 10:58:32  pgm
 * this seems to work (don't use heap after fftw :-))
 *
 * Revision 1.12  2002/04/14 07:02:24  pgm
 * test mode
 *
 * Revision 1.11  2002/04/12 10:35:47  pgm
 * added fftw code. segfaults :-(
 *
 * Revision 1.10  2002/04/12 08:52:00  pgm
 * decoupled factory arch runs
 *
 * Revision 1.9  2002/04/11 16:30:20  pgm
 * recycle must work
 *
 * Revision 1.8  2002/04/11 16:05:01  pgm
 * sensible histogram, fixes seg fault
 *
 * Revision 1.7  2002/04/11 13:14:50  pgm
 * recycle, histo in test
 *
 * Revision 1.6  2002/04/11 10:04:27  pgm
 * local poll and err det works
 *
 * Revision 1.5  2002/04/10 19:53:26  pgm
 * first runner
 *
 * Revision 1.4  2002/04/10 18:23:11  pgm
 * first runner
 *
 * Revision 1.3  2002/04/10 15:00:04  pgm
 * *** empty log message ***
 *
 * Revision 1.2  2002/04/10 13:14:32  pgm
 * compiles
 *
 * Revision 1.1  2002/04/10 12:36:47  pgm
 * *** empty log message ***
 *
 * Revision 1.1  2002/04/02 10:25:14  pgm
 * *** empty log message ***
 *
 *
 * Revision 1.1  1999/10/22 16:26:49  pgm
 * first entry to cvs
 *
 *
\*****************************************************************************/

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



#define HELP \
"htstream $Revision: 1.22 $\n"\
"htstream [opts] [length-bytes]\n"\
"\n"\
"opts:\n"\
"    -b board\n"\
"    -m msecs - monitor\n"\
"    -r recycle - recycle buffer\n"\
"    -l - use local status\n"\
"    -s keys - select a sink\n"\
"    -k - enumerate available sink keys\n"\
"    -t test mode (no acq32 required)\n"\
"    -M mask - use this interrupt mask\n"\
"    -c channels - num channels (default 16)\n"\
"\n"

/*
 * globals (keep to minimum)
 */
int verbose = 0;



#define BYTES_PER_SAMPLE  (nchannels*2)   

/*
 * signal handling - worktodo
 */




#include <signal.h>

class AbortHandler {

	static HTController* htc;
	static int board;
public:
	static void handler( int signum ) {
		Mailboxes mbx( board );
		mbx.printerr();
		htc->abort();
		exit( signum );
	}
	static void setup( int _board,  HTController* _htc ) {
		static struct sigaction  def_action = { { handler } };

		htc = _htc;
		board = _board;
		sigaction( SIGINT, &def_action, 0 );
	}
};

int AbortHandler::board;

HTController* AbortHandler::htc;


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

int testMode( Sink* my_sink, int length ) 
{
	//    static unsigned pbuffer[1024];
	unsigned* pbuffer = new unsigned[1024];

	fprintf( stderr, "testMode() start\n" );
	//    my_sink->init();	
	for ( int nbytes = 0; nbytes < length; nbytes += 1024*4 ){
		my_sink->sink( (unsigned)pbuffer, 1024*4 );
	}

	my_sink->final();
	fprintf( stderr, "sinks done\n" );

	//    delete [] pbuffer;
	return 0;
}

int testMode( Sink* my_sink, ContiguousDmaBuffer& cdb )
	// test on real data
{
	unsigned pbuffer = (unsigned)cdb.getVaddr();

	for ( unsigned nbytes = 0; nbytes <= cdb.getLength(); nbytes += 1024*4 ){
		my_sink->sink( pbuffer, 1024*4 );
		pbuffer += 1024*4;
	}

	my_sink->final();
	fprintf( stderr, "sinks done\n" );
	return 0;
}

int main( int argc, const char* argv[] )
{
	int rc;
	unsigned length = DEFLEN;
	const char* arg;
	char* outfname = NULL;
	char* sink_keys = NULL;

	int fd_out = -1;
	int board = 1;
	int monitor = 0;
	int use_local_status = 0;
	int recycle = 0;
	int nchannels = 12;
	unsigned channel_mask = 0x1;
	int test = 0;
	unsigned mask = 0;

    
	// meanwhile, last sink wins

	struct poptOption opt_table[] = {
		{ "help",       'h', POPT_ARG_NONE,   0,             'h'  },
		{ "verbose",    'v', POPT_ARG_INT,    &verbose,       1   },
		{ "output",     'o', POPT_ARG_STRING, &outfname,     'o'  },
		{ "sink",       's', POPT_ARG_STRING, &sink_keys,    's'  },
		{ "keys",       'k', POPT_ARG_NONE,   0,             'k'  },
		{ "board",      'b', POPT_ARG_INT,    &board,         1   },
		{ "monitor",    'm', POPT_ARG_INT,    &monitor,       0   },
		{ "local",      'l', POPT_ARG_NONE,   0,              'l' },
		{ "recycle",    'r', POPT_ARG_NONE,   0,              'r' },
		{ "test",       't', POPT_ARG_NONE,   0,              't' },
		{ "mask-ints",  'M', POPT_ARG_INT,    &mask,          0 },
		{ "channels",   'c', POPT_ARG_INT,    &nchannels,      0 },
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
		case 't':
			test++;
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


	Sink* my_sink = SinkFactory::inst().instantiateSink( 
			            sink_keys, nchannels, channel_mask );
	my_sink->init();

	if ( test==1 ) {
		return testMode( my_sink, length );
	}


	ContiguousDmaBuffer& cdb = 
		ContiguousDmaBuffer::getBuffer( board, length+1024*2 );

	if ( test==2 ) {
		return testMode( my_sink, cdb );
	}
	HTController& htc = HTController::getHTController( board );

	htc.requestLocalStatus( use_local_status );
	htc.setRecycle( recycle );

	if ( mask ){
		fprintf( stderr, 
			 "WARNING: setting interrupt mask %08x\n", mask );
		htc.setInterruptMask( mask );
	}
	AbortHandler::setup( board, &htc );
	if ( !mask && monitor ){
		Monitor::setup( board, monitor );
	}
    

	if ( verbose ) {
		my_sink->print();
	}
	//    fprintf( stderr, "Quit while the goings good\n" );
	//    return 1;

	htc.registerSink( my_sink );
	htc.run( length, cdb, BYTES_PER_SAMPLE );

	if ( mask ){
		htc.clrInterruptMask( mask );
	}
	Monitor::quit = 1;
	return 0;
}
