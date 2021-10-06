/*****************************************************************************
 *
 * File: tv.c
 *
 * $RCSfile: tv.c,v $
 * 
 * Copyright (C) 1999 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: tv.c,v 1.2 2001/04/01 19:42:38 pgm Exp $
 * $Log: tv.c,v $
 * Revision 1.2  2001/04/01 19:42:38  pgm
 * tv et al work
 *
 * Revision 1.1.1.1  2001/04/01 12:27:06  pgm
 *
\*****************************************************************************/





#undef __KERNEL__

#include "local.h"
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
#include "acq32busprot.h"



#define FNAME  "/dev/acq32/acq32.%d.%02d"

#define HELP \
"tv $Revision: 1.2 $\n"\
"tv [opts] channel [length] [start] [stride]\n"\
"defaults:\n"\
"    length:     all of it\n"\
"    start:      0\n"\
"    stride:     1\n"\
"opts:\n"\
"    -v verbose	 verbose output\n"\
""

int verbose = 0;

struct FRAME_DATA {
	unsigned sf;
	unsigned ch;
	unsigned trigger;
	unsigned isbit;
	unsigned major;
};

void fill_fd( struct FRAME_DATA* pfd, unsigned short data, unsigned* sample )
{
	pfd->sf			= data&T_SUBFRAME_MASK;
	pfd->ch 		= (data&T_CHANNEL_MASK)>>7;
	pfd->trigger	= (data&T_TRIGGER_BIT) != 0;
	pfd->isbit  	= (data&T_ISAMPLE_BIT) != 0;
	pfd->major		= (data&T_RAMP_MASK)>>13;
	
	if ( pfd->sf == 0 ){
		*sample = 0;
	}
	*sample        |= pfd->isbit<<pfd->sf;
}

static int increment_sf( unsigned sf )
{
	return ++sf&0x3f;
}

static int validate( 
	int zchan,
	struct READ_LOCALBUF_DESCR* buffer_def 
)
{
	int ibuf;
	struct FRAME_DATA	fd[2];
	struct FRAME_DATA* this_frame = &fd[0];
	struct FRAME_DATA* prev_frame = &fd[1];
	struct FRAME_DATA* temp;
	
	unsigned prev_sample = buffer_def->istart;
	unsigned sample = 0;
	int mismatches = 0;
	
	if ( verbose > 2 ){
		printf( "%10s %4s %2s %2s %c %c %10s\n", 
				"ibuf", "data", "ch", "sf", 'T', 'x', "Sample" );
	}
	

	fill_fd( prev_frame, *(unsigned short*)&buffer_def->buffer[0], &sample );
	
	for ( ibuf = 1; ibuf != buffer_def->nsamples; ++ibuf ){
		
		unsigned short data = *(unsigned short*)&buffer_def->buffer[ibuf];		

		fill_fd( this_frame, data, &sample );		

		if ( verbose > 2 ){
			printf( "0x%08x %04x %02d %2d %d %d 0x%08x\n",
					ibuf, data, 
					this_frame->ch, 
					this_frame->sf, 
					this_frame->trigger, 
					this_frame->isbit, sample );
		}

		if ( increment_sf( prev_frame->sf ) != this_frame->sf ){
			printf( "ERROR: sample [0x%08x] subframe out of sync %d %d\n",
					ibuf, increment_sf( prev_frame->sf ), this_frame->sf );
			++mismatches;
		}else if ( prev_frame->ch != this_frame->ch ){
			printf( "ERROR: sample [0x%08x] channel slip %d %d\n",
						ibuf, prev_frame->ch, this_frame->ch );	
			++mismatches;
		}else if ( this_frame->sf == 63 ){
			if ( ibuf > 63 ){
				if ( sample == prev_sample + 64 ){
					if ( verbose > 1 ){
						printf( "Samples match at 0x%08x\n", sample );
					}
				}else{
					printf( "ERROR: sample[0x%08x] mismatch at 0x%08x 0x%08x 0x%04x\n", 
						ibuf, 
						prev_sample, sample, data );
					++mismatches;
				}
			}
			prev_sample = sample;
		}

		SWAP( this_frame, prev_frame, temp );
	}
	if ( verbose ){
		if ( mismatches ){
			printf( "mismatches=%d\n", mismatches );
		}else{
			printf( "no mismatched in %d records\n", ibuf );
		}
	}
	if ( mismatches ){
			printf( "mismatches=%d\n", mismatches );
	}
	return mismatches != 0;
}
 
int main( int argc, const char* argv[] )
{
  /* WORKTODO ... args handling */

    int fd_in; 
    void* region;
    char fname[80];
    int rc;
    const char* arg;
    struct READ_LOCALBUF_DESCR buffer_def;
	int zchan;
	int channel, board;									// zero based channel #
        
    struct poptOption opt_table[] = {
        { "help",   'h', POPT_ARG_NONE,             0, 'h' },
        { "write",  'w', POPT_ARG_NONE,             0, 'w' },
		{ "verbose",'v', POPT_ARG_INT,	     &verbose, 0   },
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

    if ( (arg = poptGetArg( opt_context )) == NULL ){
        fprintf( stderr, "please specify channel\n" );
	    return 1;
	}else{
		zchan = atoi( arg ) - 1;
	    channel = zchan%32 + 1;
	    board   = zchan/32 + 1;
	    
	    sprintf( fname, FNAME, board, channel );

	    if ( verbose ){
	    	fprintf( stderr, "using device %s\n", fname );
		}
        if ( (fd_in = open( fname, O_RDONLY )) < 0 ){
            fprintf( stderr, "mmap: failed to open device \"%s\" - ", fname );
            perror( "" );
            return 1;
        }
	}
	
	// default settings
	
	buffer_def.istart = 0;
	buffer_def.istride = 1;

    if ( (arg = poptGetArg( opt_context )) != NULL ){
        buffer_def.nsamples = strtol( arg, 0, 0 );
	}else{
	    fprintf( stderr, "sorry, please specify nsamples (not optional yet)\n" );
	}
    if ( (arg = poptGetArg( opt_context )) != NULL ){
        buffer_def.istart = strtol( arg, 0, 0 );
    }
    if ( (arg = poptGetArg( opt_context )) != NULL ){	
        buffer_def.istride = strtol( arg, 0, 0 );
	}

    region = mmap( NULL, buffer_def.nsamples*2, PROT_READ, MAP_SHARED, fd_in, 0 );

    if ( region == (caddr_t*)-1 ){
        perror( "mmap" );
	    rc = -1;
    }else{
        buffer_def.buffer = (short*)region;
	    rc = 0;
	}

    if ( rc == 0 ){
        rc = ioctl( fd_in, ACQ32_IOREAD_LOCALBUF, &buffer_def );
    
        if ( rc < 0 ){
            perror( "ioctl ACQ32_IOREAD_LOCALBUF failed" );
	    }
    }

	return validate( zchan, &buffer_def );
}






