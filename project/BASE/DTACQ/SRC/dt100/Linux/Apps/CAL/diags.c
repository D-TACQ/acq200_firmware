/*****************************************************************************
 *
 * File: diags.c
 *
 * $RCSfile: diags.c,v $
 * 
 * Copyright (C) 1999 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description: diagnostic dump from acq32
 *
 * $Id: diags.c,v 1.21 2002/11/14 12:10:38 pgm Exp $
 * $Log: diags.c,v $
 * Revision 1.21  2002/11/14 12:10:38  pgm
 * clr works better with sleep
 *
 * Revision 1.20  2002/11/01 14:25:48  pgm
 * clr, lazy opts
 *
 * Revision 1.19  2002/06/13 20:07:19  pgm
 * fix redefined message
 *
 * Revision 1.18  2001/11/19 14:33:09  pgm
 * quite on too many errors
 *
 * Revision 1.17  2001/11/05 22:19:52  pgm
 * ACQCMD_DEV opt included
 *
 * Revision 1.16  2001/11/05 21:50:06  pgm
 * bidir works
 *
 * Revision 1.14  2001/07/11 19:04:34  pgm
 * 32 sampel AI
 *
 * Revision 1.13  2001/07/11 15:42:55  pgm
 * revise for HISTOLEN 32
 *
 * Revision 1.12  2001/06/29 11:32:18  pgm
 * pre-release
 *
 * Revision 1.9  2001/06/23 20:52:46  pgm
 * mean means something - use it
 *
 * Revision 1.8  2001/05/15 11:05:32  pgm
 * add cumulative
 *
 * Revision 1.7  2001/05/14 17:54:33  pgm
 * force 32 bit snapshot
 *
 * Revision 1.6  2001/05/14 17:40:32  pgm
 * snapshot diags first
 *
 * Revision 1.5  2001/05/13 18:17:31  pgm
 * AO histo
 *
 * Revision 1.4  2001/04/08 19:08:50  pgm
 * update help
 *
 * Revision 1.3  2001/01/02 14:21:25  pgm
 * interactive mode, auto calcals pollcat
 *
 * Revision 1.2  2000/10/14 22:39:22  pgm
 * ASSERT dump
 *
 * Revision 1.1  2000/10/14 21:36:49  pgm
 * diags simple histo works
 *
 *
\*****************************************************************************/





#undef __KERNEL__

#include "local.h"

#include "ctype.h"

#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

#include <stdlib.h>
#include <popt.h>

#define FNAMEF  "/dev/acq32/acq32.%d.raw"

#define HELP \
    "diags $Revision: 1.21 $\n"\
    "usage: diags opts [acq16]\n"\
    "-b board {1,2,3,4}\n"\
    "-i interactive\n"\
    "-A AO histo\n"\
    "-v <n> verbose\n"\
    "-D firmware debug\n"\
    "-S sleep before exit\n"\
    "-Z superlazy\n"\
    ""

#define VPRINT    if(G_verbose)printf

int G_verbose;
int G_sleep;            // sleep on end of input
int G_super_lazy;       // sleep on end of command

#define DIAGS_BASE 0x3f0000
#define MMAP_LEN   0x010000
#define HISTLEN 32

#define COPY_LEN  (3*HISTLEN*sizeof(unsigned))

static void plot_histogram( unsigned* pdata, int ndata, char* title, int do_pollcat )
{
#define HEAD_LINE "%12s %12s %12s\n"
#define PLOT_LINE "%12d %12d %12d|"
#define MAXDOTS   20
#define DOT       '*'

    int idata;
    unsigned totdata;
    int idot;
    int maxdot;
    unsigned samples;
    unsigned totcount = 0;
    unsigned sumsamples = 0;
    float mean;

    totdata = pdata[0];

    for ( idata = 1, samples = 0; idata != ndata; ++idata ){
        samples += pdata[idata]*idata;
        totdata += pdata[idata];
    }

    printf( "Histogram of %s\n\n", title );

    printf( HEAD_LINE, "value", "cumulative", "count" );

    for ( idata = 0; idata != ndata; ++idata ){
        totcount += pdata[idata];
        sumsamples += pdata[idata] * idata;  // zero cancels - we want this!
        printf( PLOT_LINE, idata, totcount, pdata[idata] );

        if ( pdata[idata] != 0 ){
            putchar( DOT );

            maxdot = MAXDOTS*pdata[idata]/totdata;

	    if ( maxdot > MAXDOTS ) maxdot = MAXDOTS;

            for ( idot = 1; idot < maxdot; ++idot ){
		putchar( DOT );
            }
        }
        putchar( '\n' );
    }

    mean = (double)sumsamples/(double)totcount;

    putchar( '\n' );

    if ( samples && do_pollcat ){
        printf( "Average pollcat assuming %d samples is %.2f\n",
		samples, (double)pdata[0]/samples );
	printf( "Mean samples in fifo: %.2f\n", mean );
    }
    putchar( '\n' );
}

