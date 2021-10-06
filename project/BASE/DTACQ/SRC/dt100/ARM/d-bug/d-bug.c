/*****************************************************************************
 *
 * File: d-bug.c
 *
 * $RCSfile: d-bug.c,v $
 * 
 * Copyright (C) 2001 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description: d-bug monitor 
 *
 * $Id: d-bug.c,v 1.134.4.25 2009/03/30 07:35:19 pgm Exp $
 * $Log: d-bug.c,v $
 * Revision 1.134.4.25  2009/03/30 07:35:19  pgm
 * B2212 - locks out commands in !=ST_STOP
 *
 * Revision 1.134.4.24  2008/07/02 11:31:39  pgm
 * drop a lot of acq32 crud
 *
 * Revision 1.134.4.23  2007/04/15 21:02:32  pgm
 * processFileDMA works again
 *
 * Revision 1.134.4.22  2007/01/03 23:16:16  pgm
 * double fork() is good
 *
 * Revision 1.134.4.21  2007/01/03 12:31:37  pgm
 * MU interrupts rock! - .7s for 100 commands.
 *
 * Revision 1.134.4.20  2007/01/02 18:24:19  pgm
 * trim from 106K to 82K
 *
 * Revision 1.134.4.19  2007/01/02 18:18:10  pgm
 * *** empty log message ***
 *
 * Revision 1.134.4.18  2005/05/16 08:49:58  pgm
 * *** empty log message ***
 *
 * Revision 1.134.4.17  2004/10/25 09:19:18  pgm
 * dma experiments - no work, no crash
 *
 * Revision 1.134.4.16  2004/09/23 21:07:29  pgm
 * B2081
 *
 * Revision 1.134.4.15  2004/02/14 18:04:33  pgm
 * split out connector stuff from d-bug to main
 *
 * Revision 1.134.4.14  2004/02/06 20:42:17  pgm
 * unwind abortive c++ expt - compiles but no libs
 *
 * Revision 1.134.4.13  2004/01/17 10:12:11  pgm
 * dbg skt vbuf good, zombie mode operates
 *
 * Revision 1.134.4.12  2003/12/17 13:29:52  pgm
 * ACQ200_PPJOB
 *
 * Revision 1.134.4.11  2003/12/12 14:19:15  pgm
 * *** empty log message ***
 *
 * Revision 1.134.4.10  2003/11/07 09:06:10  pgm
 * pre A4, post A3
 *
 * Revision 1.134.4.9  2003/11/05 22:29:57  pgm
 * hooks GATED_TRANSIENT, dio
 *
 * Revision 1.134.4.8  2003/10/25 15:12:00  pgm
 * gutted
 *
 * Revision 1.134.4.7  2003/10/19 20:44:29  pgm
 * works with dt100rc
 *
 * Revision 1.134.4.6  2003/09/13 13:52:38  pgm
 * DMA IN, PIO OUT works well enough
 *
 * Revision 1.134.4.5  2003/09/11 23:10:23  pgm
 * DMA switch hack
 *
 * Revision 1.134.4.4  2003/09/10 20:33:18  pgm
 * split File IO prep for DMA, PIO still works
 *
 * Revision 1.134.4.3  2003/09/08 12:54:20  pgm
 * i2o file download works. ship it quick
 *
 * Revision 1.134.4.2  2003/09/07 16:30:39  pgm
 * basic i2o
 *
 * Revision 1.134.4.1  2003/08/29 17:41:02  pgm
 * init pci command handling
 *
 * Revision 1.134  2002/11/01 20:49:15  pgm
 * env, invert, acq16 offset stub, dmac tests
 *
 * Revision 1.133  2002/08/21 19:49:20  pgm
 * B1689 - auto trig, sample set adjust OK??
 *
 * Revision 1.132  2002/08/21 09:02:39  pgm
 * B1680 dhAcq12run as fast as it will get
 *
 * Revision 1.131  2002/08/06 19:41:38  pgm
 * lower ICACHE load, add mbox boost
 *
 * Revision 1.130  2002/07/29 11:21:42  pgm
 * B1689 minimal acq16 AO reserve
 *
 * Revision 1.129  2002/07/29 11:03:48  pgm
 * B1688 remove acq16 AO reserve
 *
 * Revision 1.128  2002/07/25 15:13:12  pgm
 * acq16 trig comp now automatic
 *
 * Revision 1.127  2002/07/24 18:07:41  pgm
 * acq16 trigger offset comp - use 1st TW
 *
 * Revision 1.126  2002/07/22 20:01:34  pgm
 * fixes gap in pre gpem P1
 *
 * Revision 1.125  2002/07/18 18:43:05  pgm
 * 44M stub, 64b copy test
 *
 * Revision 1.124  2002/06/10 20:02:40  pgm
 * B1666 multi samples per pci packet
 *
 * Revision 1.123  2002/06/09 15:57:56  pgm
 * from handa
 *
 * Revision 1.122  2002/04/26 10:51:59  pgm
 * B1638 finally handles flash num channels correctly
 *
 * Revision 1.121  2002/03/24 11:54:54  pgm
 * B1617 - works (sometimes)
 *
 * Revision 1.120  2002/03/21 11:54:51  pgm
 * untabify
 *
 * Revision 1.119  2002/03/21 11:10:35  pgm
 * *** empty log message ***
 *
 * Revision 1.118  2002/03/13 21:43:09  pgm
 * correct formatting with emacs (Stroustrup)
 *
 * Revision 1.117  2002/03/10 12:31:42  pgm
 * no warnings
 *
 * Revision 1.116  2002/03/09 12:20:39  pgm
 * fix FS44 sink, ensure clean hw ch change
 *
 * Revision 1.115  2002/03/09 11:07:03  pgm
 * make acq16 hw channel select more complete
 *
 * Revision 1.114  2002/03/06 09:43:43  pgm
 * improve md display
 *
 * Revision 1.113  2002/02/19 19:32:10  pgm
 * clean up llcontrol and make it go forever
 *
 * Revision 1.112  2002/02/16 07:40:49  pgm
 * attempting to make edge trig faster
 *
 * Revision 1.111  2002/02/11 22:06:08  pgm
 * DO is GO
 *
 * Revision 1.110  2002/02/10 17:56:50  pgm
 * usecs timing reliable over time
 *
 * Revision 1.109  2002/02/09 21:32:11  pgm
 * work on acq16 2/4/8/16 (boots)
 *
 * Revision 1.108  2002/02/09 10:16:49  pgm
 * more moving static inits till after lca load
 *
 * Revision 1.107  2002/02/09 09:53:55  pgm
 * set channels after lca init
 *
 * Revision 1.106  2002/02/09 09:31:05  pgm
 * war on warnings
 *
 * Revision 1.105  2002/02/08 22:45:30  pgm
 *  acq16 setnum channels
 *
 * Revision 1.104  2002/02/07 16:56:35  pgm
 * update to new lca
 *
 * Revision 1.103  2002/01/18 18:33:10  pgm
 * *** empty log message ***
 *
 * Revision 1.102  2002/01/18 10:05:41  pgm
 * onwards and upwards
 *
 * Revision 1.101  2002/01/13 22:21:58  pgm
 * acq32 fast threshtrig kludge coded
 *
 * Revision 1.100  2002/01/05 21:48:26  pgm
 * fixes DUMDMA stride probs
 *
 * Revision 1.98  2001/12/04 21:28:53  pgm
 * full rate threshtrig in switchable sink
 *
 * Revision 1.97  2001/11/23 22:28:07  pgm
 * fixes AO ST, direct dumdma speed demo
 *
 * Revision 1.96  2001/11/20 22:23:53  pgm
 * better dregs, i2o in capture, fix AO soft trig
 *
 * Revision 1.95  2001/11/05 22:28:40  pgm
 * TEST messages, fix gut buffer overrun bug
 *
 * Revision 1.94  2001/11/04 23:35:10  pgm 
 * host side fw debugs
 *
 * Revision 1.93  2001/10/22 20:00:17  pgm
 * Build 1400 edge trig works rel to Scalar
 *
 * Revision 1.92  2001/10/07 10:19:59  pgm
 * GPME looking good build 1370 checkin
 *
 * Revision 1.91  2001/10/06 21:03:07  pgm
 * CPEM with PXI trig works, AI overflow to cope with
 *
 * Revision 1.90  2001/10/01 21:31:46  pgm
 
 * init i2o-dump
 *
 * Revision 1.89  2001/09/30 18:38:00  pgm
 * i2o two commands still
 *
 * Revision 1.88  2001/09/30 14:08:20  pgm
 * i2o boot time - two commands
 *
 * Revision 1.87  2001/09/08 13:54:36  pgm
 * Build 1333 - fixes E1 overrun, opt i2o incoming
 *
 * Revision 1.86  2001/08/18 07:31:24  pgm
 * tidy debug, it runs @250 ksps
 *
 * Revision 1.85  2001/08/17 15:20:11  pgm
 * fail gracefully with duff LCA
 *
 * Revision 1.84  2001/07/12 10:16:48  pgm
 * take more care over lca init order
 *
 * Revision 1.83  2001/07/11 14:52:35  pgm
 * build 1265 in cvs, ++TLB flush done
 *
 * Revision 1.82  2001/06/27 22:49:41  pgm
 * better status handling, acq16 deeper fifo
 *
 * Revision 1.81  2001/06/23 20:21:07  pgm
 * data fills to end
 *
 * Revision 1.80  2001/06/23 19:03:36  pgm
 * acq16 part 1 captures runs
 *
 * Revision 1.79  2001/06/13 20:44:30  pgm
 * lca-ident, fix pre- channel swap bug
 *
 * Revision 1.78  2001/06/11 14:47:25  pgm
 * GUT ran thru
 *
 * Revision 1.77  2001/06/09 20:28:59  pgm
 * GUT - in testing
 *
 * Revision 1.76  2001/06/09 11:56:28  pgm
 * GUT: lets get to testing
 *
 * Revision 1.75  2001/06/08 20:02:01  pgm
 * GUT builds, fifo_sink testing
 *
 * Revision 1.74  2001/06/02 08:43:55  pgm
 * reserve AO, GUT STUBS
 *
 * Revision 1.73  2001/05/29 19:51:24  pgm
 * fixed cal, DACpot
 *
 * Revision 1.72  2001/05/13 20:00:46  pgm
 * thrashing around to get AO to go
 *
 * Revision 1.71  2001/05/09 07:46:59  pgm
 * more adds to Wavegen, EmbTrig
 *
 * Revision 1.70  2001/05/08 20:18:14  pgm
 * first cut wavegen in acq32_run() - compiles
 *
 * Revision 1.69  2001/05/07 19:49:11  pgm
 * loads waveforms, uses storeman OK
 *
 * Revision 1.68  2001/05/07 14:23:01  pgm
 * storeman in
 *
 * Revision 1.67  2001/05/07 11:38:37  pgm
 * DAC output - wrk in progress
 *
 * Revision 1.66  2001/05/05 08:58:37  pgm
 * ext clk works, better debug <dr>, ldcal, channel ass correct
 *
 * Revision 1.65  2001/05/04 20:37:07  pgm
 * big upgrade to improve qwual, match spec
 *
 * Revision 1.64  2001/05/04 09:32:16  pgm
 * add data trig control
 *
 * Revision 1.63  2001/05/01 20:59:23  pgm
 * first pass em trig runs - runs away in fact
 *
 * Revision 1.62  2001/04/19 19:55:25  pgm
 * mbox cal ops, physchan mask
 *
 * Revision 1.61  2001/04/18 20:39:09  pgm
 * built in multi cal facility - needs busprot
 *
 * Revision 1.60  2001/04/16 15:33:52  pgm
 * works with LCA2
 *
 * Revision 1.59  2001/04/15 09:52:26  pgm
 * SyncSetRoute works
 *
 * Revision 1.58  2001/04/14 20:37:50  pgm
 * CPCI routing coded, needs testing
 *
 * Revision 1.57  2001/04/14 19:06:13  pgm
 * remvoe WORTODOS - new LCA is GO
 *
 * Revision 1.56  2001/04/13 20:05:20  pgm
 * new LCA - compiled, not tested, must check WORKTODOS
 *
 * Revision 1.55  2001/04/09 09:58:49  pgm
 * deep grief, distributor bug fixed (testing..)
 *
 * Revision 1.54  2001/04/01 08:25:55  pgm
 * DUMDMA works - synch on post fixes
 *
 * Revision 1.53  2001/03/25 19:20:28  pgm
 * first cut i2o command handling
 *
 * Revision 1.52  2001/03/25 10:24:45  pgm
 * incoming i2o works with cache disabled. outgoing trahsed :-(
 *
 * Revision 1.51  2001/03/24 20:47:17  pgm
 * rejig I2O, add incoming hooks (outgoing still works)
 *
 * Revision 1.50  2001/03/20 22:12:53  pgm
 * *** empty log message ***
 *
 * Revision 1.49  2001/03/20 21:51:04  pgm
 * added headers
 *
 *
\*****************************************************************************/



