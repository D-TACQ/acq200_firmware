/*****************************************************************************
 *
 * File: irq.c - app to set global interrupt mask, via acq32-drv as convenience
 *
 * usage irq { enable | disable } mask
 *
 * $RCSfile: irq.c,v $
 * 
 * Copyright (C) 1999 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: irq.c,v 1.1 2002/07/28 17:27:19 pgm Exp $
 * $Log: irq.c,v $
 * Revision 1.1  2002/07/28 17:27:19  pgm
 * *** empty log message ***
 *
 */

#include "local.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>


#define HELPSTR "irq {enable | disable } mask\n"
#define HELP    fprintf( stderr, HELPSTR )


#include "acq32ioctl.h"

#include "acq32ioctl.h"

#define DEVICE "/dev/acq32/acq32.1.raw"
#define DEVICE2 "/dev/acq32/acq32.0.raw"

int main( int argc, char* argv[] )
{
    unsigned fxn;
    unsigned mask;
    int fd;

    if ( argc == 3 ){
	if ( STREQ( argv[1], "enable" ) ){
	    fxn = ACQ32_IOS_INTS_ENABLE;
	}else if ( STREQ( argv[1], "disable" ) ){
	    fxn = ACQ32_IOS_INTS_DISABLE;
	}else if ( STREQ( argv[1], "swallow" ) ){
	    fxn = ACQ32_IOS_SWALLOW_KMEM;
	}else{
	    HELP;
	    return 1;
	}

	mask = strtoul( argv[2], 0, 0 );
    }else{
	HELP;
	return 1;
    }

    fd = open( DEVICE, O_RDONLY );

    if ( !fd ){
        fd = open( DEVICE2, O_RDONLY );
        if ( !fd ){
            perror( "ERROR: failed to open " DEVICE  " or " DEVICE2);
            exit( 1 );
        }
    }

    if ( ioctl( fd, fxn, mask ) != 0 ){
	perror( "ERROR ioctl failed" );
	exit(1);
    }
    return 0;
}


