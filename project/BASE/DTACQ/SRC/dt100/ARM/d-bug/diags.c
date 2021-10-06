/*****************************************************************************
 *
 * File: diags.c
 *
 * $RCSfile: diags.c,v $
 * 
 * Copyright (C) 1999 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description: diagnostics
 *
 * $Id: diags.c,v 1.12.4.2 2004/10/14 20:12:25 pgm Exp $
 * $Log: diags.c,v $
 * Revision 1.12.4.2  2004/10/14 20:12:25  pgm
 * no warnings
 *
 * Revision 1.12.4.1  2003/09/08 12:54:20  pgm
 * i2o file download works. ship it quick
 *
 * Revision 1.12  2002/08/21 12:06:12  pgm
 * this is as fast as it gets
 *
 * Revision 1.11  2002/03/21 11:54:51  pgm
 * untabify
 *
 * Revision 1.10  2002/03/13 21:43:09  pgm
 * correct formatting with emacs (Stroustrup)
 *
 * Revision 1.9  2001/08/27 12:55:18  pgm
 * acq16 runs again - needs LCA bit change, caveat embedded trigger
 *
 * Revision 1.8  2001/07/11 21:17:58  pgm
 * AI fifo==32, should fix AO load probs
 *
 * Revision 1.7  2001/05/13 20:00:46  pgm
 * thrashing around to get AO to go
 *
 * Revision 1.6  2001/04/09 09:58:49  pgm
 * deep grief, distributor bug fixed (testing..)
 *
 * Revision 1.5  2001/03/19 23:47:22  pgm
 * updated d-bug help, nodos
 *
 * Revision 1.4  2001/03/18 17:38:44  pgm
 * builds with max 2 warnings
 *
 * Revision 1.3  2000/12/29 22:28:43  pgm
 * better diags, correct int clock, opt1
 *
 * Revision 1.2  2000/10/18 07:26:03  pgm
 * fix rescinded GATE in GATED_TRANSIENT
 *
 * Revision 1.1  2000/10/15 19:58:04  pgm
 * multi samples/cycle WORKS, streams too and is FAST enough
 *
 * Revision 1.3  2000/06/24 15:17:29  pgm
 * I2O rools OK
 *
 * Revision 1.2  1999/11/19 20:07:01  pgm
 * distributor func, internal samplerate 200k
 *
 *
 *
\*****************************************************************************/

#include "local.h"

#include <string.h>

#include "acq32_drv.h"
#include "acq32_hml.h"

#include "diags.h"

extern int sprintf( char*, const char*, ... );

void acq32_fifo_histo_reset( int quick )
{
#define HRP( pp )     PRINTF( "%s" #pp " %p\n", FN, pp )

    if ( quick ) {
        memset( NFIFO_HISTO, 0, sizeof(Histogram) );
    }else{
        memset( NFIFO_HISTO, 0, sizeof(Histogram) );
        memset( AO_HISTO, 0, sizeof(Histogram) );
        sprintf( ASSERT_STRING, "ACQ32: no worries\n" );
        
        HRP( NFIFO_HISTO );
        HRP( AO_HISTO );
        HRP( ASSERT_STRING );
    }
#undef HRP
}


#define FOUL_DOS_SEP     '\\'
#define SLASH            '/'

void makeAssertion( char* file, int line )
{
    char* pfile = file+strlen(file);
    
    while( pfile != file ){
        if ( pfile[-1] == FOUL_DOS_SEP || pfile[-1] == SLASH ){
            break;
        }
        pfile--;
    }
    
    memset( ASSERT_STRING, 0, sizeof(Histogram) );
    sprintf( ASSERT_STRING, "ASSERT %s %d\n", pfile, line );

    printf( ASSERT_STRING );
    while( 1 ){
        ;
    }
}
void makeMarker( char* file, int line )
{
    memset( ASSERT_STRING, 0, sizeof(Histogram) );
    sprintf( ASSERT_STRING, "MARKER %s %d\n", file, line );
}

void makeEmbed( char* file, int line, void* p1, void* p2 )
{
    void** pv = (void**)(ASSERT_STRING+2*sizeof(Histogram));
    
    pv[0] = (void*)line;
    pv[1] = p1;
    pv[2] = p2;
    pv[3] = (void*)0xdddddddd;
}