/*
 * d-bug.c - dtacq debug monitor
 *
 * basic commands are:
 *
 * md[.lwb] a1 [a2]   : display data {default:l}
 *
 * mm[.lwb] a1[=] [v] : modify memory
 * mm a1              :    modify in sequence until quit <ctrl-D>
 * mm a1 v            :    modify one address
 * mm a1=             ;    modify same address repeatedly
 *
 * bf[.lwb] a1 a2 v1 [v2] : block fill
 * mmucr [value]      ; read [write] mmu control reg
 *
 * RESTRICTIONS: only .l version available in rev 1
 */


#ifdef ARM
#error ARM was here 
#endif



#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <setjmp.h>

#include "local.h"

#include "ctype.h"

#include "dt100api.h"

#include "acq32busprot.h"

#include "acq32_drv.h"
#include "acq32_hml.h"
#include "acq32cpci_drv.h"

#include "dataHandling.h"

#include "mbox_slave.h"
#include "stream.h"

#include "dlog.h"

#include "hal.h"

#include "../cal-image/cal-image.h"

#include "processMessage.h"

// conversion: 0x means hex, else decimal

struct accessor {
    u32 (*peek)( u32 addr );    
    void (*poke)( u32 addr, u32 value );
    void (*print)( u32 addr, u32 value, int new_start );
    int size;
};

