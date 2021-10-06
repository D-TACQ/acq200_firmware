/* ------------------------------------------------------------------------- */
/* bigread.c - recommended data read method for acq2xx devices               */
/* ------------------------------------------------------------------------- */
/*   Copyright (C) 2004 Peter Milne, D-TACQ Solutions Ltd
 *                      <Peter dot Milne at D hyphen TACQ dot com>
                                                                               
    This program is free software; you can redistribute it and/or modify
    it under the terms of Version 2 of the GNU General Public License
    as published by the Free Software Foundation;
                                                                               
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
                                                                               
    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.                */
/* ------------------------------------------------------------------------- */


//#include "local.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <popt.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define HELP \
"bigread $Revision: 1.1.2.3 $\n"\
"bigread [-b board] [ch1] .... [chN] \n"\
"bigread [-n nsamples]\n"\
"bigread [-v verbose]\n"\
""

#define FNAME "/dev/acq32/acq32.%d.%02d"

poptContext opt_context;
int verbose = 0;
int outfd = 1;

#define FN __FUNCTION__

#define dbg(lvl,format, arg...)					\
	do {							\
		if(verbose>=lvl)				\
			fprintf(stderr, "%s: " format "\n",	\
				FN, ## arg);		        \
	} while(0)						\


#define PRINTF(format, arg...)  dbg(1, format, ##arg)


int big_read(int fd, void* buf, int max_samples)
{
	int total = 0;
	int maxbuf = max_samples *sizeof(short);
	int nread = maxbuf;

	while( total < maxbuf && (nread = read(fd, buf, nread)) > 0){

		dbg(3, "maxbuf:%d total:%d nread:%d",
		    maxbuf, total, nread);

		buf += nread;
		total += nread;
		nread = maxbuf - total;
	}

	dbg(3, "maxbuf:%d total:%d nread:%d",
		    maxbuf, total, nread);
	
	return total/sizeof(short);
}

static void doBigRead(int board, const char* charg, int nsamples)
/** simple case - charg represents a single channel */
{
	int ch = atoi(charg);
	char fname[80];
	int fd;
	void *buf = malloc(nsamples*sizeof(short));
	int totsam;

	sprintf(fname, FNAME, board, ch);

	fd = open(fname, O_RDONLY);
	assert(fd >= 0);
	assert(buf);

	totsam = big_read(fd, buf, nsamples);
	
	dbg(1, "big_read %s asked:%d got:%d", fname, nsamples, totsam);

	write(outfd, buf, totsam*2);
}


int main( int argc, const char* argv[] )
{
    int rc;
    int board = 1;
    int nsamples = 1024000;
    const char* charg;
    const char* outfile;

    struct poptOption opt_table[] = {
        { "version",    0,  POPT_ARG_NONE,   0,           'h' },
        { "board",      'b', POPT_ARG_INT,    &board,      'b' },
	{ "nsamples",   'n', POPT_ARG_INT,    &nsamples,   0   },
	{ "output",     'o', POPT_ARG_STRING, &outfile,    'o' },
        { "verbose",    'v', POPT_ARG_INT,    &verbose,    0   },
	POPT_AUTOHELP
	POPT_TABLEEND
    };

    opt_context = poptGetContext( argv[0], argc, argv, opt_table, 0 );

    while ( (rc = poptGetNextOpt( opt_context )) > 0 ){
        switch( rc ){
	case 'o':
		outfd = open(outfile, O_WRONLY);
		assert(outfd>0);
		break;
        case 'h':
            fprintf( stderr, HELP );
            return 1;
        }
    }  // processes all other opts via arg pointers

    while((charg = poptGetArg(opt_context)) != 0){
	    doBigRead(board, charg, nsamples);
    }

    return 0;
}




