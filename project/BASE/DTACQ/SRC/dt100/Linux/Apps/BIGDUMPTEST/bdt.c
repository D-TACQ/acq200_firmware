/*****************************************************************************
 *
 * File: bdt.c
 *
 * $RCSfile: bdt.c,v $
 * 
 * Copyright (C) 1999 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: bdt.c,v 1.3 2001/01/02 12:35:02 pgm Exp $
 * $Log: bdt.c,v $
 * Revision 1.3  2001/01/02 12:35:02  pgm
 * top-down Make, release targets
 *
 * Revision 1.2  2000/11/12 14:19:04  pgm
 * check serial, start, id
 *
 * Revision 1.1  2000/11/12 08:36:38  pgm
 * first pass
 *
 * Revision 1.2  2000/10/05 21:29:55  pgm
 * bdt can read,write,fill
 *
 * Revision 1.1  2000/10/05 20:56:38  pgm
 * *** empty log message ***
 *
 * Revision 1.1  1999/10/22 16:26:49  pgm
 * first entry to cvs
 *
 *
\*****************************************************************************/





#undef __KERNEL__

#include "local.h"

#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <assert.h>

#include <popt.h>

#define FNAME  "/dev/acq32/acq32.1.host"

#define HELP \
"bdt $Revision: 1.3 $\n"\
"bigdumptest ... first capture data using distributor id\n"\
"usage: bdt .... | bdt [opts] pre [post]\n"\
"-n <nchannels> : default 32\n"\
"-I             : channel ID used\n"\
"-S             : look for sample serial number\n"\
"-v <n>         : set  verbosity\n"\
""

#define START_AT_ZERO 1

#define COUNT_MASK 0x7fff

#define NCHANS 32
#define NPAIRS 16

typedef union {
    unsigned short chan[NCHANS];
    unsigned int pair[NPAIRS];
} Channels;

static int S_verbose;
static int S_nchannels = 32;
static int S_channel_ids = 0;
static int S_serial = 0;

#define IX_SERIAL 4

static void dump( int isample, Channels* sample )
{
    int ichan;

    for ( ichan = 0; ichan != S_nchannels; ){
	if ( ichan%8 == 0 ){
	    printf( "0x%08x ", isample*S_nchannels + ichan );
	}
	printf(  "%04x ", sample->chan[ichan] );
	if ( ++ichan%8 == 0 ){
	    printf( "\n" );
	}
    }
}

static int is_start_record( Channels* test )
{
    int ipair;
    int npairs = S_nchannels/2;

    for ( ipair = 0; ipair != npairs; ++ipair ){
	switch( ipair ){
	case 0:
	case IX_SERIAL:
	    continue;
	default:
	    if ( test->pair[ipair] != 0x55555555 ){
		return 0;
	    }
	}
    }
    return 1;
}
static int channel_id_mismatch( Channels* test )
{
    static Channels model = { {
	0x0003, 0x0003, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 
	0x0002, 0x0000, 0x000a, 0x000b, 0x000c, 0x000d, 0x000e, 0x000f, 
	0x0010, 0x0011, 0x0012, 0x0013, 0x0014, 0x0015, 0x0016, 0x0017, 
	0x0018, 0x0019, 0x001a, 0x001b, 0x001c, 0x001d, 0x001e, 0x001f 
    } };
    static Channels odd_model = { {
	0x0002, 0x0002, 0xfff8, 0xffff, 0xfff0, 0xffff, 0xffe8, 0xffff, 
	0x0001, 0x0000, 0xfff6, 0xffff, 0xffee, 0xffff, 0xffe6, 0xffff, 
	0xfffc, 0xffff, 0xfff4, 0xffff, 0xffec, 0xffff, 0xffe4, 0xffff, 
	0xfffa, 0xffff, 0xfff2, 0xffff, 0xffea, 0xffff, 0xffe2, 0xffff
    } };
    
    {
	int ipair;
	int npairs = S_nchannels/2;

	for ( ipair = 0; ipair != npairs; ++ipair ){
	    switch( ipair ){
	    case 0:
	    case IX_SERIAL:
		continue;
	    default:
		if ( test->pair[ipair] != model.pair[ipair] && 
		     test->pair[ipair] != odd_model.pair[ipair] ){
		    return -1;
		}
	    }
	}

	return 0;
    }
}

