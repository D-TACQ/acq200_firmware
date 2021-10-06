/*****************************************************************************
 *
 * File: acq32fetch.c
 *
 * $RCSfile: acq32fetch.c,v $
 * 
 * Copyright (C) 1999 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *     Fetches data from acq32 using the DUMDMA mechanism
 *
 * $Id: acq32fetch.c,v 1.17 2002/08/02 10:55:45 pgm Exp $
 * $Log: acq32fetch.c,v $
 * Revision 1.17  2002/08/02 10:55:45  pgm
 * more chunky diags
 *
 * Revision 1.16  2002/07/28 19:08:23  pgm
 * neaten up marker code
 *
 * Revision 1.15  2002/07/25 18:57:10  pgm
 * verb opts in chunk
 *
 * Revision 1.14  2002/07/25 15:52:34  pgm
 * add verbosity to fetchChunks()
 *
 * Revision 1.13  2002/07/24 07:20:33  pgm
 * chunked ioctl() for longer fetches
 *
 * Revision 1.12  2002/07/02 11:24:40  pgm
 * *** empty log message ***
 *
 * Revision 1.11  2002/01/06 12:08:12  pgm
 * ioctl retursn max samples available
 *
 * Revision 1.10  2001/11/23 22:49:48  pgm
 * -X crossection download demo works
 *
 * Revision 1.9  2001/10/14 09:34:01  pgm
 * cant use sqrt() on Alpha ??cvs diff acq32fetch.c
 *
 * Revision 1.8  2001/06/10 19:26:08  pgm
 * csv format stats
 *
 * Revision 1.7  2001/05/27 18:56:51  pgm
 * *** empty log message ***
 *
 * Revision 1.6  2001/05/06 18:47:21  pgm
 * cleanup
 *
 * Revision 1.5  2001/05/05 15:25:46  pgm
 * munmap() needed in loop, abort on ioctl() fail
 *
 * Revision 1.3  2001/04/09 13:03:34  pgm
 * comment
 *
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


/*
 * DUMDMA - easy as 1-2-3
 *
 * (1) - open() a path to channelized data device
 * (2) - mmap() gets a user mapped buffer
 * (3) - fetch user defined data using ioctl()
 */



#include <math.h>

#include "local.h"


#include <stdio.h>
#include <stdlib.h>

#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

#include <popt.h>

#include "acq32ioctl.h"



#define FNAME  "/dev/acq32/acq32.%d.%02d"
#define FNAMEX "/dev/acq32/acq32.%d.XX"

#define HELP \
"acq32fetch $Revision: 1.17 $\n"\
"acq32fetch [opts] channel[..channel2] [length] [start] [stride]\n"\
"defaults:\n"\
"    length:     all of it\n"\
"    start:      0\n"\
"    stride:     1\n"\
"opts:\n"\
"    -o [file]   output data (- is stdout)\n"\
"    -v verbose	 verbose output\n"\
"    -n          no fetch just map\n"\
"    -z          zero the data before fetch\n"\
"    -m          no munmap() just close()\n"\
"    -l [loopcount]\n" \
"    -M          check buffer markers\n"\
"    -S          compute some stats on the data\n"\
"    -X          cross section fetch (capture < 2MB only)\n"\
"    -H          open an hold the mmapping\n"\
"    -C chunk    specify chunksize\n"\
""

int verbose = 0;
int do_stats = 0;
int do_munmap = 1;
int do_Xsect = 0;
int do_zero = 0;
int do_markers = 0;
int chunk_size = 0;

