/*****************************************************************************
 *
 * File: dmac.c
 *
 * $RCSfile: dmac.c,v $
 * 
 * Copyright (C) 2001 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: dmac.c,v 1.18.4.4 2004/10/14 20:12:25 pgm Exp $
 * $Log: dmac.c,v $
 * Revision 1.18.4.4  2004/10/14 20:12:25  pgm
 * no warnings
 *
 * Revision 1.18.4.3  2003/10/25 15:12:01  pgm
 * gutted
 *
 * Revision 1.18.4.2  2003/09/08 12:54:20  pgm
 * i2o file download works. ship it quick
 *
 * Revision 1.18.4.1  2003/08/29 17:41:02  pgm
 * init pci command handling
 *
 * Revision 1.18  2002/03/21 11:54:51  pgm
 * untabify
 *
 * Revision 1.17  2002/03/13 21:43:09  pgm
 * correct formatting with emacs (Stroustrup)
 *
 * Revision 1.16  2001/05/21 20:07:53  pgm
 * llc - WIP
 *
 * Revision 1.15  2001/04/01 08:25:55  pgm
 * DUMDMA works - synch on post fixes
 *
 * Revision 1.14  2001/03/30 20:20:13  pgm
 * UDMA first cut working
 *
 * Revision 1.13  2001/03/20 21:51:04  pgm
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
 * dmac.c - driver for DMAC
 */

#include <stdio.h>

#include "local.h"
#include "dt100api.h"

#include "acq32_drv.h"
#include "memmap.h"

#include "REGDefs.h"
#include "dmac.h"

#define C1_BYTE_COUNT           (*(R32*)(CSR_BASE+CHAN_1_BYTE_COUNT)) // 0x80
#define C1_PCI_ADDR                     (*(R32*)(CSR_BASE+CHAN_1_PCI_ADDR))   // 0x84
#define C1_DRAM_ADDR            (*(R32*)(CSR_BASE+CHAN_1_DRAM_ADDR))  // 0x88
#define C1_DESC_PTR                     (*(R32*)(CSR_BASE+CHAN_1_DESC_PTR))      // 0x8C
#define C1_CONTROL                      (*(R32*)(CSR_BASE+CHAN_1_CONTROL))    // 0x90

#define CC_ENABLE       0x00000001      // R/W
#define CC_TRFR_DONE 0x00000004 // WC
#define CC_ERROR                0x00000008      // WC
#define CC_DIRECT       0x00000010      // SA-110 has written values to direct regs

// read 8 Dwords at a time (same as cache fill )
#define CC_SDRAM_READ_8         0x00030000
// JWM found 16 was better
#define CC_SDRAM_READ_16        0x00040000

#define CC_SDRAM_READ           CC_SDRAM_READ_16

#define CB_SDRAM_TO_PCI 0x40000000
#define CB_END_OF_CHAIN 0x80000000

// Interburst delay - it's not obvious whether 1 or 0 is shortest delay
// safest to hedge for '1' - '0' might well be 32!
// (#7-27)
#define CB_INTERBURST_DELAY 0x01000000
/* trivial single shot impl for now. multiple buffering later! */

#define MAXCHAIN        32


void dmacInit( void )
{
#ifdef PGMCOMOUT
    S_status = DMAC_IDLE;
    C1_CONTROL = CC_TRFR_DONE|CC_SDRAM_READ|CC_ERROR|CC_DIRECT;
    S_DMACQ.inext = 0;
#endif
}


void dmacWritePciPrecharge( unsigned pci_addr, unsigned local_addr, int nbytes )
// tee up the DMAC ready to go
{
#ifdef PGMCOMOUT
    S_status = DMAC_IDLE;
    C1_CONTROL &= ~CC_ENABLE;
    C1_CONTROL |= CC_TRFR_DONE|CC_ERROR|CC_DIRECT;
    C1_BYTE_COUNT   &= ~0x00ffffff; // mask Byte Count
    C1_BYTE_COUNT       = nbytes|CB_INTERBURST_DELAY|CB_SDRAM_TO_PCI|CB_END_OF_CHAIN;
    C1_PCI_ADDR         = pci_addr;
    C1_DRAM_ADDR        = local_addr;
#endif
}

void dmacWritePciTrigger( void )
{
#ifdef PGMCOMOUT
    C1_CONTROL  |= CC_ENABLE;
#endif
}


enum DMAC_STATUS dmacGetStatus( void )
{
#ifdef PGMCOMOUT
    if ( S_status != DMAC_IDLE ){
        return S_status;
    }else{
        if ( dmacBusy() ){
            return DMAC_BUSY;
        }else if ( dmacError() ){
            return DMAC_HW_ERROR;
        }else{
            return DMAC_IDLE;
        }
    }
#else
    return DMAC_IDLE;
#endif
}

enum DMAC_STATUS dmacPollCompletion( unsigned poll_usecs )
{
#ifdef PGMCOMOUT
    enum DMAC_STATUS status;
    int ipoll = 0;

    while ( (status = dmacGetStatus()) == DMAC_BUSY ){
        if ( poll_usecs ){
            uspin( poll_usecs );
        }
        if ( (++ipoll&0xffff) == 0 ){
            PRINTFL(2)( FN " polling ...\n" );
        }
    }
        
    if ( status != DMAC_IDLE ){
        PRINTF( FN "WARNING: DMAC NOT IDLE %d\n", status );
    }
    return status;
#else
    return DMAC_IDLE;
#endif
}


int dmacQueueFlush()
{
#ifdef PGMCOMOUT
    if ( S_DMACQ.inext == 0 ) return -1;

    S_DMACQ.chain[S_DMACQ.inext-1].count |= CB_END_OF_CHAIN;

    PRINTF( "dmacQueueFlush() S_DMACQ.chain at 0x%p, inext = %d\n",
            S_DMACQ.chain, S_DMACQ.inext );
        
    flushChain();
#endif
    return 0;

}

int  dmacQueueWritePciChain(
    unsigned count, 
    unsigned pci_addr, 
    unsigned loc_addr,
    unsigned send_now
    )
{
#ifdef PGMCOMOUT
    int inext = S_DMACQ.inext;
    int iprev = inext==0? 0: inext-1;


    if ( inext >= MAXCHAIN   ) return -1;
    if ( inext == MAXCHAIN-1 ) send_now = TRUE;

    if ( send_now) count |= CB_END_OF_CHAIN;

    count |= CB_SDRAM_TO_PCI;

    S_DMACQ.chain[inext].count = count;
    S_DMACQ.chain[inext].pci_addr = pci_addr;
    S_DMACQ.chain[inext].loc_addr = loc_addr;

    if ( iprev != inext ){
        S_DMACQ.chain[iprev].next = &S_DMACQ.chain[inext];
    }

    PRINTF( "dmacQueueWriteChain() chain is at 0x%p"
            "inext was %d send_now %d\n",
            S_DMACQ.chain, inext, send_now );
        
    if ( send_now ){
        flushChain();
        return 0;               
    }else{
        S_DMACQ.inext++;
        return 0;
    }
#else
    return 0;
#endif
}