static int validate( int nsamples, int start_at_zero )
{
    Channels sample;
    int isample;
    unsigned short this_count = 0;
    static unsigned this_serial;
    int duff = 0;
    int errors = 0;
    int dump_done = 0;

    for ( isample = 0; isample != nsamples; 
	  ++isample, duff = 0, dump_done = 0 ){

	int nbytes = read( 0, &sample, S_nchannels*2 );

	if ( nbytes != S_nchannels*2 ){
	    printf( "read() returned %d at sample %d\n", nbytes, isample );
	    return -1;
	}

	if ( isample == 0 && !start_at_zero ){
	    this_count = sample.chan[0];

	}

	if ( isample == 0 && this_serial == 0 ){
	    this_serial = sample.pair[IX_SERIAL];
	}

	if ( sample.chan[0] != this_count || sample.chan[1] != this_count ){
	    printf( "ERROR at %d, count failed want: 0x%04x got:0x%04x\n",
		     isample, this_count, sample.chan[0] );
	    duff = 1;
	}

	if ( S_serial ){
	    if ( this_serial != sample.pair[IX_SERIAL] ){
		printf( "serial number mismatch at %d, wanted:%d got %d\n",
			isample, this_serial, sample.pair[IX_SERIAL] );
	    }
	}
	
	if ( is_start_record( &sample ) ){
	    printf ( "start record\n" );
	    dump( isample, &sample );
	    dump_done = 1;
	}else if ( S_channel_ids && channel_id_mismatch( &sample ) ){
	    printf( "ERROR at %d, id mismatch\n", isample );
	    duff = 1;
	}

	if ( duff ){
	    dump( isample, &sample );
	    ++errors;
	}else if ( S_verbose && !dump_done ){
	    if ( isample == 0 || isample == 1 || 
		 isample+2 == nsamples || isample+1 == nsamples ){
		dump( isample, &sample );
	    }
	}

	this_count = ++this_count&COUNT_MASK;
	this_serial = ++this_serial;
    }
    return errors;
}

int main( int argc, char** argv )
{
    const char* spre;
    const char* spost;
    int npre;
    int npost = 0;
    int rc;

    struct poptOption opt_table[] = {
        { "help",       'h', POPT_ARG_NONE,         0, 'h' },
	{ "nchannels",  'n', POPT_ARG_INT,    &S_nchannels, 0 },
	{ "channel ID", 'I', POPT_ARG_NONE,         0, 'I' },
	{ "serial",     'S', POPT_ARG_NONE,         0, 'S' },
	{ "verbose",    'v', POPT_ARG_INT,    &S_verbose, 0 },
        { }
    };

    poptContext opt_context;

    opt_context = poptGetContext( argv[0], argc, argv, opt_table, 0 );

    while ( (rc = poptGetNextOpt( opt_context )) > 0 ){
        switch( rc ){
        case 'h':
            fprintf( stderr, HELP );
            return 1;
	case 'I':
	    S_channel_ids = 1;
	    break;
	case 'S':
	    S_serial = 1;
	    break;
        }
    }  // processes all other opts via arg pointers

    spre = poptGetArg( opt_context );

    if ( spre == 0 ){
	fprintf( stderr, "ERROR: " HELP );
	return 1;
    }
    npre = strtol( spre, 0, 0 );

    spost = poptGetArg( opt_context );

    if ( spost ) npost = strtol( spost, 0, 0 );

    fprintf( stderr, 
	     "bdt: nchannels: %d npre:%d npost:%d\n", S_nchannels, npre, npost );

    if ( npre != 0 && validate( npre, !START_AT_ZERO ) == 0 ){
	fprintf( stderr, "validated %d samples with NO errors\n", npre );
    }
    if ( npost != 0 && validate( npost, START_AT_ZERO ) == 0 ){
	fprintf( stderr, "validated %d samples with NO errors\n", npost );
    }
    return 0;
}