static int S_repeat;



static void print32( u32 addr, u32 value, int new_start )
{
    static int iprint;

    if ( new_start ){
        iprint = 0;
    }

    if ( iprint%8 == 0 ) printf( "%08x:", addr );

    printf( "%08x%c", value, ++iprint%8==0? '\n': ' ' );
}



static u32 mem_peek32( u32 addr )
{
    return *(u32*)addr;
}

static void mem_poke32( u32 addr, u32 value )
{
    *(u32*)addr = value;
}


static struct accessor mem_acc32 = {
    mem_peek32,
    mem_poke32,
    print32,
    sizeof( u32 )
};

static u32 mem_peek8( u32 addr )
{
    return (u32)*(u8*)addr;
}

static void mem_poke8( u32 addr, u32 value )
{
    *(u8*)addr = (u8)value;
}

static void print8( u32 addr, u32 value, int new_start )
{
    static int iprint;
    static char print_chars[17];

    if ( new_start ){
        iprint = 0;
            print_chars[0] = '\0';
    }

    if ( iprint%16 == 0 ) printf( "0x%08x:", addr );

    printf( "%02x ", value );
    print_chars[iprint%16] = IN_RANGE( value, ' ', 'z' ) ? value: '.';

    if ( ++iprint%16 == 0 ) printf( " %s\n", print_chars );
}

