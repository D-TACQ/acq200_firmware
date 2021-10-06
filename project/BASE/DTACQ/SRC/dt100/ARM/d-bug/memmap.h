/*****************************************************************************
 *
 * File: acq32_memmap.h
 *
 * $RCSfile: memmap.h,v $
 * 
 * Copyright (C) 1999 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description: memory map of ACQ32
 *
 * $Id: memmap.h,v 1.17.4.2 2003/10/25 15:12:01 pgm Exp $
 * $Log: memmap.h,v $
 * Revision 1.17.4.2  2003/10/25 15:12:01  pgm
 * gutted
 *
 * Revision 1.17.4.1  2003/08/29 17:41:03  pgm
 * init pci command handling
 *
 * Revision 1.17  2002/08/21 09:02:39  pgm
 * B1680 dhAcq12run as fast as it will get
 *
 * Revision 1.16  2002/03/21 11:54:51  pgm
 * untabify
 *
 * Revision 1.15  2002/03/13 21:43:09  pgm
 * correct formatting with emacs (Stroustrup)
 *
 * Revision 1.14  2001/11/04 23:35:10  pgm
 * host side fw debugs
 *
 * Revision 1.13  2001/05/15 17:51:01  pgm
 * new fifo mapping scheme uses wider VM, avoids AI/AO overlap
 *
 * Revision 1.12  2001/05/14 17:49:50  pgm
 * AO works well - shippit quick
 *
 * Revision 1.11  2001/05/13 20:00:46  pgm
 * thrashing around to get AO to go
 *
 * Revision 1.10  2001/05/07 11:38:38  pgm
 * DAC output - wrk in progress
 *
 * Revision 1.9  2001/03/31 20:45:13  pgm
 * DUMDMA works with debugs on ...
 *
 * Revision 1.8  2001/03/30 20:20:13  pgm
 * UDMA first cut working
 *
 * Revision 1.7  2001/03/19 23:47:22  pgm
 * updated d-bug help, nodos
 *
 * Revision 1.6  2001/03/04 21:09:29  pgm
 * LL kludge, Data Trigger 99kHz
 *
 * Revision 1.5  2001/02/17 21:25:15  pgm
 * LL kludge part 1
 *
 * Revision 1.4  2000/10/14 22:35:25  pgm
 * first pass speed up
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

#ifndef _ACQ32_MEMMAP_H_
#define _ACQ32_MEMMAP_H_


// SDRAM memory areas

// sizes in LONGS

#ifdef PGMCOMOUT
#define STORE_BASE              ((unsigned*)0x00100000)
#define STORE_LEN                        ((0x00200000))


#define PCI_BUF                     ((unsigned*)0x00300000)
#define PCI_BUF_LEN                             0x00080000
#define I2O_BASE                ((unsigned*)0x00380000)
#define I2O_END                            (0x003e0000)

#define SCRATCH_PAD             ((unsigned*)0x003e0000)
#define DIAGS                               0x003f0000
#define SIOBUF                              0x003ff000    
#else
extern void* PCI_BUF;
#define PCI_BUF_LEN                             0x00080000
extern short *BIG_BUF;
extern unsigned scratch_pad_buf[];
#define SCRATCH_PAD scratch_pad_buf
extern char diags_buf[];
#define DIAGS diags_buf
#define SIOBUF (diags_buf+0xf000)
#endif

#define SCRATCH_PAD_LEN                        0x10000



#define SIOBUFLEN                           0x00001000    // 4K serial IO buf

#define EXTENDED_STORE_BASE     ((unsigned*)0x08000000)


// PGM - Quick hack ... try to separate AI read and AO write 
#define FIFO_DATA       ((unsigned*)0x06000000)
#define FIFO_DATAU                 (0x06000000)
#define FIFO_DATA_LEN              (0x01000000)     // length in bytes
#define FIFO_DATA_GUARD            (0x00100000)     // guard band in bytes
// never overrun MAX
#define FIFO_DATA_LIMIT \
    (FIFO_DATA+(FIFO_DATA_LEN-FIFO_DATA_GUARD)/sizeof(unsigned))     

#define FIFO_DATA_LIMITER    (FIFO_DATAU|0x007fff00)

#define FIFO_AO_BASE    ((unsigned*)0x07000000)
#define FIFO_DO_BASE    ((unsigned*)0x07000000)  


/*
 * some interresting facts about FIFOS
 */
 
#define FIFO_LEN_AO    256
#define FIFO_LEN_DO    256


#endif
