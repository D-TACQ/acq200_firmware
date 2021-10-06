/*****************************************************************************
 *
 * File: dt100api.c -
 *
 * $RCSfile: dt100api.c,v $
 *
 * Copyright (C) 1999 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description: library module implements dt100api
 *
 * $Id: dt100api.c,v 1.4 2000/09/26 19:46:52 pgm Exp $
 * $Log: dt100api.c,v $
 * Revision 1.4  2000/09/26 19:46:52  pgm
 * GATED_CONTINUOUS pre/post, lseek, spin
 *
 * Revision 1.3  1999/11/20 21:20:30  pgm
 * 991120 time for bed
 *
 * Revision 1.2  1999/10/26 14:50:51  pgm
 * fflush removes need for over-conservative open/close policy
 *
 * Revision 1.1  1999/10/16 06:59:39  pgm
 * APi and tester : V1
 *
 *
\*****************************************************************************/

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>              // getenv()
#include <errno.h>

#include "local.h"
#include "dt100api.h"

#include "acq32ioctl.h"

#define _DEVROOT  "/dev/acq32/"
#define _NAMEROOT "acq32"
#define _MASTER   ".1.m1"             // WORKTODO .. this can vary

#define DEV    _DEVROOT _NAMEROOT
#define MASTER _DEVROOT _NAMEROOT _MASTER

#define NL      "\n"

#define NDEBUG      // switch off internal status reports

extern int G_debug;

#define PRINTF if ( G_debug ) printf

static const char* getMaster()
{
   if ( getenv( "DT100_MASTER" ) ){
       return getenv( "DT100_MASTER" );
   }else{
       return MASTER;
   }
}
static FILE* FD_MasterW()
{

    static FILE* fd;

    if ( !fd ){
        fd = fopen( getMaster(), "w" );
        if ( !fd ){
            char buf[80];
   
            sprintf( buf, "ERROR opening %s", getMaster() );
            perror( buf );
            exit( errno );
	}
    }

    return fd;
}
static FILE* FD_MasterR()
{

    static FILE* fd;

    if ( !fd ){
        fd = fopen( getMaster(), "r" );
        if ( !fd ){
            char buf[80];
   
            sprintf( buf, "ERROR opening %s", getMaster() );
            perror( buf );
            exit( errno );
	}
        fd->_flags |= _IO_UNBUFFERED;
    }

    return fd;
}

#define ACQ32_MAX_CHANNELS 32

#define BOARD( channel )    ((channel)/ACQ32_MAX_CHANNELS +1)
#define PCHAN( channel )    ((channel)%ACQ32_MAX_CHANNELS +1)

static FILE* FD_Channel( int ichannel )
{
    static int _ichannel = -1;
    static FILE* fd;

    if ( _ichannel == ichannel ){
        ASSERT( fd );
    }else{
        char fname[80];

        if ( fd ){
            fclose( fd );
        }

        sprintf( fname, "%s.%d.%02d", 
                 DEV, BOARD(ichannel), PCHAN(ichannel) );

        fd = fopen( fname, "r" );

        if ( !fd ){
             char buf[80];

             sprintf( buf, "ERROR opening %s", fname );
             perror( buf );
             exit( errno );
        }else{
             _ichannel = ichannel;
        }

        PRINTF( "using device %s\n", fname );
    }

    return fd;
}


#define NORESPONSE NULL
#define FMTNL      "%s\n" 


int writeMaster( char* response, int maxresponse, char* fmt, ...  )
// return negative on fail, positive if response, 0 if no response
{
     va_list args;
     char lresponse[80];

     va_start( args, fmt );
#ifndef NDEBUG
     printf( "this is the command ... %s\n", fmt );
     vprintf( fmt, args );
#endif
     vfprintf( FD_MasterW(), fmt, args );
     va_end( args );

     fflush( FD_MasterW() );    // needed to force write before read!!

     strcpy( lresponse, "" );

     fgets( lresponse, sizeof(lresponse)-1, FD_MasterR() );

     if ( strlen( lresponse ) > 0 ){
#ifndef NDEBUG
         fprintf( stderr, "response:%s\n", lresponse );
#endif
         if ( strncmp( lresponse, ACQ32_ACK, strlen( ACQ32_ACK ) ) == 0 ){
             char* prams = lresponse+strlen(ACQ32_ACK);

             if ( *prams != '\0' && *prams != '\n' ){
                 strncpy( response, prams, maxresponse );
                 if ( strlen( prams ) > maxresponse && maxresponse ){
                    prams[maxresponse-1] ='\0';
                    fprintf( stderr, "WARNING: discarding \"%s\"\n",
                                        &prams[maxresponse]            );
                 }
                 return strlen( response );
             }else{
                 return 0;
             }
	 }else{
             fprintf( stderr, "ERROR: unexpected reply \"%s\", bailing out\n",
                 lresponse );
             return -1;
         }
     }else{

        printf ( "reponse %s\n", lresponse );
	perror( "ERROR: lost contact with MASTER" );
	return -1;
     }
}

int failedDecode( const char* fn, const char* response )
{
    fprintf( stderr, "ERROR:%s failed to decode \"%s\"\n", fn, response );
    exit( 1 );
    return -1;
}

int dt100_GetNumChannels()
// returns number of channels available in rack {32,64,96}
{
    char response[80];

    int numchannels;
    int nresponse = writeMaster( response, 80, MC_GET_NUMCHANNELS NL ); 

    ASSERT( nresponse > 0 );

    if ( sscanf( response, MC_GET_NUMCHANNELS "=%d", &numchannels ) == 1 ){
        return numchannels;
    }else{
        return failedDecode( "dt100_GetNumChannels()", response );
    }
}




