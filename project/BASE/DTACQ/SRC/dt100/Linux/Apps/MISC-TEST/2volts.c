/*****************************************************************************
 *
 * File: 2volts.c
 *
 * $RCSfile: 2volts.c,v $
 * 
 * Copyright (C) 2001 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Descriptio
  * $Id: 2volts.c,v 1.1 2001/09/07 12:11:29 pgm Exp $
 * $Log: 2volts.c,v $
 * Revision 1.1  2001/09/07 12:11:29  pgm
 * 2volts, with dos mode opt
 *
 * Revision 1.2  2000/12/28 11:03:00  pgm
 * *** empty log message ***
 *
 * Revision 1.1  1999/10/22 16:26:49  pgm
 * first entry to cvs
 *
 *
\*****************************************************************************/



#include "local.h"
#include "acq32ioctl.h"
#include "acq32busprot.h"
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <time.h>

#include <assert.h>

#include <popt.h>



#define HELP \
"2volts [opts] [nchannels]\n"\
"print incoming binary data as volts\n"\
"-t acq32 acq16\n"\
"-d dos create dos mode file\n"\
""
static int S_verbose;

double convertACQ32( short data )
{
    float volts = data * 10.0f / 32768;
    
    return volts;
}

double convertACQ16( short data )
{
    float volts = data * 25.f /32768;
    
    return volts;
}

int main( int argc, const char* argv[] )
{

    char* btype = "acq32";
    double (*convertor)( short ) = convertACQ32;
    short a_sample;
    int rc;
    int dos = 0;
    
    struct poptOption opt_table[] = {
        { "board-type", 't', POPT_ARG_STRING,  &btype, 't'  },
        { "help",       'h', POPT_ARG_NONE,         0, 'h'  },
	{ "verbose",    'v', POPT_ARG_INT,    &S_verbose, 0 },
	{ "dos",        'd', POPT_ARG_INT,     &dos, 0 },
        { }
    };

    poptContext opt_context;

    opt_context = poptGetContext( argv[0], argc, argv, opt_table, 0 );

    while ( (rc = poptGetNextOpt( opt_context )) > 0 ){
        switch( rc ){
	    case 't':
	        if ( STREQ( btype, "acq16" ) ){
		        convertor = convertACQ16;
			}
			break;
        case 'h':
            fprintf( stderr, HELP );
            return 1;
        }
    }  // processes all other opts via arg pointers

    while ( fread( &a_sample, sizeof(a_sample), 1, stdin ) == 1 ){
        printf( "%6.4f%s", convertor( a_sample ), dos? "\n\r": "\n" );
    }
    return 0;
}

