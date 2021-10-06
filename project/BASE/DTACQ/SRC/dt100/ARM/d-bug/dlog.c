/*****************************************************************************
 *
 * File:
 *
 * $RCSfile: dlog.c,v $
 * 
 * Copyright (C) 2001 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: dlog.c,v 1.7.4.1 2003/08/29 17:41:02 pgm Exp $
 * $Log: dlog.c,v $
 * Revision 1.7.4.1  2003/08/29 17:41:02  pgm
 * init pci command handling
 *
 * Revision 1.7  2002/06/10 11:53:50  pgm
 * streaming runs, but the data is crap
 *
 * Revision 1.6  2002/03/21 11:54:51  pgm
 * untabify
 *
 * Revision 1.5  2002/03/13 21:43:09  pgm
 * correct formatting with emacs (Stroustrup)
 *
 * Revision 1.4  2001/03/20 21:51:04  pgm
 * added headers
 *
 * Revision 1.2  2000/12/28 11:03:00  pgm
 * *** empty log message ***
 *
 * Revision 1.1  1999/10/22 16:26:49  pgm
 * first entry to cvs
 *
 *
\*****************************************************************************/




#include <stdio.h>

#include "local.h"
#include "memmap.h"

#include "dlog.h"

#include <stdarg.h>

#define LINE            128
#define BUFFERLEN       10000

#define BS            (EXTENDED_STORE_BASE+(64*0x100000/4))

typedef char BufferLine[LINE];

#define BUFFERSTART ((BufferLine*)BS+LINE)

#define BUFFEREND       (&BUFFERSTART[BUFFERLEN])

typedef struct {
    unsigned guard1;
    BufferLine* pbuffer;
    unsigned has_wrapped;
    unsigned guard2;
    unsigned dlog_enable;
}
BufferStuff;

#define BUFFERSTUFF     ((BufferStuff*)BS)     



static int dlogInit()
{
    PRINTF( "Building dlog at %p\n", BS );
    BUFFERSTUFF->guard1 = 0xfeedc0de;
    BUFFERSTUFF->pbuffer = BUFFERSTART;
    BUFFERSTUFF->has_wrapped = 0;
    BUFFERSTUFF->guard2 = 0xdeadbeef;
    return 0;
}

extern int pgm_getchar( void ); // buried deep in startup code

int dumpPage( int last_entry )
{
    int iback = BUFFERSTUFF->pbuffer - BUFFERSTART;
    int iprint = 0;

    for ( ; last_entry > iback && iprint < 24; last_entry--, ++iprint ){
        printf( "%s", BUFFERSTART[BUFFERLEN-last_entry+iback] );
    }
    for ( ; last_entry > 0 && last_entry <= iback && iprint<24; 
          last_entry--, ++iprint ){
        printf( "%s", BUFFERSTART[last_entry-1] );
    }
//    return getchar() == 'n';            WORKTODO - distinguish serialpci debug
    return 1;
}
int dlogDump( void )
{
    if ( BUFFERSTUFF->guard1 == 0xfeedc0de && 
         BUFFERSTUFF->guard2 == 0xdeadbeef ){

        int maxentries = BUFFERSTUFF->has_wrapped? 
            BUFFERLEN: BUFFERSTUFF->pbuffer-BUFFERSTART;
        int nentries = maxentries;

        if ( BUFFERSTUFF->has_wrapped ){
            BufferLine* pget = BUFFERSTUFF->pbuffer;
	    
	        while( ++pget < BUFFEREND ){
		        printf( "%s", *pget );
			}
	    }
	    {
	        BufferLine* pget = BUFFERSTART;
		    do {
		        printf( "%s", *pget );
		    } while( ++pget <= BUFFERSTUFF->pbuffer );
	    }
        return maxentries - nentries;
    }else{
        printf( "dlogDump NO ENTRIES\n" );
        return 0;
    }
}

int dlogEnable( int enable )
{
    dlogInit();
    BUFFERSTUFF->dlog_enable = enable? 0xcafebabe: 0xdeadbeef;
    return 0;
}
int dlogPrintf( const char* format, ... )
{
    va_list args;


    va_start( args, format );

    if ( dlogIsEnabled() ){
        int rv;
        rv = vsprintf( *BUFFERSTUFF->pbuffer, format, args );
        va_end( args );

        if ( ++BUFFERSTUFF->pbuffer >= BUFFEREND ){
            BUFFERSTUFF->pbuffer = BUFFERSTART;
            BUFFERSTUFF->has_wrapped = 1;
        }
        return rv;
    }else{
        va_end( args );
        return 0;
    }
}

int dlogIsEnabled() 
{
    return BUFFERSTUFF->dlog_enable == 0xcafebabe;
}
