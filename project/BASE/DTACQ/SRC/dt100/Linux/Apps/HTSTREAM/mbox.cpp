/*****************************************************************************
 *
 * File: mbox.cpp
 *
 * $RCSfile: mbox.cpp,v $
 * 
 * Copyright (C) 2001 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: mbox.cpp,v 1.5 2003/06/22 12:42:38 pgm Exp $
 * $Log: mbox.cpp,v $
 * Revision 1.5  2003/06/22 12:42:38  pgm
 * local status works with correct data
 *
 * Revision 1.4  2002/08/12 10:56:07  pgm
 * streaming works, display from bigdump buf
 *
 * Revision 1.3  2002/04/10 19:53:26  pgm
 * first runner
 *
 * Revision 1.2  2002/04/10 18:23:11  pgm
 * first runner
 *
 * Revision 1.1  2002/04/10 15:00:04  pgm
 * *** empty log message ***
 *
 * Revision 1.2  2000/12/28 11:03:00  pgm
 * *** empty log message ***
 *
 * Revision 1.1  1999/10/22 16:26:49  pgm
 * first entry to cvs
 *
 *
\*****************************************************************************/



#include "local.h"


#include <assert.h>

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


#include "acq32ioctl.h"
#include "platform.h"


#include "mbox.h"

#define USE_IOCTL

typedef volatile u32 r32;


struct _Mailboxes {
    int fd;                         // alternate - use ioctl to access
    r32* mailboxes;                 // or mapping (x86 only)
    u32 mailboxes_shadow[4];        // up to dat cache of write values
    u32 mailboxes_lastwrite[4];     // what did we write last ??
};

#define FNAME_FMT        "/dev/acq32/acq32.%d."
#define FNAME_FMT_MBOX   FNAME_FMT "raw"
#define FNAME_FMT_DMABUF FNAME_FMT "01"    // don't ask - this is a convenience

int Mailboxes::set( int ibox, u32 value )      
// set a mail mbox register: ibox {0..3}. return 0 on success
{
    int rc;
    assert( IN_RANGE( ibox, 0, 3 ) );

    
    PRINTF(3)( "setMbox( %d, 0x%08x )\n", ibox, value );
#ifndef USE_IOCTL           
    m->mailboxes[ibox] = 
#endif
        m->mailboxes_shadow[ibox] = 
        m->mailboxes_lastwrite[ibox] = value;    

#ifdef USE_IOCTL
    rc = ioctl( m->fd, ACQ32_IOSMBX(ibox), value );
    
    PRINTF(4)( "ioctl( %d, %d, %08x ) returns %d\n",
               m->fd, ACQ32_IOSMBX(ibox), value, rc );
                        
    assert( rc >= 0 );
#endif
    return 0;   
}    

void Mailboxes::showLastWrites()
{
    fprintf( stderr, "lastWrites()\n" );
    fprintf( stderr, "0x%08x  0x%08x  0x%08x  0x%08x\n",
             m->mailboxes_lastwrite[0],
             m->mailboxes_lastwrite[1],
             m->mailboxes_lastwrite[2],
             m->mailboxes_lastwrite[3] );
}
int Mailboxes::setBits( int ibox, u32 bits_to_set )
{
    assert( IN_RANGE( ibox, 0, 3 ) );
    
    m->mailboxes[ibox] = m->mailboxes_shadow[ibox] |= bits_to_set;
    return 0;
}

int Mailboxes::clrBits( int ibox, u32 bits_to_clr )
{
    assert( IN_RANGE( ibox, 0, 3 ) );
    
    m->mailboxes[ibox] = m->mailboxes_shadow[ibox] &= ~bits_to_clr;
    return 0;
}

int Mailboxes::setField( int ibox, u32 field_mask, u32 field_value )
{
    assert( IN_RANGE( ibox, 0, 3 ) );
    
    m->mailboxes_shadow[ibox] &= ~field_mask;
    m->mailboxes_shadow[ibox] |= field_value;
    m->mailboxes[ibox] = m->mailboxes_shadow[ibox];
    return 0;
}

