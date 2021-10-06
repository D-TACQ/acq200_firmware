/*****************************************************************************
 *
 * File: dmac.h
 *
 * $RCSfile: dmac.h,v $
 * 
 * Copyright (C) 2001 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description: API for DMA engine
 *
 * $Id: dmac.h,v 1.11 2002/03/21 11:54:51 pgm Exp $
 * $Log: dmac.h,v $
 * Revision 1.11  2002/03/21 11:54:51  pgm
 * untabify
 *
 * Revision 1.10  2002/03/13 21:43:09  pgm
 * correct formatting with emacs (Stroustrup)
 *
 * Revision 1.9  2001/03/20 21:51:04  pgm
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



/*
 * dmac.h - API for DMAC
 */


void dmacInit( void );

void dmacWritePci( unsigned pci_addr, unsigned local_addr, int nbytes );

void dmacWritePciPrecharge( unsigned pci_addr, unsigned local_addr, int nbytes );

void dmacWritePciTrigger( void );

#define DMA_MAX_BYTE_COUNT 0xff0000


typedef struct DMACCHAIN {
    unsigned count;                     // byte count in d23:0
    unsigned pci_addr;
    unsigned loc_addr;
    struct DMACCHAIN* next;
}
DmacChain;

// fields for count

#define CB_SDRAM_TO_PCI 0x40000000
#define CB_END_OF_CHAIN 0x80000000

void dmacWritePciChain( DmacChain* chain );

enum DMAC_STATUS {
    DMAC_IDLE, 
    DMAC_BUSY,
    DMAC_HW_ERROR = -1,
    DMAC_OUT_OF_BUFFERS_ERROR = -2
};

int dmacQueueWritePciChain(
    unsigned count, 
    unsigned pci_addr, 
    unsigned loc_addr,
    unsigned send_now
    );

// returns 0 on success, negative on error

int dmacQueueFlush( void );

enum DMAC_STATUS dmacGetStatus( void );

enum DMAC_STATUS dmacPollCompletion( unsigned poll_usecs );
