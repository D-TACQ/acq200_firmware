/*****************************************************************************
 *
 * File: dt100test.c -
 *
 * $RCSfile: dt100test.c,v $
 *
 * Copyright (C) 1999 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description: library module implements dt100api
 *
 * $Id: dt100test.c,v 1.3 1999/11/20 21:20:30 pgm Exp $
 * $Log: dt100test.c,v $
 * Revision 1.3  1999/11/20 21:20:30  pgm
 * 991120 time for bed
 *
 * Revision 1.2  1999/10/25 10:40:51  pgm
 * add other modes handling
 *
 *

 * Test Coverage:

1:int dt100_GetNumChannels()  
1:enum STATE dt100_GetState()
0:int dt100_SetChannelMask( short* channels, int maxchannels )
0:int dt100_GetMemoryDepth( int channel )
1:int dt100_SetMode( enum MODE mode, int samples )
1:int dt100_SetArmed()
1:int dt100_SetAbort()
1:int dt100_GetNumSamples()
1:int dt100_GetChannelData()


\*****************************************************************************/


#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "local.h"            // assert 
#include "dt100api.h"         // the API

#include "acq32ioctl.h"       // just for MC_ strings

#include <popt.h>

int G_verbose = 1;

int G_debug;

#define CAPLEN 0x10000
#define NINC   1