static int plot_ao;

static void plot_histograms( unsigned* pdata )
    /*
     * data[HISTLEN] - nfifo
     */
{
    VPRINT( "plot_histogram %p %p %d\n", pdata, &pdata[0], HISTLEN );
     
    plot_histogram( &pdata[0], HISTLEN, "Samples in Fifo", 1 );
      
    if ( plot_ao ){
	VPRINT( "plot_histogram %p %p %d\n", pdata, &pdata[HISTLEN], HISTLEN );
	plot_histogram( &pdata[HISTLEN], 16, "AO samples in Fifo", 0 );
    }
}

static void plot_acq16( unsigned* pdata )
{
    plot_histogram( &pdata[0], HISTLEN, "acq16 samples in FIFO *3/8", 1 );
}

static void u32_memcpy( void* dest, void* src, int nbytes )
{
    unsigned* dp = (unsigned*)dest;
    unsigned* sp = (unsigned*)src;
    int nwords = nbytes/sizeof(unsigned);
    
    while ( nwords-- ){
        *dp++ = *sp++;
    }
}



static void plot_status( int board, char* region )
{
    char lbuf[80];
    char* status = region + 2*HISTLEN*sizeof(unsigned);
    int ibuf;


    VPRINT( "plot_status %p\n", status );
    
    lbuf[79] = '\0';

    strncpy( lbuf, status, 80 );
  
    for ( ibuf = 0; ibuf != 80 && lbuf[ibuf]; ++ibuf ){
	if ( !isprint( lbuf[ibuf] ) ){
	    lbuf[ibuf] = '.';
	}
    }

    printf( "acq32.%d status:%s\n", board, lbuf );
}


int fd;            // GLOB :-(

#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include "acq32busprot.h"

#define ENV   "ACQCMD_DEV"             /* overrides default */

#include "acq32ioctl.h"

static int iget = 4;

static int checkStdin(  unsigned usecs, int* xoff )
// non blocking read on stdin return EOF at EOF.
{
    fd_set rfds;
    struct timeval tv;
    int rc;
    
    /* Watch stdin (fd 0) to see when it has input. */
    FD_ZERO(&rfds);
    FD_SET(0, &rfds);
    /* Wait up to five seconds. */
    tv.tv_sec = 0;
    tv.tv_usec = usecs;

    rc = select(1, &rfds, NULL, NULL, &tv);
    /* Don't rely on the value of tv now! */

    if ( rc ){
        static Message message;
	static int imessage;
	    
	/* FD_ISSET(0, &rfds) will be true. */

        if ( fgets( message.payload.cdata, MESSAGE_DATA_LEN, stdin ) ){
	    message.header.id = MAKE_ID_DTACQ( imessage++ );
	    message.header.length = sizeof(MessageHeader)+
		strlen( message.payload.cdata );
	    message.header.type = MAKE_MT_COMMAND( MTC_DEBUG_TEXT );
		    
	    rc = ioctl( fd, ACQ32_IOSENDI2O, &message );

	    if ( strncmp( "clr", message.payload.cdata, 3 ) == 0 ){
		iget = 4;  // clear bufdef this end too
		sleep( 1 );
	    }
	    if ( rc ){
		perror( "ioctl ACQ32_IOSENDI2O failed" );
		return -1;
	    }
	    if ( G_super_lazy ){
		sleep( G_super_lazy );
	    }
	}else{
	    // lazy exit - allow time for any replies to pop
	    if ( G_sleep ){
		sleep( G_sleep );
		return G_sleep = 0;
	    }else{
		return EOF;
	    }
	}
    }

    return 0;
}

