/*****************************************************************************
 *
 * File: diags.h
 *
 * $RCSfile: diags.h,v $
 * 
 * Copyright (C) 1999 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description: interface to diagnostics
 *
 * $Id: diags.h,v 1.14 2002/08/21 09:02:39 pgm Exp $
 * $Log: diags.h,v $
 * Revision 1.14  2002/08/21 09:02:39  pgm
 * B1680 dhAcq12run as fast as it will get
 *
 * Revision 1.13  2002/03/21 11:54:51  pgm
 * untabify
 *
 * Revision 1.12  2002/03/06 09:44:46  pgm
 * histo upstream fifo (top4 bits)
 *
 * Revision 1.11  2001/10/01 21:13:43  pgm
 * hard trigger works for AO
 *
 * Revision 1.10  2001/08/27 12:55:18  pgm
 * acq16 runs again - needs LCA bit change, caveat embedded trigger
 *
 * Revision 1.9  2001/07/11 21:17:58  pgm
 * AI fifo==32, should fix AO load probs
 *
 * Revision 1.8  2001/07/11 16:07:11  pgm
 * fix diags
 *
 * Revision 1.7  2001/07/11 15:35:56  pgm
 * status at 2*sizeof(HISTOGRAM)
 *
 * Revision 1.6  2001/06/27 22:49:42  pgm
 * better status handling, acq16 deeper fifo
 *
 * Revision 1.5  2001/06/05 19:45:12  pgm
 * meshed gut, datahandling - willit work
 *
 * Revision 1.4  2001/05/15 08:50:33  pgm
 * even task handling, slicker status, err detect
 *
 * Revision 1.3  2001/05/13 20:00:46  pgm
 * thrashing around to get AO to go
 *
 * Revision 1.2  2001/03/20 21:51:04  pgm
 * added headers
 *
 * Revision 1.1  2001/03/17 10:35:39  pgm
 * *** empty log message ***
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


#ifndef _DIAGS_H_
#define _DIAGS_H_

#ifndef _ACQ32_MEMMAP_H_
#include "memmap.h"
#endif

#define HISTO_LEN       32

typedef unsigned Histogram[HISTO_LEN];


#define NFIFO_HISTO   ((unsigned*)(DIAGS))
#define AO_HISTO      (&NFIFO_HISTO[HISTO_LEN])
#define ASSERT_STRING ((char*)&NFIFO_HISTO[2*HISTO_LEN])

static __inline void acq32_AI_histo_add( u32 fsw )
{
    NFIFO_HISTO[Q32_GET_AI_FIFOSAMPLES(fsw)]++;
}

static __inline void acq32_AO_histo_add( u32 fsw )
{

    AO_HISTO[Q32_GET_AO_FIFOSAMPLES(fsw)]++;
}


static __inline void acq16_histo_add( u32 fsw )
{
#ifdef PGMCOMOUT
    NFIFO_HISTO[fsw&Q16_DATA_FIFO_STAT_AIFIFOPTR]++;
    AO_HISTO[(fsw&0x0f00)>>8]++;                        // hack upstream fifo
#else
    NFIFO_HISTO[fsw&Q16_DATA_FIFO_STAT_AIFIFOPTR]++;
#endif    
}

extern void acq32_fifo_histo_reset( int quick );

#endif
