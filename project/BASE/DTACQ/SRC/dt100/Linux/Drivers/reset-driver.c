/*
 * reset-driver.c - emergency driver reset in case of crash
 */

#undef __KERNEL__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "acq32ioctl.h"

#include <popt.h>

#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <fcntl.h>


int main( int argc, const char* argv[] )
{
    char *fname  = "/dev/acq32/acq32.1.m1";
    int read_all = 0;
    int fd;

    struct poptOption opt_table[] = {
        { "all", 'a', POPT_ARG_NONE, &read_all, 0 },
        { }
    };

    poptContext opt_context;

    opt_context = poptGetContext( argv[0], argc, argv, opt_table, 0 );

    poptGetNextOpt( opt_context );  // processes all opts via arg pointers

    if ( (fd = open( fname, O_RDWR )) < 0 ){
        fprintf( stderr, 
                 "reset-driver: failed to open device \"%s\" - ", fname );
        perror( "" );
        return 1;
    }

    if ( ioctl( fd, ACQ32_IOCHARDRESET, 0 ) != 0 ){
        perror( "reset-driver: failed ioctl" );
        return 1;
    }
    
    return 0;
}