static struct accessor mem_acc8 = { mem_peek8, mem_poke8, print8, 1 };


static u32 mem_peek16( u32 addr )
{
    return (u32)*(u16*)addr;
}
static void mem_poke16( u32 addr, u32 value )
{
    *(u16*)addr = (u16)value;
}
static void print16( u32 addr, u32 value, int new_start )
{
    static int iprint;

    if ( new_start ) iprint = 0;

    if ( iprint%8 == 0 ) printf( "0x%08x:", addr );

    printf( "%04x ", value );

    if ( ++iprint%8 == 0 ) printf( "\n" );
}
static struct accessor mem_acc16 = { mem_peek16, mem_poke16, print16, 2 };


static struct accessor* acc =   &mem_acc32;


static void fix_size( char cmd[], char* match )
{
    if ( STREQNL( cmd, match ) ){
        switch( cmd[3] ){
        default:
        case 'l':
            acc = &mem_acc32;
            break;
        case 'b':
            acc = &mem_acc8;
            break;
        case 'w':
            acc = &mem_acc16;
            break;
        }
    }
}

static void dump( u32 a1, u32 a2 )
{
    u32 value;
    int new_start = 1;

    for ( ; a1 < a2; a1 += acc->size ){
        value = acc->peek( a1 );
        acc->print( a1, value, new_start );
        new_start = 0;
    }
}

static struct {
    u32 a1, a2;
    u32 delta;
}
md_defs;    

static void init_md_defs( void )
{
    md_defs.a1 = 0;
    md_defs.delta = 256;
    md_defs.a2 = 256;
}
static void memory_display( int argc, char* argv[] )
{
    
    if ( S_repeat ){
        md_defs.a1 = md_defs.a2;
        md_defs.a2 = md_defs.a1 + md_defs.delta;
    }else{
        fix_size( argv[1], "md." );

        switch( argc ){
        case 1:                                 /* proc [cmd] */
        case 2:                                 /* proc cmd */
            md_defs.a2 =  md_defs.a1 + md_defs.delta;
            break;
        case 3:                                 /* proc cmd a1 */
            md_defs.a1 = ATOL( argv[2] );
            md_defs.a2 = md_defs.a1 + md_defs.delta;
            break;
        case 4:                                /* proc cmd a1 a2 */
            md_defs.a1 = ATOL( argv[2] );
            md_defs.a2 = ATOL( argv[3] );
            md_defs.delta = md_defs.a2 - md_defs.a1;
            break;
        default:
            printf( "d-bug: ERROR bad number of args\n" );
            return;
        }
    }

    // prevent annoying runaways, by brute force
    
    if ( md_defs.delta > 512 ) md_defs.delta = 512;
    if ( md_defs.a1 > md_defs.a2 ) md_defs.a2 = md_defs.a1;
    
    md_defs.a2 = md_defs.a1 + md_defs.delta;
    
    dump( md_defs.a1, md_defs.a2 );
}
static void memory_modify( int argc, char* argv[] )
{
    u32 a1, v1;

    fix_size( argv[1], "mm." );

    switch( argc ){
    case 4:                     /* proc cmd a1 v1 */
        a1 = ATOL( argv[2] );
        v1 = ATOL( argv[3] ); 
        acc->poke( a1, v1 );
        break;
    case 3:
    {
        int increment = strchr( argv[2], '=' )? 0: acc->size;
        char command[20];
        int finished = 0;

        a1 = ATOL( argv[2] );

        do {
            v1 = acc->peek( a1 );
            acc->print( a1, v1, 1 );
            printf( " ?" );
 
            if ( fgets( command, 20, stdin ) == 0 || command[0] == '\n' ){
                printf( "\n" );
            }else if ( command[0] == '.' ){
                finished = 1;
            }else{
                v1 = ATOL( command );
                acc->poke( a1, v1 );
            }
            a1 += increment;
        }while( !finished );
    }
    break;
    default:
        printf( "d-bug: ERROR wrong number of args\n" );
    }
}
static void block_fill( int argc, char* argv[] )
{
    u32 a1, a2;
    u32 v1 = 0, v2 = 0;

    fix_size( argv[1], "bf." );

    switch( argc ){
    case 6:                   /* proc cmd a1 a2 v1 v2 */
        v2 = ATOL( argv[5] ); /* fall thru */
    case 5:
        v1 = ATOL( argv[4] ); /* fall thru */   
    case 4:
        a2 = ATOL( argv[3] );
        a1 = ATOL( argv[2] );
        break;
    default:
        printf( "d-bug: ERROR bad number of args\n" );
        return;
    }

    {
        u32 aa;
        u32 vv = v1;

        for ( aa = a1; aa <= a2; aa+= acc->size ){
            acc->poke( aa, vv );
            
            if ( ++vv > v2 ){
                vv = v1;
            }
        }
    }
}