static void debugTerminal( const char* fname, void* region )
{

    char* buffer = ((char*)region+0xf000);
    volatile int* pput = (int*)buffer;
    int iput = 4;
    int ilines = 0;
    int xoff = 0;

    printf( "debugTerminal for %s\n", fname );

    do {
        if ( !xoff ){
	    if ( IN_RANGE( *pput, 0x3ff000, 0x400000 ) ){
		iput = *pput - 0x3ff000;

		for ( ; iget != iput; ++iget ){
		    if ( iget >= 0x1000 ){
			iget = 4;
		    }
		    putchar( buffer[iget] );
		    if ( buffer[iget] == '\n' ){
			if ( ++ilines > 24 ){
			    ilines = 0;
			    break;
			}
		    }
		}

	    }else{
		static int nerrors;
		sleep( 1 );
		if ( ++nerrors > 10 ){
		    printf( "region %p pput %p *pput 0x%08x iput %d iget %d\n",
			region, pput, *pput, iput, iget );
		    printf( "too many errors, quitting\n" );
		    return;
		}
	    }
            fflush( stdout );
	}
    }
    while ( checkStdin( 20000, &xoff ) != EOF );
}
int main( int argc, const char* argv[] )
{
    /* WORKTODO ... args handling */


    void* region;
    void* snapshot;
    char fname[80];
    int rc;
    int board = 1;
    int interactive = 0;
    int debug_terminal = 0;
    const char* arg;
    int acq16 = 0;

    struct poptOption opt_table[] = {
        { "help",           'h', POPT_ARG_NONE,         0, 'h' },
        { "board",          'b', POPT_ARG_INT,        &board, 'b' },
        { "interactive",    'i', POPT_ARG_INT,     &interactive, 0 },
        { "AO",             'A', POPT_ARG_NONE,         0, 'A' },
        { "verbose",        'v', POPT_ARG_INT,     &G_verbose, 0 },
    	{ "debug",          'D', POPT_ARG_NONE,      0, 'D' },
	{ "sleep",          'S', POPT_ARG_INT,     &G_sleep, 0 },
	{ "superlazy",      'Z', POPT_ARG_INT,     &G_super_lazy, 0 },
        { }
    };

    poptContext opt_context;

    opt_context = poptGetContext( argv[0], argc, argv, opt_table, 0 );

    // set default fname
    
    if ( getenv( ENV ) ){
        strcpy( fname, getenv( ENV ) );
    }else{
	sprintf( fname, FNAMEF, board );
    }
	
    while ( (rc = poptGetNextOpt( opt_context )) > 0 ){
        switch( rc ){
    	case 'D':
	    debug_terminal = 1;
	    break;
    	case 'A':
	    plot_ao = 1;
	    break;
	case 'b':
	    sprintf( fname, FNAMEF, board );
	    break;
        case 'h':
            fprintf( stderr, HELP );
            return 1;
        }
    }  // processes all other opts via arg pointers

    if ( (arg = poptGetArg( opt_context )) != 0 ){
        if ( STREQ( arg, "acq16" ) ){
	    acq16 = 1;
	    printf( "acq16\n" );
	}
    }
    sprintf( fname, FNAMEF, board );

    if ( (fd = open( fname, O_RDWR )) < 0 ){
        fprintf( stderr, "diags: failed to open device \"%s\" - ", fname );
        perror( "" );
        return 1;
    }

    snapshot = malloc( MMAP_LEN);
    if ( snapshot == 0 ){
        perror( "malloc failed :-(" );
	return 1;
    }
    region = mmap( NULL, MMAP_LEN, PROT_READ|PROT_WRITE, MAP_SHARED, fd, DIAGS_BASE );

    if ( region == (caddr_t)-1 ){
        perror( "mmap" );
        return 1;
    }

    
    if ( debug_terminal ) {
	debugTerminal( fname, region );
	exit( 0 );
    }else{
        do {
	    u32_memcpy( snapshot, region, COPY_LEN );

	    if ( acq16 ){
		plot_acq16( snapshot );	
	    }else{
		plot_histograms( snapshot );
	    }            
	    plot_status( board, snapshot );
        }
	while( interactive && getchar() != -1 );
    }
    return 0;
}