#define VPRINTRET( fn, ret ) \
    if ( G_verbose ) printf( #fn " returns %d\n", ret )

#define VPRINTF( fmt, args... ) \
    if ( G_verbose ) printf( fmt, ##args )

static enum MODE getMode( char* key )
{
    if ( STREQ( key, "GATED_TRANSIENT" ) ){
        return M_GATED_TRANSIENT;
    }else if ( STREQ( key, "GATED_CONTINUOUS" ) ){
        return M_GATED_CONTINUOUS;
    }else if ( STREQ( key, "SOFT_TRANSIENT" ) ){
        return M_SOFT_TRANSIENT;
    }else{
        fprintf( stderr, "ERROR: bad mode %s\n", key );
        exit( 1 );
    }
}

static char* getModeStr( enum MODE mode )
{
    switch( mode ){
    case M_GATED_TRANSIENT:
        return "M_GATED_TRANSIENT";
    case M_GATED_CONTINUOUS:
        return "M_GATED_CONTINUOUS";
    default:
        return "M_SOFT_TRANSIENT";
    }
}

#define HELP \
"dt100test [opts] test1 [test2...]\n"\
"tests:\n"\
"   capture\n"\
"   fetch [channel]\n"\
"\n"\
"opts:\n"\
"   --ninc <ninc>         : increment value for fetch [1]\n"\
"   --caplen <caplen>     : capture length [0x4000]\n"\
"   --n0                  : first sample to fetch\n"\
"   --mode                : mode\n"\
"   --fetchlen <fetchlen> : nsamples to fetch\n"\
"   --mask <hex-mask>     : set channel mask [0xffffffff]\n"\
""

struct Prams {
    enum MODE mode;
    int caplen;
    unsigned mask;

    int n0;
    int ninc;    
    int fetchlen;
    int fetch_channel;
};

static void setChannelMask( unsigned mask )
{
    short channels[32];
    unsigned imask;
    unsigned bit;
    int ok;

    for ( imask = 0, bit = 0x1; imask != 32; ++imask, bit <<= 1 ){
      channels[imask] = bit&mask? 1: 0;
    }

    ok = dt100_SetChannelMask( channels, 32 );

    VPRINTRET( dt100_SetChannelMask(), ok );
    ASSERT( ok == 0 ); 
}

static void capture( struct Prams* prams )
{
    int numchannels, numsamples;
    enum STATE state;
    int ok;
    /*
     * do an example capture
     */

    setChannelMask( prams->mask );

    ok = dt100_SetMode( prams->mode, prams->caplen );

    VPRINTF( "dt100_SetMode( %s, %d )\n", 
                   getModeStr( prams->mode ), prams->caplen );    
    VPRINTRET(  CAPLEN set, prams->caplen );
    VPRINTRET(  dt100_SetMode(), ok );

    
    ASSERT( ok == 0 );

    

    ok = dt100_SetArmed();

    VPRINTRET( dt100_SetArmed(), ok );
    ASSERT( ok == 0 );

    do {
        state = dt100_GetState();

        VPRINTRET( dt100_GetState(), state );

        sleep( 1 );
    }
        while ( state != ST_STOP );

    numsamples = dt100_GetNumSamples();

    VPRINTRET( dt100_GetNumSamples(), numsamples );
    ASSERT( numsamples > 0 );

    numchannels = dt100_GetNumChannels();
    VPRINTRET( dt100_GetNumChannels(), numchannels );
    ASSERT( numchannels > 0 );
}


static void fetch( struct Prams* prams )
{
    FILE* fp;
    char outfile[80];
    int nsamples;
    short* buf = malloc( sizeof(short)*prams->fetchlen );

    ASSERT( buf );

    nsamples = dt100_GetChannelData( 
                   buf, 
                   prams->fetch_channel, 
                   prams->n0, 
                   prams->ninc, 
                   prams->fetchlen );

    VPRINTF( "dt100_GetChannelData( buf, %d, %d, %d, %d ) returns %d\n",
                    prams->fetch_channel,
                    prams->n0,
                    prams->ninc,
                    prams->fetchlen,
                    nsamples              );
    sprintf( outfile, "/tmp/acq32/dt100test.%02d", prams->fetch_channel+1 );

    fp = fopen( outfile, "w" );
    ASSERT( fp );

    fwrite( buf, sizeof(short), nsamples, fp );
    fclose( fp );

    printf( "data written to file %s\n", outfile );

    free( buf );
}

int main( int argc, char* argv[] )
{ 
    int dunnit = FALSE;

    struct Prams prams = {
        M_SOFT_TRANSIENT, CAPLEN, 0xffffffff, 0, 1, CAPLEN, 0
    };
    char* mbuf;

    char *cmd;
    char *opt;

    struct poptOption opt_table[] = {
        { "debug",  'd', POPT_ARG_INT,    &G_debug,    0 },
        { "ninc",     0, POPT_ARG_INT,    &prams.ninc, 0 },
        { "caplen",   0, POPT_ARG_INT,    &prams.caplen, 0 },
        { "n0",       0, POPT_ARG_INT,    &prams.n0,     0 },
        { "mode",     0, POPT_ARG_STRING, &mbuf, 'm' },
        { "fetchlen", 0, POPT_ARG_INT,    &prams.fetchlen, 0 },
        { "mask",     0, POPT_ARG_INT,    &prams.mask, 0 },
        { "help",   'h', POPT_ARG_NONE,   0, 'h' },
        { }
    };

    poptContext opt_context =
        poptGetContext( argv[0], argc, argv, opt_table, 0 );
    int rc;

    printf( "dt100test: exercise dt100 api $Revision: 1.3 $\n" );

    while ( (rc = poptGetNextOpt( opt_context )) != -1 ){  
        switch( rc ){
        case 'm':
            prams.mode = getMode( mbuf );
            break;
        case 'h':
            fprintf( stderr, HELP );
            return 0;
        }
    }


    while ( (cmd = poptGetArg( opt_context )) != NULL ){
        if ( STREQNL( cmd, "capture" ) ){
            capture( &prams );
	}else if ( STREQNL( cmd, "fetch" ) ){
            while ( (opt = poptPeekArg( opt_context )) && isdigit( opt[0] )){
                prams.fetch_channel = atoi( poptGetArg( opt_context ) );
                fetch( &prams );
                dunnit = TRUE;
            }
            if ( !dunnit ){
                fetch( &prams );
            }
        }else{
            fprintf( stderr, "sorry, command %s not understood\n", cmd );
            return 1;
        }
        dunnit = TRUE;
    }

    if ( !dunnit ){
        capture( &prams );
        fetch( &prams );
    }
    return 0;
}



