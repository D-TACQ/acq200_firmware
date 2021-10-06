/*****************************************************************************
 *
 * File:
 *
 * $RCSfile: hal.c,v $
 * 
 * Copyright (C) 2001 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: hal.c,v 1.11.4.2 2003/10/25 15:12:01 pgm Exp $
 * $Log: hal.c,v $
 * Revision 1.11.4.2  2003/10/25 15:12:01  pgm
 * gutted
 *
 * Revision 1.11.4.1  2003/08/29 17:41:02  pgm
 * init pci command handling
 *
 * Revision 1.11  2002/03/21 11:54:51  pgm
 * untabify
 *
 * Revision 1.10  2002/03/13 21:43:09  pgm
 * correct formatting with emacs (Stroustrup)
 *
 * Revision 1.9  2002/02/10 17:56:51  pgm
 * usecs timing reliable over time
 *
 * Revision 1.8  2002/02/02 15:13:41  pgm
 * usec timing changes from EPFL visit 01/2002
 *
 * Revision 1.7  2001/09/30 14:08:20  pgm
 * i2o boot time - two commands
 *
 * Revision 1.6  2001/05/29 19:51:25  pgm
 * fixed cal, DACpot
 *
 * Revision 1.5  2001/03/20 21:51:04  pgm
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



// hal.c ... D-TACQ Hardware Abstraction Layer for 21285

#include <stdio.h>

#include "local.h"
#include "memmap.h"

#include "REGDefs.h"

#include "hal.h"

#define T1_LOAD         (*(R32*)(CSR_BASE+TIMER_1_LOAD))        // 0x300
#define T1_VALUE        (*(R32*)(CSR_BASE+TIMER_1_VALUE))   // 0x304
#define T1_CONTROL      (*(R32*)(CSR_BASE+TIMER_1_CONTROL)) // 0x308
#define T1_CLEAR        (*(R32*)(CSR_BASE+TIMER_1_CLEAR))       // 0x30C

#define T2_LOAD         (*(R32*)(CSR_BASE+TIMER_2_LOAD))        // 0x300
#define T2_VALUE        (*(R32*)(CSR_BASE+TIMER_2_VALUE))   // 0x304
#define T2_CONTROL      (*(R32*)(CSR_BASE+TIMER_2_CONTROL)) // 0x308
#define T2_CLEAR        (*(R32*)(CSR_BASE+TIMER_2_CLEAR))       // 0x30C



#define TC_SOURCE_MASK  0x0000000c
#define TC_SOURCE_FC1   0x00000000
#define TC_SOURCE_FC16  0x00000004
#define TC_SOURCE_FC256 0x00000008
#define TC_SOURCE_EXT   0x0000000c

#define TC_MODE_PERIODIC        0x00000040
#define TC_MODE_FREERUN         ~TC_MODE_PERIODIC

#define TC_ENABLE                       0x00000080

unsigned usecs_of;        // rolls over in 18 minutes
unsigned prevcount;

void usecTimerInit( void )
{
    T1_LOAD = 0;
    T1_CONTROL  = TC_ENABLE|TC_SOURCE_FC16;
        
    usecs_of = 0;
    prevcount = 0;

    // init the msec timer also

    T2_LOAD = 0;
    T2_CONTROL = TC_ENABLE|TC_SOURCE_FC256;
}
unsigned usecTimerGetValue( void )
// time rolls over in 2**24/4 = 4.2 sec
{
#define USECS_OF    (0x01000000*16)/66;
    unsigned counts_up = 0x01000000U - T1_VALUE;

    unsigned usecs = counts_up*16;usecs /= 66;

    if ( counts_up < prevcount ){
        usecs_of += USECS_OF;
    }
    prevcount = counts_up;
    return usecs_of + usecs;            // approx usecs! (4.125 actually)
}
unsigned usecTimerDelta( unsigned t1, unsigned t2 )
// assumes one timer rollover only!
{
    if ( t2 < t1 ){ 
        unsigned rollover = 0x01000000*16;
                
        rollover /= 66;
        t2 += rollover;
    }

    return t2 - t1;
}


unsigned qusecTimerGetValue( void )
// time rolls over in 2**24/4 = 4.2 sec
{
    unsigned counts_up = 0x01000000U - T1_VALUE;

    return counts_up;
}

unsigned qusecTimerDelta( unsigned t1, unsigned t2 )
// assumes one timer rollover only!
{
    if ( t2 < t1 ){ 
        unsigned rollover = 0x01000000;
                
        t2 += rollover;
    }

    return t2 - t1;
}

unsigned msecTimerGetValue( void )
// time rolls over in 2**16 msec = 65 sec
{
    unsigned counts_up = 0x01000000U - T2_VALUE;
    return counts_up/258;               // yes 258: 66MHz/256 = 258 kHz
}

unsigned msecTimerDelta( unsigned t1, unsigned t2 )
{
    if ( t2 < t1 ) t2 += 0x01000000U/258;

    return t2 - t1;
}

#define PCI_ADDR_EXT_REG   (*(R32*)(CSR_BASE+PCI_ADDR_EXT)) 

extern void* pci2vma( u32 replyAddr )
{
    if ( replyAddr&0x80000000 ){
        PCI_ADDR_EXT_REG = 0x00008000;
    }else{
        PCI_ADDR_EXT_REG = 0;
    }
    return (void*)(replyAddr|0x80000000);
}