static void block_check( int argc, char* argv[] )
{
    u32 a1, a2;
    u32 v1 = 0, v2 = 0;
    int nerrors = 0;

    fix_size( argv[1], "bc." );

    switch( argc ){
    case 6:                   /* proc cmd a1 a2 v1 v2 */
        v2 = ATOL( argv[5] ); /* fall thru */
    case 5:
        v1 = ATOL( argv[4] ); /* fall thru */   
    case 4:
        a2 = ATOL( argv[3] );
        a1 = ATOL( argv[2] );
        break;
    default:
        printf( "d-bug: ERROR bad number of args\n" );
        return;
    }

    {
        u32 aa;
        u32 vv = v1;
        u32 vpeek;

        for ( aa = a1; aa <= a2; aa+= acc->size ){
            if ( (vpeek = acc->peek( aa )) != vv ){
                if ( ++nerrors < 10 ){
                    printf( "ERROR: wanted, got" );
                    acc->print( aa, vv, 1 );
                    acc->print( aa, vpeek, 0 );
                    printf( "\n" );
                }
            }
           
            if ( ++vv > v2 ){
                vv = v1;
            }
        }
        printf( nerrors? " ERRORS %d \n": " OK\n", nerrors );
    }
}

int P_debug = 1;

void POKEL( unsigned addr, unsigned data )
{
    unsigned rbk = *(unsigned*)addr = data;

    if ( P_debug ){
        printf( "POKEL:0x%08x =   %08x now reads %08x\n", addr, data, rbk );
    }
}

void POKEW( unsigned addr, unsigned short data )
{
    *(unsigned short*)addr = data;

    if ( P_debug ){
        printf( "POKEW:0x%08x =    %04x\n", addr, data );
    }
}
void POKEB( unsigned addr, unsigned char data )
{
    unsigned char rbk = *(unsigned char*)addr = data;

    if ( P_debug ){
        printf( "POKE :0x%08x =   %02x now reads %02x\n", addr, data, rbk );
    }
}
void SETL( unsigned addr, unsigned bits )
{
    unsigned rbk = *(unsigned*)addr;

    rbk |= bits;

    *(unsigned*)addr = rbk;

    if ( P_debug ){
        printf( "SETL :0x%08x |=  %08x now reads %08x\n", addr, bits, rbk ); 
        printf( "SETL :0x%08x |=  %08x now reads %08x\n", addr, bits, rbk );
    }
}
void CLRL( unsigned addr, unsigned bits )
{
    unsigned rbk = *(unsigned*)addr &= ~bits;

    rbk &= ~bits;

    *(unsigned*)addr = rbk;

    if ( P_debug ){
        printf( "CLRL :0x%08x &= ~%08x now reads %08x\n", addr, bits, rbk );
    }
}

unsigned PEEKL( unsigned addr )
{
    unsigned peekl = *(unsigned*)addr;

    if ( P_debug ){
        printf( "PEEKL: 0x%08x is 0x%08x\n", addr, peekl );
    }
    return peekl;
}
unsigned char PEEKB( unsigned addr )
{
    unsigned char peekb = *(unsigned char*)addr;

    if ( P_debug ){
        printf( "PEEKB: 0x%08x is 0x%02x\n", addr, peekb );
    }
    return peekb;
}


static void init_command( int argc, char* argv[] )
{
    printf( "init_command\n" );
    P_debug = 0;
}

extern void pgm_speed_debug( int );

static void bug_init( void )
{
    mem_acc32.peek = mem_peek32;
    mem_acc32.poke = mem_poke32;
    mem_acc32.print= print32;
    mem_acc32.size = 4;
    acc = &mem_acc32;

    mem_acc16.peek = mem_peek16;
    mem_acc16.poke = mem_poke16;
    mem_acc16.print= print16;
    mem_acc16.size = 2;

    mem_acc8.peek = mem_peek8;
    mem_acc8.poke = mem_poke8;
    mem_acc8.print= print8;
    mem_acc8.size = 1;
    init_md_defs();
}

extern int usc_debug;
extern int G_command_log;
extern int new_state_debug;       
extern int ls_debug;

struct DebugTable {
	const char* key;
	int *var;
};