void doStats( short* buffer, int nsamples, int ichannel, int stride )
/*
 * computer mean and std deviation in one pass
 * http://www.cs.duke.edu/education/courses/cps130/fall98/lectures/lect07/node13.html
 *
 *
 */
{
#define FMT_TITLE "%3s, %10s, %13s, %13s, %6s, %6s\n" 
#define FMT_STATS "%3d, %10d, %13.2f, %13.2f, %6d, %6d\n"
#define STAT_PRINT( fmt, data ) \
    fprintf( stderr, "%20s:" fmt "\n", #data, data )
 
    static int printed_title;
       
    double sum   = 0L;
    double sumsq = 0L; 
    int ibuffer;
    short xx;
    long lxx;
    double mean, variance, std_deviation;
    short xmax = -32768;
    short xmin = 32767;
    
    
    for ( ibuffer = 0; ibuffer != nsamples; ibuffer += stride ){
        lxx = xx = buffer[ibuffer];
        sum   += lxx;
	sumsq += lxx*lxx;
	    
	if ( xx > xmax ) xmax = xx;
	if ( xx < xmin ) xmin = xx;
    }
    
    mean = (double)sum/nsamples;
    
    variance = ((double)sumsq - nsamples*mean*mean)/(nsamples-1);
#if defined ( __alpha__ )
#warning "how do you use sqrt on Alpha"
    std_deviation = variance;
#else
    std_deviation = sqrt( variance );
#endif  
    if ( !printed_title ){
        fprintf( stderr, FMT_TITLE, "ch", "nsamples", "mean", "std_dev", "min", "max" );
	printed_title = 1;
    }
    fprintf( stderr, FMT_STATS, ichannel, nsamples, mean, std_deviation, xmin, xmax );
}

void doMarkers( struct READ_LOCALBUF_DESCR buffer_def )
{
    int ibuf;
    char* cbuf = (char*)buffer_def.buffer;
    int maxbuf = buffer_def.nsamples * 2;

    for ( ibuf = 0; ibuf < maxbuf; ibuf += 0x1000 ){
	if ( strncmp( &cbuf[ibuf], "PAGE", 4 ) == 0 ){
	    fprintf( stderr, "%s", &cbuf[ibuf]  );
	}
    }
}

static void sayWhen( int vl )
{
    if ( verbose > vl ){
	fprintf( stderr, "Say When\n" );
	getchar();
    }
}
static int fetchChunks( 
    int fd_in, 
    struct READ_LOCALBUF_DESCR buffer_def, 
    int channel                                // diags only
)
// length for each fetch is restricted.
// so do it in chunks
{
    struct READ_LOCALBUF_DESCR bd;
    int ichunk = 0;
    int nsamples_togo = buffer_def.nsamples;
    int totfetch = 0;
    int rc;

    memcpy( &bd, &buffer_def, sizeof(struct READ_LOCALBUF_DESCR) );

    if ( verbose ) fprintf( stderr, "\n" );

    for ( ; nsamples_togo > 0; ++ichunk ){
	bd.nsamples = MIN( nsamples_togo, chunk_size? chunk_size: READ_LOCALBUF_MAXCHUNK );

	if ( verbose > 1 ){
	    fprintf( stderr, "               fetchChunks buffer:%p start:0x%08x length:%d\n",
		     bd.buffer, bd.istart, bd.nsamples );
	    sayWhen( 2 );
	}
	rc = ioctl( fd_in, ACQ32_IOREAD_LOCALBUF, &bd );

	if ( rc < 0 ){
	    fprintf( stderr, "ERROR on chunk %d\n", ichunk );
	    return rc;
	}else{
	    totfetch += rc;
	}

	if ( verbose && do_stats ){
	    doStats( bd.buffer, bd.nsamples, 100+channel, 1 );
	}
	if ( nsamples_togo > bd.nsamples ){
	    bd.buffer += bd.nsamples;
	    bd.istart += bd.nsamples*bd.istride;
	}
	nsamples_togo -= bd.nsamples;
    }    

    return totfetch;
}

	
void acq32fetch( 
    int channel, 
    struct READ_LOCALBUF_DESCR buffer_def,
    int fetch,
    int fd_out,
    int hold
    )
// mmap and fetch data from channel, optionally write to fd_out, and clean up
{
    int zchan = channel - 1;
    int chnum = zchan%32 + 1;
    int board = zchan/32 + 1;
    int fd_in;
    char fname[80];
    void* region;
    int rc;	
    static int upload;
    int actual_samples = 0;
	
    sprintf( fname, FNAME, board, chnum );

    if ( verbose ){
	fprintf( stderr, "%4d using device %s\n", ++upload, fname );
	sayWhen( 2 );
    }
	
/*
 * (1)open() a path to the device
 */
    if ( (fd_in = open( fname, O_RDONLY )) < 0 ){
        fprintf( stderr, "mmap: failed to open device \"%s\" - ", fname );
        perror( "" );
        exit( 1 );
    }

/*
 * (2) mmap() a buffer
 */
 
    region = mmap( NULL, buffer_def.nsamples*2, PROT_READ, MAP_SHARED, fd_in, 0 );

    if ( region == (caddr_t*)-1 ){
        perror( "mmap" );
	rc = -1;
    }else{
        buffer_def.buffer = (short*)region;
    	rc = 0;
	
	if ( do_zero ){
	    memset( region, 0, buffer_def.nsamples*2 );
	}
    }

    if ( hold ){
	printf( "Holding the mmap <cr> to continue\n" );
	getchar();
    }
/*
 * (3) ioctl() - fetch the data
 */
    if ( rc == 0  ){
	if ( fetch ){
	    rc = fetchChunks( fd_in, buffer_def, channel );
    
	    if ( rc < 0 ){
		perror( "ioctl ACQ32_IOREAD_LOCALBUF failed" );
		exit( 1 );
	    }else{
		actual_samples = rc;
	    }
	}else{
	    actual_samples = buffer_def.nsamples;
	}
	
	if ( verbose > 1 ){
	    fprintf( stderr, "press <cr> to continue\n" );
	    getchar();
	}
	if ( do_markers ){
	    doMarkers( buffer_def );
	}
	if ( fd_out > 0 ){
	    write( fd_out, buffer_def.buffer, actual_samples*2 );
	}
	if ( do_stats ){
	    doStats( buffer_def.buffer, actual_samples, channel, 1 );
	}
    }

/*
 * (4) clean up 
 */

    if ( do_munmap ){
        if ( verbose > 2 ) {
            fprintf( stderr, "call munmap()\n" );
	    getchar();
	}
	
        rc = munmap( region, buffer_def.nsamples*2 );
        if ( rc != 0 ){
            perror( "ERROR:munmap() failed " );
	    exit( 1 );
	}
    }

    if ( verbose > 2 ) {
        fprintf( stderr, "call close()\n" );
	getchar();
    }
	
    rc = close( fd_in );
    if ( rc != 0 ){
        perror( "ERROR:close() failed " );
	exit( 1 );
    }
	
	
}

void acq32fetchX( 
    int channel,
    struct READ_LOCALBUF_DESCR buffer_def,
    int fetch,
    int fd_out,
    int loop
    )
// mmap and fetch data from channel, optionally write to fd_out, and clean up
{
    int zchan = channel - 1;
    int board = zchan/32 + 1;
    int fd_in;
    char fname[80];
    void* region;
    int rc;	
    static int upload;
    int iter;
    int actual_samples = 0;
	
    sprintf( fname, FNAMEX, board );

    if ( verbose ){
	fprintf( stderr, "%4d using device %s\n", ++upload, fname );
	if ( verbose > 2 ){
	    fprintf( stderr, "Say When\n" );
	    getchar();
	}
    }
	
/*
 * (1)open() a path to the device
 */
    if ( (fd_in = open( fname, O_RDONLY )) < 0 ){
        fprintf( stderr, "mmap: failed to open device \"%s\" - ", fname );
        perror( "" );
        exit( 1 );
    }

/*
 * (2) mmap() a buffer
 */
 
    region = mmap( NULL, buffer_def.nsamples*2, PROT_READ, MAP_SHARED, fd_in, 0 );

    if ( region == (caddr_t*)-1 ){
        perror( "mmap" );
	rc = -1;
    }else{
        buffer_def.buffer = (short*)region;
	rc = 0;
    }

    for ( iter = 0; iter != loop; ++iter ) {
/*
 * (3) ioctl() - fetch the data
 */
	if ( rc == 0  &&  fetch ){
	    rc = ioctl( fd_in, ACQ32_IOREAD_LOCALBUF, &buffer_def );
    
	    if ( rc < 0 ){
		perror( "ioctl ACQ32_IOREAD_LOCALBUF failed" );
		exit( 1 );
	    }
	}
	
	if ( verbose > 1 ){
	    fprintf( stderr, "press <cr> to continue\n" );
	    getchar();
	}
	if ( rc >= 0  ){
	    actual_samples = rc;
	    
	    if ( fd_out > 0 ){
		write( fd_out, buffer_def.buffer, actual_samples*2 );
	    }
	    if ( do_stats ){
		doStats( buffer_def.buffer, actual_samples, channel, 1 );
	    }
	}
    }
/*
 * (4) clean up 
 */

    if ( do_munmap ){
        if ( verbose > 2 ) {
            fprintf( stderr, "call munmap()\n" );
    	    getchar();
	}
	
        rc = munmap( region, buffer_def.nsamples*2 );
        if ( rc != 0 ){
            perror( "ERROR:munmap() failed " );
	    exit( 1 );
	}
    }

    if ( verbose > 2 ) {
        fprintf( stderr, "call close()\n" );
    	getchar();
    }
	
    rc = close( fd_in );
    if ( rc != 0 ){
        perror( "ERROR:close() failed " );
    	exit( 1 );
    }
}

int main( int argc, const char* argv[] )
{
    int fd_out = -1;
    char* outfname = NULL;
    const char* arg;
    struct READ_LOCALBUF_DESCR buffer_def;
    int fetch = 1;
    int channel1, channel2, ch;
    int loop = 1;
    int iter;
    int rc;
    int hold = 0;
    
    struct poptOption opt_table[] = {
        { "help",   'h', POPT_ARG_NONE,             0, 'h' },
	{ "hold",   'H', POPT_ARG_INT           &hold,   0 },
        { "write",  'w', POPT_ARG_NONE,             0, 'w' },
	{ "verbose",'v', POPT_ARG_INT,	     &verbose, 0   },
	{ "output", 'o', POPT_ARG_STRING,   &outfname, 'o' },
	{ "nofetch",'n', POPT_ARG_NONE,             0, 'n' },
	{ "nomunmap",'m',POPT_ARG_NONE,             0, 'm' },
	{ "loop",   'l', POPT_ARG_INT,          &loop, 0   },
	{ "stats",  'S', POPT_ARG_NONE,             0, 'S' },
	{ "markers", 'M', POPT_ARG_NONE,            0, 'M' },
	{ "chunk_size", 'C', POPT_ARG_INT, &chunk_size, 0 },
	{ "xsect",  'X', POPT_ARG_NONE,             0, 'X' },
	{ "zero",   'z', POPT_ARG_NONE,             0, 'z' },
        { }
    };

    poptContext opt_context;

    opt_context = poptGetContext( argv[0], argc, argv, opt_table, 0 );

    while ( (rc = poptGetNextOpt( opt_context )) > 0 ){
        switch( rc ){
	case 'X':
	    do_Xsect = 1;
	    break;
	case 'n':
	    fetch = 0;
	    break;
	case 'm':
	    do_munmap = 0;
	    break;
	case 'h':
	    fprintf( stderr, HELP );
	    return 1;
	case 'o':		
	    if ( outfname[0] == '-' ){
		fd_out = 1;                // use stdout
	    }else{
		fd_out = open( outfname, O_WRONLY|O_CREAT|O_TRUNC, 0666 );
			
		if ( fd_out < 0 ){
		    perror( "failed to open output file" );
		    return fd_out;
		}
	    }
	    break;
	case 'S':
	    do_stats = 1;
	    break;    
	case 'M':
	    do_markers = 1;
	    break;
	case 'z':
	    do_zero = 1;
	    break;
        }
    }  // processes all other opts via arg pointers

    if ( (arg = poptGetArg( opt_context )) == NULL ){
	fprintf( stderr, "please specify channel\n" );
	return 1;
    }else{
	if ( sscanf( arg, "%d..%d", &channel1, &channel2 ) != 2 ) {
	    channel1 = atoi( arg );			// zero based channel #
	    channel2 = channel1;
	}
    }

	
    // default settings
	
    buffer_def.istart = 0;
    buffer_def.istride = 1;

    if ( (arg = poptGetArg( opt_context )) != NULL ){
        buffer_def.nsamples = strtol( arg, 0, 0 );
    }else{
	fprintf( stderr, 
		 "sorry, please specify nsamples (not optional yet)\n" );
    }
    if ( (arg = poptGetArg( opt_context )) != NULL ){
        buffer_def.istart = strtol( arg, 0, 0 );
    }
    if ( (arg = poptGetArg( opt_context )) != NULL ){	
        buffer_def.istride = strtol( arg, 0, 0 );
    }

    if ( do_Xsect ){
	acq32fetchX( channel1, buffer_def, fetch, fd_out, loop );
    }else{
	for ( iter = 0; iter != loop; ++iter ) {
	    for ( ch = channel1; ch <= channel2; ++ch ){
		acq32fetch( ch, buffer_def, fetch, fd_out, hold );
	    }
	}
    }
    return 0;
}






