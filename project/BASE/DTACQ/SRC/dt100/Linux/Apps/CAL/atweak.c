/*****************************************************************************
 *
 * File: atweak.c
 *
 * $RCSfile: atweak.c,v $
 * 
 * Copyright (C) 1999 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description: give the cal "pots" a tweak.
 *            
 * Input is a row of binary means from "mean".
 * Output is a revised cal-file
 *
 * The calfile is in the format:
0xOFF1GAN1
0xOFF2GAN2
...
0xOF32GA32

but is used as an array short [32][2] where offset is [*][1]

 The CALDACS are 10 bit, Offset Binary coded
 and the default CALDACS file <cal_dacs> is has all 
 dacs set to mid range by default (0x02000200)


 *
 * $Id: atweak.c,v 1.5.4.1 2003/12/15 14:15:09 pgm Exp $
 * $Log: atweak.c,v $
 * Revision 1.5.4.1  2003/12/15 14:15:09  pgm
 * works with gcc3.2.2
 *
 * Revision 1.5  1999/11/01 20:13:46  pgm
 * gain cal works with negative gain .. ship it quick!
 *
 * Revision 1.4  1999/11/01 13:17:46  pgm
 * mbox, mboxcontrol have async arm notification
 *
 * Revision 1.3  1999/10/25 10:06:27  pgm
 * better help
 *
 * Revision 1.2  1999/10/25 09:59:40  pgm
 * looks good after mean simulation
 *
 * Revision 1.1  1999/10/25 09:26:51  pgm
 * first pass tweaks seems to work
 *
\*****************************************************************************/

const char* DOC = 
" * atweak $Revision: 1.5.4.1 $\n"
" * \n"
" * Usage: atweak {offset|gain} ideal-code\n"
" *\n"
" * Opts:  [-x]                 - offset is [*][0] (deprecated) - now default\n"
" *        [-X]                 - swaps offset/gain if needed in future\n"
" *        [-i <cal-file> ]     - input cal file [def:defcal]\n"
" *        [-o <cal-file> ]     - output cal file [def:stdout]\n"
" *        [-d <debug-level>] \n"
" *        [-O <Koffset>]\n"
" *        [-G <Kgain> ]\n"
" *        \n"
" * example use with simulated data\n"
"\n"
"$ mean -s 0 | atweak -d 1 offset 0                 ; show example calc\n"
"\n"
"\n"
" * real life example, using and updating a file <mycal>\n"
"\n"
"$ setDACS 0\n"
"$ setCAL mycal                                     ; update cal data\n"
"$ softTransient                                   \n"
"$ dump | mean | atweak -i mycal -o mycal offset 0\n"
"$ setDACS 0x7fff7fff\n"
"$ setCAL mycal\n"
"$ softTransient\n"
"$ dump | mean | atweak -i mycal -o mycal gain 0x7fff\n"
"\n"
" * enjoy *\n"
;


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <popt.h>

#include "local.h"

#define NCHANNELS 32

#define FPIN stdin
#define FPOUT stdout

#define MAX_DAC 0x3ff

#define KOFFSET 10
#define KGAIN    -2

#define LEFT_SHIFT 4

static int G_debug;

static void getCalDacSettings( unsigned* cal_dacs, char* calfile )
{
    int ichan;
    char aline[80];

    FILE* fp = fopen( calfile, "r" );
    ASSERT( fp );

    for ( ichan = 0; ichan != NCHANNELS; ++ichan ){
        if ( fgets( aline, 80, fp ) == NULL ){
            fprintf( stderr, "ERROR in calfile %s, out of data at line %d\n",
                               calfile, ichan );
            exit( 1 );
        }
        cal_dacs[ichan] = strtoul( aline, 0, 0 );
    }
}

static void getMeans( int* means )
{
    int nread = fread( means, sizeof(int), NCHANNELS, stdin );

    ASSERT( nread==NCHANNELS );
}

static void putCalDacSettings( unsigned* cal_dacs, char* outfn )
{
    FILE* fp;
    int ichan;

    if ( outfn != NULL ){
        fp = fopen( outfn, "w" );
        ASSERT( fp );
    }else{
        fp = stdout;
    }

    for ( ichan = 0; ichan != NCHANNELS; ++ichan ){
        fprintf( fp, "0x%08x\n", cal_dacs[ichan] );
    }    
}