static void _setDebug(const char *key, int value)
{
	static struct DebugTable dt[] = {
		{ "acq200_debug", &acq200_debug },
		{ "P_debug", &P_debug },
		{ "usc_debug", &usc_debug },
		{ "G_command_log", &G_command_log },
		{ "new_state_debug", &new_state_debug },
		{ "ls_debug", &ls_debug },
		{ 0, 0 }
	};
	int ivar;

	if (strcmp(key, "help") == 0){
		for (ivar = 0; dt[ivar].key != 0; ++ivar){
			printf("%s\n", dt[ivar].key);
		}
		return;
	}else{
		for (ivar = 0; dt[ivar].key != 0; ++ivar){
			if (strcmp(dt[ivar].key, key) == 0){
				printf("setting %s to %d\n", key, value);
				*dt[ivar].var = value;
				return;
			}
		}
		printf("ERROR: failed to set %s\n", key);
	}

	
}

static void setDebug( int argc, char* argv[] )
{
	printf("setDebug %d\n", argc);
	switch( argc ){
	case 4:
		_setDebug(argv[2], atoi(argv[3]));
		return;
	default:
		printf("setDebug key value (try help 0)\n");
	}
}

extern int G_mbto;

static void setMbto( int argc, char* argv[] )
{
	printf("setMtbo %d\n", argc);
	switch( argc ){
	case 3:
		G_mbto = atoi( argv[2] );
		printf( "G_mbto set %d\n", G_mbto);
		break;
	default:
		printf( "G_mbto is %d\n", G_mbto);
	}
}

void acq32_unmangle( int nsamples )
{
    unsigned* src = (unsigned*)0x00100000;
    unsigned* dst = (unsigned*)0x00200000;

    unsigned isample, icopy;

    for( isample = 0; isample != nsamples; ++isample ){
  
        // copy from FIFOS to well known address

        for ( icopy = 0; icopy != 16; ++icopy ){           
            unsigned ilog = ((icopy&3)<<2) + ((icopy&0xc)>>2);

            dst[ilog] = *src++;
        }
        dst += 16;
    }
}


int G_dump_i2o;

void i2o_dump( char* pmessage )
{
    printf( "message at %p\n", pmessage );
    acc = &mem_acc8;
    dump( (u32)pmessage, (u32)(pmessage+40) );
}

extern void i2o_set_device_state( int state );





extern void acq200_setTransferModeDma(int);








static void doStream( int argc, char* argv[] )
{
    int nmessages = 0;
    int imessage;
    Message* message;

    switch( argc ){
    case 4:
        PRINTF( "not priming free list\n" );
        nmessages = ATOL( argv[2] );
        break;
    case 3:
        nmessages = ATOL( argv[2] );    // fall thru
    default:
        if ( nmessages ){
            i2o_initOutboundHostPull( nmessages );
        }else{
            i2o_showOutboundStatus();
        }
    }

    

    for ( imessage = 0; imessage != nmessages; ++imessage ){
        if ( (message = i2o_getFreeOutboundMessage()) != (Message*)0 ){
            PRINTF( "doStream: posting message %d\n", imessage );
            sprintf( message->payload.cdata, 
				     "message %d 0x%04x", 
                     imessage, imessage );
            message->header.id = imessage;
            i2o_postOutboundMessage( message );
        }else{
            PRINTF( "doStream: no free messages, quit\n" );
            return;
        }
    }
}

static void doDlog( int argc, char* argv[] )
{
    switch( argc ){
    case 3:
        printf( "doDlog() %s\n", ATOL( argv[2] )? "ENABLE":"DISABLE" );
        dlogEnable( ATOL( argv[2] ) );
        dlogPrintf( "!dlog Is Enabled!\n" );            
        break;
    default:
        dlogDump();
    }
}

extern int G_command_log;

static void doClog( int argc, char* argv[] )
{
	switch( argc ){
	case 3:
		printf( "doClog() %s\n", argv[2] );
		G_command_log = atoi(argv[2]);
		break;
	default:
		dlogDump();
	}
}



static void thrash( int argc, char* argv[] )
{
    int ithrash;
    
    int v1 = 1;
    int v2 = 0x04000000;

    int x1, x2;

    switch( argc ){
    case 6:                   /* proc cmd a1 a2 v1 v2 */
        v2 = ATOL( argv[5] ); /* fall thru */
    case 5:
        v1 = ATOL( argv[4] ); /* fall thru */   
        break;
    default:
        printf( "d-bug: ERROR bad number of args\n" );
        return;
    }

    x1 = v1;
    x2 = v2;

    for ( ithrash = 0; !kb_hit(); ++ithrash ){
        printf( "Testing %d 0x%08x to 0x%08x", ithrash, x1, x2 );
        block_fill( argc, argv );
        block_check( argc, argv );
        
        if ( (x2 <<= 1) < (x1 <<= 1) ){
            x1 = v1;
            x2 = v2;
        }
        sprintf( argv[4], "0x%08x", x1 );
        sprintf( argv[5], "0x%08x", x2 );
    }
}

void spin( unsigned msecs )
// spine for msecs, hopefully from cache
{
    volatile unsigned ii, jj;

    ii = msecs;

    for ( ; ii--; )
        for ( jj = 0x00ffffU; jj--; )
            ;
}