enum STATE dt100_GetState()
// returns current state
{
    char response[80];

    int state;
    int nresponse = writeMaster( response, 80, MC_GET_STATE NL ); 

    ASSERT( nresponse > 0 );

    if ( sscanf( response, "%d", &state ) == 1 ){
        return state;
    }else{
        return failedDecode( "dt100_GetState()", response );
    }    
}

int dt100_SetChannelMask( short* channels, int maxchannels )
// <channels>: a user supplied array indicating channels to use
// <channels>[0] == 1 means "enable channel 0
// <channels>[0] == 0 means "disable channel 0
// <maxchannels>: number of elements { 32, 64, 96 }
// returns 0 or negative error code
{
    char chandef[33];
    int ichan;

    memset( chandef, '0', 32 );
    chandef[32] = '\0';

    for ( ichan = 0; ichan != MIN( 32, maxchannels ); ++ichan ){
      chandef[ichan] = channels[ichan]? '1': '0';
    }
    
    return writeMaster( 
               NORESPONSE, 0, MC_SET_CHANNEL_MASK " %s" NL, chandef );
}


int dt100_GetMemoryDepth( int channel )
// <channel> selects channel in range { 0..GetNumChannels()-1}
// returns depth of memory in samples for <channel>,
// Function of available memory depth on the board where
// <channel> is located and the current channel mask
// Eg - first board has 64MB memory:
// <channel>,    enabled channels,    dt100_GetMemoryDepth (0)
// 0         32, 0 is enabled,		  1M
// 0		  8, 0 is enabled, 	  4M
// 0         32, 0 NOT enabled,       0
{
    char response[80];

    int numchannels;
    int nresponse = writeMaster( response, 80, MC_GET_MEMORY_DEPTH NL ); 

    ASSERT( nresponse > 0 );

    if ( sscanf( response, MC_GET_MEMORY_DEPTH "=%d", &numchannels ) == 1 ){
        return numchannels;
    }else{
        return failedDecode( "dt100_GetNumChannels()", response );
    }
}


int dt100_SetMode( enum MODE mode, int samples )
// <mode>               <samples>
// M_GATED_TRANSIENT  - NUM-SAMPLES
// M_GATED_CONTINUOUS - POST-SAMPLES
// M_SOFT_TRANSIENT   - NUM-SAMPLES { 0=>fill memory }
{
    const char* smode;

    switch( mode ){
    case M_GATED_TRANSIENT:
        smode = MC_MODE_GATED_TRANSIENT;
        break;
    case M_GATED_CONTINUOUS:
        smode = MC_MODE_GATED_CONTINUOUS;
        break;
    case M_SOFT_TRANSIENT:
        smode = MC_MODE_SOFT_TRANSIENT;
        break;
    default:
        ASSERT( 0 );
    }
        

    return writeMaster( NORESPONSE, 0, 
                        "%s %s %d" NL, MC_SET_MODE, smode, samples ); 
}



int dt100_SetArmed()	// request transition to ST_ARM
{
    return writeMaster( NORESPONSE, 0, FMTNL, MC_SET_ARM );
}

int dt100_SetAbort()	// request transition to ST_STOP
{
    return writeMaster( NORESPONSE, 0, FMTNL, MC_SET_ABORT );
}

int dt100_GetNumSamples()
// returns number of samples available
// this is the number of clocks that have been captured.
// the total amount of data available is 
// GetNumSamples * enabled channels
{
    char response[80];

    int numsamples;
    int nresponse = writeMaster( response, 80, MC_GET_NUMSAMPLES NL ); 

    ASSERT( nresponse > 0 );

    if ( sscanf( response, MC_GET_NUMSAMPLES "=%d", &numsamples ) == 1 ){
        return numsamples;
    }else{
        return failedDecode( "dt100_GetNumSamples()", response );
    }
}

int dt100_GetChannelData(
    unsigned short* buf,	// caller's buffer
    int chan,      		// channel number 
    int n0,			// the first sample to transfer
    int ninc,			// increment (stride)
    int nmax			// the max # samples to copy
)
// returns # transferred samples or negative error
// in GATED_CONTINUOUS mode, 
// n0 == 0 represents the point of trigger (GATE going OFF)
// examples
//  <n0>, <ninc>,	 <nmax>
//     0,      1,  100   get first 100 samples from trigger
//  -100,      1,  200   get 200 samples spanning trigger
//  -100,      1,  100   get 100 samples before trigger
//     0,     -1,  100   get 100 samples before trigger, 
//                       in reverse order ??!!
{
    ASSERT( ninc >= 1 );  // WORKTODO

    if ( fseek( FD_Channel( chan ), n0, 0 /*SEEK_TRIG*/ ) == -1 ){
         fprintf( stderr, "fseek( FD(%d), %d, %d )\n", chan, n0, SEEK_TRIG );
         perror( "ERROR: seek failed" );
         exit( 1 );
    }

    if ( ninc == 1 ){
        return fread( buf, sizeof(short), nmax, FD_Channel(chan) );
    }else{
    
        unsigned short* bigbuf = malloc( nmax*ninc*sizeof(short) );
        int nsamples, isample;
        unsigned short* psrc = bigbuf;

        ASSERT( bigbuf );

        nsamples = fread( bigbuf, sizeof(short), nmax*ninc, FD_Channel(chan) );

        for ( isample = 0; isample != nmax; ++isample, psrc += ninc ){
            buf[isample] = *psrc;
        }

        return isample;
    }
}












