/*****************************************************************************
 *
 * File: mean.c
 *
 * $RCSfile: mean.c,v $
 * 
 * Copyright (C) 1999 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description: calculates mean of 32 raw input words
 *
 * Opts:  [-n nmean] - take mean of <nmean> values {256}
 *        [-d]       - output as decimal
 *        [-v]       - output as volts
 *
 * $Id: mean.c,v 1.4 2002/11/15 09:57:57 pgm Exp $
 * $Log: mean.c,v $
 * Revision 1.4  2002/11/15 09:57:57  pgm
 * add nchannels range opts
 *
 * Revision 1.3  1999/11/06 21:09:11  pgm
 * just use good old mean=sum/n
 *
 * Revision 1.2  1999/10/25 09:59:40  pgm
 * looks good after mean simulation
 *
 * Revision 1.1  1999/10/24 21:08:42  pgm
 * mean.c ... mean filter for acq32 bin data
 *
 * Revision 1.1  1999/10/22 16:26:49  pgm
 * first entry to cvs
 *
 *
\*****************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <popt.h>

#include "local.h"

#define NCHANNELS 32
#define FSVOLTS 10
#define FRACTBITS 4

static int nchannels = NCHANNELS;
static int fsvolts   = FSVOLTS;
int left_shift = FRACTBITS;                          // default left shift

static char* HELP =
"mean $Revision: 1.4 $\n"
"usage: mean [opts] [-dhvn <nmean> -s <sim-mean>] <input >output\n"
"--nchannels <nchan> {32}\n"
"--fsvolts <v> {10V} (bipolar only)\n"
"--fractional_bits <b> [4] compute mean to this many fractional bits\n";

#define FPIN stdin
#define FPOUT stdout

void simulate( int means[], int real_mean )
{
    int ichan, delta;

    for ( ichan = 0, delta = 0; ichan != nchannels; ++ichan ){
        means[ichan] = (real_mean+delta) << 4;

        delta = delta>=0? -(delta+1): -delta;
    }
}

int main( int argc, char** argv )
{
    int nmean = 256;                               // take mean of nsamples
    enum { BIN, DEC, HEX, VOLTS } outfmt = BIN;    // output format
    int means[NCHANNELS] = {};
    short raws[NCHANNELS];
    int nn;
    int raw, ichan;

    int sim_mean = 0;
    
    struct poptOption opt_table[] = {
        { "nmean", 'n', POPT_ARG_INT,  &nmean, 0 },
        { "dec",   'd', POPT_ARG_NONE, 0, DEC },
        { "hex",   'h', POPT_ARG_NONE, 0, HEX },
        { "volts", 'v', POPT_ARG_NONE, 0, VOLTS },
        { "help",  'h', POPT_ARG_NONE, 0, 'h' },
        { "simulate", 's', POPT_ARG_INT, &sim_mean, 's' },
	{ "fsvolts", '\0', POPT_ARG_INT, &fsvolts },
	{ "nchannels", '\0', POPT_ARG_INT, &nchannels },
	{ "fractional_bits", '\0', POPT_ARG_INT, &left_shift },
        { }
    };

    poptContext opt_context = 
        poptGetContext( argv[0], argc, argv, opt_table, 0 );
    int rc;

    while ((rc = poptGetNextOpt( opt_context )) > 0 ){
        switch( rc ){
        case 's':
            simulate( means, sim_mean );
            goto output_data;
        case DEC:
        case HEX:
        case VOLTS:
            outfmt = rc;
            break;
        case 'h':
            fprintf( stderr, HELP );
            return 1;
        }
    }

    poptGetNextOpt( opt_context );

    fprintf( stderr, "mean: nmean:%d outfmt:%d\n", nmean, outfmt );

    for ( nn = 1; nn <= nmean; ++nn ){
       if ( fread( raws, sizeof(short), nchannels, FPIN ) == nchannels ){
	   for ( ichan = 0; ichan != nchannels; ++ichan ){
               raw = raws[ichan];
               raw <<= left_shift;

	       //               means[ichan] = (raw + (nn-1)*means[ichan])/nn; 
               means[ichan] += raw;
	   }
       }else{
           perror( "ERROR: mean ran out of data" );
           return 1;
       }
    }

    for ( ichan = 0; ichan != nchannels; ++ichan ){
        means[ichan] /= nmean;
    }
#define TERMCSV( ichan )  ((ichan)+1==nchannels? '\n': ',')

output_data:

    switch( outfmt ){
    case BIN:
         fwrite( means, sizeof(int), nchannels, FPOUT );
         break;
    case DEC:
         for ( ichan = 0; ichan != nchannels; ++ichan ){
	     fprintf( FPOUT, "%6d%c", means[ichan], TERMCSV(ichan) );
         }
         break;
    case HEX:
         for ( ichan = 0; ichan != nchannels; ++ichan ){
	     if ( left_shift == 0 ){
		 fprintf( FPOUT, "0x%04x%c",
			  (unsigned)means[ichan]&0x0ffff, TERMCSV(ichan) );
	     }else{
		 // valid for leftshift == 4 only
		 fprintf( FPOUT, "0x%05x%c", 
			  (unsigned)means[ichan]&0x0fffff, TERMCSV(ichan) );
	     }
         }
         break;
    case VOLTS:
         for ( ichan = 0; ichan != nchannels; ++ichan ){
             float volts = (float)means[ichan];
             
             volts /= (1<<left_shift);
             volts =  10*volts/32768;
             
             fprintf( FPOUT, "%8.4f%c", volts, TERMCSV(ichan) );
         }
    }
    return 0;
}
