/*****************************************************************************
 *
 * File: mmap.c
 *
 * $RCSfile: mmap.c,v $
 * 
 * Copyright (C) 1999 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: mmap.c,v 1.4 2001/10/07 10:01:42 pgm Exp $
 * $Log: mmap.c,v $
 * Revision 1.4  2001/10/07 10:01:42  pgm
 * *** empty log message ***
 *
 * Revision 1.3  2001/03/23 19:44:46  pgm
 * map only opt, debugs added
 *
 * Revision 1.2  2000/10/05 21:29:55  pgm
 * mmap can read,write,fill
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
#include <unistd.h>

#include <popt.h>

#define FNAME  "/dev/acq32/acq32.1.raw"

#define HELP \
"mmap $Revision: 1.4 $\n"\
"usage: mmap opts\n"\
"    -f device name\n"\
"    -r read [default]\n"\
"    -w write\n"\
"    -n nop (just block, holding the mapping)\n"\
"    -o offset\n"\
"    -l length\n"\
"maps device [ram] space and either\n"\
"    reads from ram to stdout\n"\
"-or-writes to ram from stdin\n"\
"   -b [-v value] = block fill [default 0xdeadbeef]\n"\
""


int main( int argc, char* argv[] )
{
  /* WORKTODO ... args handling */

    int fd;
    void* region;
    char* fname = FNAME;
    unsigned offset = 0;
    unsigned length = 0x100000;
    int rc;
    unsigned fill_value = 0xdeadbeef;
    enum MODE { M_READ, M_WRITE, M_FILL, M_NOP } mode = M_READ;

    struct poptOption opt_table[] = {
        { "device", 'f', POPT_ARG_STRING,  &fname, 0   },
        { "help",   'h', POPT_ARG_NONE,         0, 'h' },
        { "read",   'r', POPT_ARG_NONE,         0, 'r' },     
        { "write",  'w', POPT_ARG_NONE,         0, 'w' },
	{ "nop",    'n', POPT_ARG_NONE,         0, 'n' },
	{ "fill",   'b', POPT_ARG_NONE,         0, 'f' },
        { "offset", 'o', POPT_ARG_INT,    &offset, 'o' },
        { "length", 'l', POPT_ARG_INT,    &length, 'l' },
	{ "value",  'v', POPT_ARG_INT,    &fill_value, 0 },
        { }
    };

    poptContext opt_context;

    opt_context = poptGetContext( argv[0], argc, argv, opt_table, 0 );

    while ( (rc = poptGetNextOpt( opt_context )) > 0 ){
        switch( rc ){
        case 'h':
            fprintf( stderr, HELP );
            return 1;
        case 'r':
            mode = M_READ;
            break;
        case 'w':
            mode = M_WRITE;
            break;
	case 'n':
	    mode = M_NOP;
	    break;
	case 'f':
	  mode = M_FILL;
        }
    }  // processes all other opts via arg pointers


    if ( (fd = open( fname, O_RDWR )) < 0 ){
        fprintf( stderr, "mmap: failed to open device \"%s\" - ", fname );
        perror( "" );
        return 1;
    }

    region = mmap( NULL, length, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0 );

    if ( region == (caddr_t)-1 ){
        perror( "mmap" );
        return 1;
    }

    switch( mode ){
    default:
    case M_READ:

      // spew to stdout in one big blurt

        write( 1, (char*)region+offset, length );
	break;
    case M_FILL: {
      unsigned* praw = (unsigned*)&((char*)region)[offset];
      int iwrite;
      int imax = length/sizeof(unsigned);

      for ( iwrite = 0; iwrite != imax; ++iwrite ){
	praw[iwrite] = fill_value;
      }
      break;
    }
    case M_WRITE:{

      // write to memory - read input until first of EOF or length
      // it's slow char at a time stuff but who cares?

      int iwrite;
      int cc;
      char* praw = &((char*)region)[offset];

      for ( iwrite=0; (cc = getchar()) != -1 && iwrite != length; ++iwrite ){
         praw[iwrite] = cc;
      }
      break;
    }
    case M_NOP :
	fprintf( stderr, 
		 "mmap: blocking. Hit any key to continue, q to quit\n" );
	
	while ( getchar() != 'q' ){
	    char aline[128];
	    FILE* fp = fopen( "/proc/self/maps", "r" );
	    ASSERT( fp );
	    
	    while( fgets( aline, sizeof(aline), fp ) != NULL ) {
		fprintf( stderr, aline );
	    }
	    fclose( fp );
	}
	break;
   }

    return 0;
}






