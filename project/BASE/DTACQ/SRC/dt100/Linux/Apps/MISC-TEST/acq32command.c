/*****************************************************************************
 *
 * File: acq32command.c
 *
 * $RCSfile: acq32command.c,v $
 * 
 * Copyright (C) 1999 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: acq32command.c,v 1.2 2002/06/13 20:09:00 pgm Exp $
 * $Log: acq32command.c,v $
 * Revision 1.2  2002/06/13 20:09:00  pgm
 * fix redefined message
 *
 * Revision 1.1.1.1  2001/04/01 12:27:06  pgm
 *
 *
 *
\*****************************************************************************/

#undef __KERNEL__

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

#define FNAME  "/dev/acq32/acq32.1.01"

#define HELP \
"acq32command $\n"\
"send commands using i2o\n"\
"usage: acq32command .... | acq32command [opts] pre [post]\n"\
"-v <n>         : set  verbosity\n"\
"-f fname\n"\
""


int S_verbose;



int main( int argc, const char* argv[] )
{
    static Message message = {  { 0, 128, 37 } };
    int itest;
    int rc;
    int fd;
    char* fname = FNAME;
    
    struct poptOption opt_table[] = {
        { "device",     'f', POPT_ARG_STRING,  &fname, 0   },
        { "help",       'h', POPT_ARG_NONE,         0, 'h' },
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
        }
    }  // processes all other opts via arg pointers

    if ( (fd = open( fname, O_RDWR )) < 0 ){
        fprintf( stderr, "mbox: failed to open device \"%s\" - ", fname );
        perror( "" );
        return 1;
    }

    for ( itest = 0; ; ++itest ) {
        time_t t_now = time( 0 );
	    int error;
	    
        message.header.id = itest;
	    message.header.type = MAKE_MT_COMMAND(MTC_TEST);
	    message.header.length =
    	    sprintf( 
	            message.payload.cdata, 
		        "acq32command message %04d sent at %s\n",
	            itest, 
		        asctime( localtime( &t_now ) ) 
			);
		    
        if ( S_verbose ){
	        fprintf( stderr, "Send %s\n", message.payload.cdata );
		}
		error = ioctl( fd, ACQ32_IOSENDI2O, &message );
		if ( error != 0 ){
		    if ( S_verbose ) {
		        fprintf( stderr," no buffers, sleep and retry\n" );
			}
			sleep( 1 );
			error = ioctl( fd, ACQ32_IOSENDI2O, &message );
        }
		
		if ( error != 0 ){
		    fprintf( stderr, "ERROR from ioctl %d\n", error );
		    return error;
		}
		if ( S_verbose > 1 ) {
		    fprintf( stderr, "hit any key to continue\n" );
		    getchar();
		}
    }
    return 0;
}