void uspin( unsigned usecs )
// spin for usecs, hopefully from cache
{
    volatile int jj;

    for ( jj = usecs*20 - 12; jj--; )
        ;
}

static void doTimerTest( void )
{
    unsigned previous = usecTimerGetValue();
    unsigned current;
    int delta;
    int ii;
//   80 chars, 1msec/char = 80000 usecs /line approx
#define USECS_PER_CHAR ((8+1)*(1000000/9600))
#define USECS_PER_LINE (80*USECS_PER_CHAR)
    int usecs_per_line = USECS_PER_LINE;
    int nprint;

        
    for ( ii = 0; ii != 10; ++ii ){
        current = usecTimerGetValue();
        delta = current - previous;
        delta = MAX( 0, delta );

        nprint = printf( 
//   123456789012345678901234   012345678   4 5678901    78  45678 
//   80 chars, 1msec/char = 80000 usecs /line approx

            "doTimerTest() curr 0x%06x prev 0x%06x delta 0x%06x %6d us %6d\n",
            current, previous, delta, delta, usecs_per_line );
        previous = current;

        usecs_per_line = nprint*USECS_PER_CHAR;
    }
}


static void doFifoReset( int argc, char*argv[] )
{
    acq32_fifoReset();
}



static void doTriggerTest( void ) 
{
    int trigger_is_on = acq32_trigger_is_on();
    int clock_is_hi   = acq32_clock_is_hi();
    int clock_running = 0;

    printf( "Trigger test - hit any key to quit\n" );
    printf( "Trigger %s clock %s\n", 
            trigger_is_on? "ON": "OFF", 
            clock_is_hi? "HI": "LO" );

    while( !kb_hit() ){
        if ( clock_is_hi != acq32_clock_is_hi() ){
            if ( !clock_running ){
                printf( "Clock running\n" );
                clock_running = 1;
            }
        }
        if ( acq32_trigger_is_on() != trigger_is_on ){
            trigger_is_on = !trigger_is_on;
            clock_is_hi = acq32_clock_is_hi();
            printf( "Trigger %s clock %s\n", 
                    trigger_is_on? "ON": "OFF", 
                    clock_is_hi? "HI": "LO" );
        }               
    }
}



typedef void (*CmdFn)( int argc, char* argv[] );

struct command_def {
    char* key;
    char *prams;
    char* help;
    char* fname;
    CmdFn the_command;
};

/*
 * build a command table with NO static init
 */
#define MAXCOMMANDS 100

static struct command_def command_def_tab[MAXCOMMANDS];
static int NCOMMANDS_ACTUAL;


void clearScreen( int argc, char* argv[] );

static void doHelp( int argc, char* argv[] ) 
{
    int print_all = 0;
    int print_fun = 0;
    int ii;
   
    while ( argc-- >= 3 ){
        if ( STREQNL( argv[argc], "all" ) ){
            print_all = 1;
        }else if ( STREQNL( argv[argc], "fun" ) ) {
            print_fun = 1;
        }
    }
        
    printf ( "$Id: d-bug.c,v 1.134.4.25 2009/03/30 07:35:19 pgm Exp $\n\n" );
    
    for ( ii = 0; ii != NCOMMANDS_ACTUAL; ++ii ){
        printf( "%-13s%-35s%s\n", 
                command_def_tab[ii].key, 
                command_def_tab[ii].prams,
                print_all? command_def_tab[ii].help:
                print_fun? command_def_tab[ii].fname: ""   );                       
    }

}

static void build_command_tab() 
{
#define ADD( KEY, PRAMS, HELP, FN) \
    command_def_tab[NCOMMANDS_ACTUAL].key = KEY; \
    command_def_tab[NCOMMANDS_ACTUAL].prams = PRAMS; \
    command_def_tab[NCOMMANDS_ACTUAL].help = HELP; \
    command_def_tab[NCOMMANDS_ACTUAL].fname = #FN; \
    command_def_tab[NCOMMANDS_ACTUAL].the_command = FN; \
    ARM_ASSERT( ++NCOMMANDS_ACTUAL < MAXCOMMANDS )
    
    NCOMMANDS_ACTUAL = 0;

    ADD("mbto", "nnn", "mail box timeout usecs", setMbto);    
    ADD( "md",     "[.s] [a1] [a2]", "memory display s = { b,w,l }",
         memory_display );
    ADD( "mm",     "[.s] a1[=][v]",  "memory modify",
         memory_modify);
    ADD( "bf",     "[.s] a1 a2 v1 [v2] ", "block fill",
         block_fill);
    ADD( "bc",     "[.s] a1 a2 v1 [v2] ", "block check",
         block_check);
    ADD( "init",   "",                "restore init state",
         init_command );
    ADD( "debug",  "[n]",              "get [set] debug level",
         setDebug );
    ADD( "thrash",  "a1 ar v1 v2 ",    "memory thrash test",
         thrash );
    ADD( "stream",   "",               "- start a streaming test ??",
         (CmdFn)doStream);
    ADD( "clog",     "{0|1}",          "- command logging enable",
	 (CmdFn)doClog);
    ADD( "dlog",     "[enable]",       "dump [enable] dlog",
         doDlog );
    ADD( "fiforeset", "", "reset the fifos",
         doFifoReset );
    ADD( "timer",    "", "timer test",
         (CmdFn)doTimerTest );
    ADD( "trigger",  "", "trigger monitor",
         (CmdFn)doTriggerTest );
    ADD( "clr", "", "clear screen", clearScreen );
    ADD( "h", "[all][func]", "help on commands ...",
         doHelp );
	
}
    
