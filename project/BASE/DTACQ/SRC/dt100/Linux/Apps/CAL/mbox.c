/*
 * mbox.c - linux app to peek/poke mailboxes
 */

#undef __KERNEL__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "local.h"
#include "acq32ioctl.h"

#include <popt.h>

#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <fcntl.h>

#include <unistd.h>

#define POLL_TIMEOUT 10000 // timeout in usecs

int G_debug = 1;

#define PRINTF if( G_debug ) printf
#define PRINTFL(n) if( (n)<G_debug ) printf

static unsigned read_mbox( int fd, int mbox )
{
    unsigned value = 37;

    int error = ioctl( fd, ACQ32_IOGMBX( mbox ), &value );

    ASSERT( error==0 );

    PRINTF( "mbox:%d == 0x%08x\n", mbox, value );
    return value;
}

static void poll_mbox_bits( 
    int fd, 
    int mbox, 
    unsigned bits_wanted_hi, unsigned bits_wanted_lo )
{
    volatile unsigned value;
    unsigned pollcat = 0;

    do {
        int error ;

        usleep( POLL_TIMEOUT );

        error = ioctl( fd, ACQ32_IOGMBX( mbox ), &value );
        
        ASSERT( error==0 );

        if ( (++pollcat&0xffff) == 0 ){
            printf( "pollcat %d=0x%08x\n", mbox, value );
	}
    }
        while( (value&bits_wanted_hi) != bits_wanted_hi ||
               (value&bits_wanted_lo) != 0                 );
}

static void write_mbox( int fd, int mbox, unsigned value )
{
    int error;

    PRINTF( "mbox:%d := 0x%08x\n", mbox, value );

    error = ioctl( fd, ACQ32_IOSMBX( mbox ), value );

    ASSERT( error==0 );
}


#define HELP \
"mbox $Revision: 1.8 $\n"\
"mbox [opts] [mboxn[=+-Tvalue ...]\n"\
" = assign\n"\
" + poll for rising bits\n"\
" - poll for falling bits\n"\
" T test using value as mask\n"\
"opts:\n"\
"    -f device [default /dev/acq32/acq32.1.raw]\n"\
"    -b board  1..4 default: 1\n"\
"    -a all    read all mailboxes\n"



static void do_bittest( int fd, int mbox, unsigned mask )
{
  unsigned xx, yy;
  
  for ( xx = 0; ; ++xx ){
    xx &= mask;
    write_mbox( fd, mbox, xx );
    if ( xx != (yy = read_mbox( fd, mbox )) ){
      fprintf( stderr, "ERROR: wanted 0x%08x got 0x%08x\n", xx, yy );
    }
  }
}

int main( int argc, const char* argv[] )
{
    char fname_buf[80];
    int board;
    char *fname  = "/dev/acq32/acq32.1.raw";
    int read_all = 0;
    const char* cmd;
    int fd;

    struct poptOption opt_table[] = {
        { "device", 'f', POPT_ARG_STRING, &fname, 0 },
        { "board",  'b', POPT_ARG_INT,  &board,  'b' },
        { "all",    'a', POPT_ARG_INT, &read_all, 0 },
        { "debug",  'd', POPT_ARG_INT,  &G_debug, 0 },
        { "help",   'h', POPT_ARG_NONE,         0, 'h' },
        { }
    };

    poptContext opt_context = 
        poptGetContext( argv[0], argc, argv, opt_table, 0 );

    int rc;

    while ((rc = poptGetNextOpt( opt_context )) != -1 ){
        switch( rc ){
        case 'b':
            sprintf( fname_buf, "/dev/acq32/acq32.%d.raw", board );
            fname = fname_buf;
            break;
        case 'h':
            fprintf( stderr, HELP );
            return 1;
	}
    }  // processes all other opts via arg pointers

    PRINTFL(2)( "mbox: open %s\n", fname );

    if ( (fd = open( fname, O_RDWR )) < 0 ){
        fprintf( stderr, "mbox: failed to open device \"%s\" - ", fname );
        perror( "" );
        return 1;
    }

    PRINTFL(3)( "device is open ...\n" );

    if ( read_all ){
        read_mbox( fd, 0 );
        read_mbox( fd, 1 );
        read_mbox( fd, 2 );
        read_mbox( fd, 3 );
    }else{
        while ( (cmd = poptGetArg( opt_context )) != NULL ){
            int mbox = atoi( cmd );

            if ( !IN_RANGE( mbox, 0, 3 ) ){
                fprintf( stderr, "mbox: please enter mbox in range 0..3\n" );
                exit( 1 );
            }

            if ( strchr( cmd, '+' ) ){
	      /*
               * poll for bits going hi
               */

               unsigned hibits = strtoul( strchr( cmd, '+' ) + 1, NULL, 0 );

               poll_mbox_bits( fd, mbox, hibits, 0 );
            }else if ( strchr( cmd, '-' ) ){

	      /* poll for bits going lo */

               unsigned lobits = strtoul( strchr( cmd, '-' ) + 1, NULL, 0 );

               poll_mbox_bits( fd, mbox, 0, lobits );
            }else if ( strchr( cmd, '=' ) ){
                unsigned value = strtoul( strchr( cmd, '=' ) + 1, NULL, 0 );

                write_mbox( fd, mbox, value );
	    }else if ( strchr( cmd, 'T' ) ){
	        unsigned mask = strtoul( strchr( cmd, 'T' )+1, NULL, 0 );
		
		if ( mask == 0 ) mask = 0xffffffff;

		do_bittest( fd, mbox, mask );
            }else{
                read_mbox( fd, mbox );
            }                  
        }
    }

    return 0;
}
