/*****************************************************************************
 *
 * File: acq32dump.c
 *
 * $RCSfile: acq32dump.c,v $
 * 
 * Copyright (C) 1999 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description: dump selected data from acq32. shows use of seek command
 *
 * $Id: acq32dump.c,v 1.1 2001/01/02 12:28:53 pgm Exp $
 * $Log: acq32dump.c,v $
 * Revision 1.1  2001/01/02 12:28:53  pgm
 * *** empty log message ***
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

#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <popt.h>

#include "acq32ioctl.h"

#define WHENCE_OPTS \
    CC_SEEK_BEGIN " "  \
    CC_SEEK_END " " \
    CC_SEEK_CURRENT " " \
    CC_SEEK_TRIG 

#define HELP \
"acq32dump $Revision: 1.1 $\n"\
"usage: acq32dump [opts] channel-def [first [last]]\n"\
"-w whence { " WHENCE_OPTS " \n"\
""

char* makeDevName( int board, const char* dev ) {
    static char dev_name[80];

    sprintf( dev_name, "/dev/acq32/acq32.%d.%s", board, dev );
    return dev_name;
}
int main( int argc, char** argv )
{
  /* WORKTODO ... args handling */

    FILE* fd;
    const char* fname;
    int rc;
    int board = 1;
    char* whence = CC_SEEK_BEGIN;
    const char* channel_def_arg;
    const char* first_arg;
    const char* last_arg;
    int first = 0;
    int last = 0x7fffffff;
    char* buffer;
    int unit_size;
    int nread;

    struct poptOption opt_table[] = {
        { "help",   'h', POPT_ARG_NONE,         0, 'h' },
	{ "board",  'b', POPT_ARG_INT,    &board, 0 },
	{ "whence", 'w', POPT_ARG_STRING,  &whence, 0 },
        { }
    };

    poptContext opt_context;

    opt_context = poptGetContext( argv[0], argc, argv, opt_table, 0 );

    while ( (rc = poptGetNextOpt( opt_context )) > 0 ){
        switch( rc ){
        case 'h':
            fprintf( stderr, HELP );
            return 1;
        }
    }  // processes all other opts via arg pointers

    
    if ( (channel_def_arg = poptGetArg( opt_context )) ){
	unit_size = (STREQ( channel_def_arg, "XX" )? 32: 1)*2;
    }else{
	fprintf( stderr, "channel-def?\n" HELP );
	exit( 1 );
    }
    if ( (first_arg = poptGetArg( opt_context )) ){
	first = strtol( first_arg, 0, 0 );
	
	if ( (last_arg = poptGetArg( opt_context )) ){
	    last = strtol( last_arg, 0, 0 );
	}
    }

    buffer = malloc( 0x10000*unit_size );
	 
    fname = makeDevName( board, channel_def_arg );

    if ( (fd = fopen( fname, "r+" )) == NULL ){
        fprintf( stderr, "diags: failed to open device \"%s\" - ", fname );
        perror( "" );
        return 1;
    }
    if ( fprintf( fd, "%s %s %d\n", CC_SEEK, whence, first ) <= 0 ){
	perror( "failed to write command" );
	exit errno;
    }
    fflush( fd );
    fprintf( stderr,  "%s %s %d\n", CC_SEEK, whence, first );
    
    while( (nread = fread( buffer, unit_size, 0x10000, fd )) ){
	fwrite( buffer, unit_size, nread, stdout );
    }
    return 0;
}