static int process_command( int argc, char* argv[] )
{
	int finished = 0;


	int ii;

	if (strlen(argv[1]) == 0){
		return 0;
	}
	for ( ii = 0; ii != NCOMMANDS_ACTUAL; ++ii ){
		if ( STREQ( argv[1], command_def_tab[ii].key ) ){
			command_def_tab[ii].the_command( argc, argv );
			return 0;
		}
	}
    
	for ( ii = 0; ii != NCOMMANDS_ACTUAL; ++ii ){
		if ( STREQNL( argv[1], command_def_tab[ii].key ) ){
			command_def_tab[ii].the_command( argc, argv );
			return 0;
		}
	}

	
	if ( STREQNL( argv[1], "angel" ) || STREQNL( argv[1], "quit" ) ){
		return finished = 1;
	}else{
		printf( "d-bug: ERROR command \"%s\" not recognised\n", 
			argv[1] );
	}

	return 0;
}

#define MAXARGS 6

static char argv[MAXARGS][40];
static char* _argv[MAXARGS];
static int argc = 0;

int d_bug_line( char* the_line )
{
    S_repeat = 0;
    strcpy( argv[0], "d-bug>" );

    if ( the_line[0] == 'q' || the_line[0] == '\x4' ){ // quit on q or ctrlD
        return 1;
    }else if ( strlen( the_line ) > 1 ){
        argc = sscanf( the_line, "%s %s %s %s %s", 
                       argv[1], argv[2], argv[3], argv[4], argv[5] );
        argc = argc >= 0? argc+1: 1;
    }else{
        S_repeat = 1;
    }
    // else reuse previous argc

    if ( process_command( argc, _argv ) ){
        printf( "starting angel ...\n\r" );
        return 1;
    }else{
        printf( "d-bug>" ); fflush( stdout );
    }

    return 0;
}

int d_bug()
{
    char the_line[81];
    int iline;
    int rc;
    
    argc = 0;
    printf( "\nd-bug>" ); fflush( stdout );

    for( iline = 0; fgets( the_line, 80, stdin ); ++iline ){
        if ( (rc = d_bug_line( the_line )) != 0 ){
            return rc;
        }else{
		;
        }
    }

    return 0;
}

void pgm_demon()
{
    while( !d_bug() )
        ;
}

void pgmDemonOnce(char *buf)
{
	d_bug_line(buf);
}
/*
 * Initialization
 */

extern void cal_init_statics( void );
  
static void static_init()
/* 
 * initialises all the statics that would normally be initialised
 * by cstart in a fully functional C environment.
 * NB - the LCA HAS NOT YET BEEN LOADED!
 */
{
    G_dump_i2o = 0;
    
    {
        int ii;

        for ( ii = 0; ii != MAXARGS; ++ii ){
            _argv[ii] = argv[ii];
        }
        argc = 0;
    }
   
    bug_init(); 
    cal_init_statics();    
    usecTimerInit();

    build_command_tab();
    i2o_static_init();
    mboxProcessI2OIncoming( MBOXI2OIN_INIT );
    mboxProcessI2OIncoming( MBOXI2OIN_ENABLE );
    acq32_setCommandInterruptAck( 0 );
    P_debug = 0;
}


extern int _end;


static int siobuf_append( const char* buf, int nbuf )
// maintain a buffer in pci mapped memory for debug
// we don't care if it overflows or not.
// first 32 bits is the put pointer
{
#define PPUTP    (char**)SIOBUF

    if ( buf == NULL ){
        // clear buffer
        *PPUTP = (char*)SIOBUF + 4;
	    return 0;    
    }
    if ( (char*)SIOBUF == *PPUTP ){
        *PPUTP += 4;
    }

    while ( nbuf-- > 0 ){
        **PPUTP = *buf++;
        *PPUTP += 1;
        if ( *PPUTP - (char*)SIOBUF >= SIOBUFLEN ){
            *PPUTP = (char*)SIOBUF + 4;
        }
    }
    return 0;
}

void clearScreen( int argc, char* argv[] )
{
    int iline;
    
    for ( iline = 0; iline != 24; ++iline ){
        printf( "\n" );
    }
    siobuf_append( NULL, 0 );
}
#include <stdarg.h>             /* ANSI varargs support */



static int no_tty;

void pgm_speed_debug( int enable )
{
    no_tty = enable? 0xcafebabe: 0;
}


void pgm_init()
/* MAIN ENTRY point - frigged from deep within dubious ANGEL bootstrap */
{
    static_init();
}