u32 Mailboxes::get( int ibox )      
// get contents of mailbox register: ibox {0..3}
{
    u32 newval;
    int rc;
    
    assert( IN_RANGE( ibox, 0, 3 ) );

#ifdef USE_IOCTL
    rc =  ioctl( m->fd, ACQ32_IOGMBX(ibox), &newval );
    assert( rc >= 0 );

    PRINTF(3)( "ioctl( %d, %d, %p ) output 0x%08x\n",
               m->fd, ACQ32_IOGMBX(ibox), &newval, newval );
#else
    newval = m->mailboxes[ibox];
#endif
    
    if ( newval != m->mailboxes_shadow[ibox] ){
        PRINTF(2)( "getMbox( %d ) = 0x%08x\n", 
		   ibox, m->mailboxes_shadow[ibox] );
    }
    
    m->mailboxes_shadow[ibox] = newval;
    return m->mailboxes_shadow[ibox];
}


u32 Mailboxes::pollBits( int ibox, u32 mask, u32 goal )
{
    int ipoll = 0;
    u32 mbtemp;
    
    while( ( (mbtemp = get( ibox ))&mask ) == 0 ){
        if ( (++ipoll&0x3ffff) == 0 ){
            fprintf( stderr, 
                     "pollMboxBits() mask:0x%08x goal:0x%08x got:0x%08x\n",
                     mask, goal, mbtemp       );
        }
    }
    
    PRINTF(3)( "pollMboxBits() returning 0x%08x\n", mbtemp );
    return mbtemp;
}


static struct _Mailboxes* mmapMbox( int iboard )   
// iboard {1..3}. return 0 on success
{
    char fname[80];

    void* region;
    _Mailboxes* m = new _Mailboxes;

    unsigned offset;

    assert( m != 0 );    
    assert( IN_RANGE( iboard, 1, 4 ) );
    
    sprintf( fname, FNAME_FMT_MBOX, iboard );

    if ( (m->fd = open( fname, O_RDWR )) < 0 ){
        fprintf( stderr, "mmap: failed to open device \"%s\" - ", fname );
        perror( "" );
        exit( 1 );
    }

    region = mmap( NULL, 0x400, PROT_READ|PROT_WRITE, MAP_SHARED, m->fd, 0 );

    if ( region == (void*)-1 ){
        perror( "mmap" );
        exit( 1 );
    }
    
    // IO not on page boundary - get the offset to adjust
    
    if ( ioctl( m->fd, ACQ32_IOG_PCIREGS_OFFSET, &offset ) != 0 ){
        perror( "ioctl ACQ32_IOG_PCIREGS_OFFSET" );
        exit( 1 );
    }
    m->mailboxes = (r32*)((char*)region+offset+MAILBOX_0);

    return m;
}


void Mailboxes::print()
{
    printf( "0x%08x 0x%08x 0x%08x 0x%08x\n",
	    m->mailboxes_shadow[0],
	    m->mailboxes_shadow[1],
	    m->mailboxes_shadow[2],
	    m->mailboxes_shadow[3]   );
}

void Mailboxes::printerr()
{
    fprintf( stderr, "0x%08x 0x%08x 0x%08x 0x%08x\n",
	    m->mailboxes_shadow[0],
	    m->mailboxes_shadow[1],
	    m->mailboxes_shadow[2],
	    m->mailboxes_shadow[3]   );
}


int Mailboxes::equals( Mailboxes& previous )
{
    int equals = 1;

    for ( int ibox = 0; ibox != 4; ++ibox ){
	get( ibox );
	if ( m->mailboxes_shadow[ibox] != previous.m->mailboxes_shadow[ibox] ){
	    equals = 0;
	}
    }
    return equals;
}


void Mailboxes::validateBigBuffer( unsigned nsamples )
{
    if ( ioctl( m->fd, ACQ32_IOS_VALIDATE_BIGBUF, nsamples ) != 0 ){
        perror( "ioctl ACQ32_IOS_VALIDATE_BIGBUF" );
        exit( 1 );
    }
}
void Mailboxes::maskInts( unsigned mask )
{
    if ( ioctl( m->fd, ACQ32_IOS_INTS_DISABLE, 0xffff ) != 0 ){
        perror( "ioctl ACQ32_IOS_INTS_DISABLE" );
        exit( 1 );
    }
}
void Mailboxes::enableInts( unsigned mask )
{
    if ( ioctl( m->fd, ACQ32_IOS_INTS_ENABLE, 0xffff ) != 0 ){
        perror( "ioctl ACQ32_IOS_INTS_ENABLE" );
        exit( 1 );
    }
}
Mailboxes::Mailboxes( int iboard ) {
    m = mmapMbox( iboard );
    get( 0 );
    get( 1 );
    get( 2 );
    get( 3 );    
}
