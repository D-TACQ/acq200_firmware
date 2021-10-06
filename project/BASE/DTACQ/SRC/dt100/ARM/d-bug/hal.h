/*****************************************************************************
 *
 * File: hal.h
 *
 * $RCSfile: hal.h,v $
 * 
 * Copyright (C) 2001 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description: D-TACQ Hardware Abstraction Layer for 21285
 *
 * $Id: hal.h,v 1.7.4.3 2003/10/19 20:44:29 pgm Exp $
 * $Log: hal.h,v $
 * Revision 1.7.4.3  2003/10/19 20:44:29  pgm
 * works with dt100rc
 *
 * Revision 1.7.4.2  2003/09/10 20:33:19  pgm
 * split File IO prep for DMA, PIO still works
 *
 * Revision 1.7.4.1  2003/09/01 08:17:37  pgm
 * i2o_init_buffes() hook
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

#ifndef __HAL_H__
#define __HAL_H__

#include "acq200_hal.h"

// the usec Timer returns elapsed time in usecs.
// This value rolls over every 2**24/(66e6/16) secs = 4 secs

void usecTimerInit( void );
unsigned usecTimerGetValue( void );

unsigned usecTimerDelta( unsigned t1, unsigned t2 ); 
// delta acocunts for one rollover

//void usleep( unsigned usecs );
// sleep on usec timer

unsigned qusecTimerGetValue( void );
unsigned qusecTimerDelta( unsigned t1, unsigned t2 ); 
// ditto in quarter usecs

// the msec timer rolls over every 2**24/(66e6/256) secs = 64 secs

unsigned msecTimerGetValue( void );
unsigned msecTimerDelta( unsigned t1, unsigned t2 );

extern void memcpy4( unsigned* dst, unsigned* src, int nbytes );

extern void* pci2vma( u32 replyAddr );
// memcpy on quad word aligned data


#endif