struct CalData {
    int*   means;
    unsigned short* cal_dacs;
    int    ipram;
    int    kk;
    int    ideal;
};


static void makeTweak( struct CalData* cal_data )
{
    int ichan;
    unsigned short* pdacs = cal_data->cal_dacs+cal_data->ipram;
    int delta;
    int delta_dac;
    short dac_val;

    if ( G_debug ){
        fprintf( stderr, "makeTweak: %s, K = %d\n",
		 cal_data->ipram==0? "evens": "odds", cal_data->kk );
    }

    for ( ichan = 0; ichan != NCHANNELS; ++ichan ){

        delta = cal_data->ideal - cal_data->means[ichan];
        delta_dac = cal_data->kk * delta;
        dac_val = pdacs[ichan*2] + (delta_dac >> LEFT_SHIFT);

        if ( dac_val < 0 ){
            dac_val = 0;
            fprintf( stderr, "WARNING: channel %d dac UNDERRANGE\n", ichan );
        }else if ( dac_val > MAX_DAC ){
            dac_val = MAX_DAC;
            fprintf( stderr, "WARNING: channel %d dac OVERRANGE\n", ichan );
        }

        pdacs[ichan*2] = dac_val;

        if ( G_debug ){
            fprintf( stderr, 
            "%2d: ideal:%6d actual:%6d delta:%6d delta_dac:%d\n",
                  ichan, cal_data->ideal, cal_data->means[ichan],
                  delta, delta_dac
            );
	}
    }
}


int main( int argc, char* argv[] )
{

  /* 
   * instantiate main data structures 
   */
    struct CalData cal_data;             // "this"
    int means[NCHANNELS];                // store input actuals here
    unsigned cal_dacs[NCHANNELS];        // store old, new cal settings here

 /*
  * loads of tedious args processing ...
  */

    int   OFFSET = 0;
#define GAIN (!OFFSET)

    char* infn = "cal_dacs";
    char* outfn = NULL;
    char* pram_sel;
    char* ideal;
    int koffset = KOFFSET;
    int kgain   = KGAIN;

    struct poptOption opt_table[] = {
        { "xchg",     'x', POPT_ARG_NONE, 0, 0          },
        { "exchange", 'X', POPT_ARG_NONE, 0, 'x'        },
        { "if",       'i', POPT_ARG_STRING, &infn, 'i'  },
        { "of",       'o', POPT_ARG_STRING, &outfn, 'o' },
        { "debug",    'd', POPT_ARG_INT,    &G_debug, 0 },
        { "gain",     'G', POPT_ARG_INT,    &kgain,   0 },
        { "offset",   'O', POPT_ARG_INT,    &koffset, 0 },
        { "help",     'h', POPT_ARG_NONE, 0, 'h'        },
        { }
    };

    poptContext opt_context = 
        poptGetContext( argv[0], argc, argv, opt_table, 0 );
    int rc;
    
    while ((rc = poptGetNextOpt( opt_context )) > 0 ){
        switch( rc ){
        case 'x':
            OFFSET = !OFFSET;
            break;
        case 'h':
            fprintf( stderr, DOC );
            return 1;
        }
    }

    if ( (pram_sel = poptGetArg( opt_context )) == NULL ){
        fprintf( stderr, "ERROR: please supply offset|gain select (--help)\n");
        return 1;
    }else if ( STREQ( pram_sel, "offset" ) ){
        cal_data.kk = koffset;
        cal_data.ipram = OFFSET;
    }else if ( STREQ( pram_sel, "gain" ) ){
        cal_data.kk = kgain;
        cal_data.ipram = GAIN;
    }else{
        fprintf( stderr, "ERROR: please supply offset|gain select (--help)\n");
        return 1;
    }

    if ( (ideal = poptGetArg( opt_context )) == NULL ){
        fprintf( stderr, "ERROR: please supply ideal code\n" );
        return 1;
    }else{
        cal_data.ideal = strtol( ideal, 0, 0 ) << LEFT_SHIFT;
    }


    /*
     * now down to business 
     */

    getCalDacSettings( cal_dacs, infn );
    cal_data.cal_dacs = (unsigned short*)cal_dacs;

    getMeans( means );
    cal_data.means = means;

    makeTweak( &cal_data );

    putCalDacSettings( cal_dacs, outfn );

    return 0;
}





 
