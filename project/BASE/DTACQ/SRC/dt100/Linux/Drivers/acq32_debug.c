/*****************************************************************************
 *
 * File: acq32_debug.c - app to set acq32-drv debug level
 *
 * $RCSfile: acq32_debug.c,v $
 * 
 * Copyright (C) 1999 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: acq32_debug.c,v 1.4 2002/07/19 18:39:37 pgm Exp $
 * $Log: acq32_debug.c,v $
 * Revision 1.4  2002/07/19 18:39:37  pgm
 * mod DUMDMA to avoid message overrun
 *
 * Revision 1.3  2002/03/12 12:12:40  pgm
 * clean format rools OK
 *
 * Revision 1.2  2002/02/27 18:41:33  pgm
 * add alternate device to acq32_debug.c
 *
 * Revision 1.1  2000/10/01 08:27:13  pgm
 * post install debug changer
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>




#include "acq32ioctl.h"

#include "acq32ioctl.h"

#define DEVICE "/dev/acq32/acq32.1.raw"
#define DEVICE2 "/dev/acq32/acq32.0.raw"

int main( int argc, char* argv[] )
{
    int debug_level = argc>1? atoi( argv[1] ): 0;
    int fd = open( DEVICE, O_RDONLY );

    if ( !fd ){
        fd = open( DEVICE2, O_RDONLY );
        if ( !fd ){
            perror( "ERROR: failed to open " DEVICE  " or " DEVICE2);
            exit( 1 );
        }
    }

    if ( ioctl( fd, ACQ32_IOSDEBUG, debug_level ) != 0 ){
	perror( "ERROR ioctl failed" );
	exit(1);
    }
    return 0;
}


