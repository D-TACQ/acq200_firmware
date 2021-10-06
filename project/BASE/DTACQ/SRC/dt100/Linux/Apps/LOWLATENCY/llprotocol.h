/*****************************************************************************
 *
 * File: llprotocol.h
 *
 * $RCSfile: llprotocol.h,v $
 * 
 * Copyright (C) 2001 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description: interface to acq32, implements part of mbox protocol          
 *
 * $Id: llprotocol.h,v 1.10.2.16 2006/01/15 11:18:18 pgm Exp $
 * $Log: llprotocol.h,v $
 * Revision 1.10.2.16  2006/01/15 11:18:18  pgm
 * SYNC_2V
 *
 * Revision 1.10.2.15  2005/11/04 17:26:18  pgm
 * *** empty log message ***
 *
 * Revision 1.10.2.14  2005/09/25 20:32:34  pgm
 * LLCV2 initial poll corrected
 *
 * Revision 1.10.2.13  2005/08/12 09:51:25  pgm
 * *** empty log message ***
 *
 * Revision 1.10.2.12  2005/08/11 10:02:24  pgm
 * SYNC_ECM - init host side slave memory areas
 *
 * Revision 1.10.2.11  2005/08/01 11:10:24  pgm
 * V2 part 1 running - local status, no hbpoll - ECM 100 acheived
 *
 * Revision 1.10.2.10  2004/12/09 12:58:34  pgm
 * various attempts at repartitioning for speed
 *
 * Revision 1.10.2.9  2004/11/14 09:50:44  pgm
 * Doxygen type docs
 *
 * Revision 1.10.2.8  2004/11/04 07:13:13  pgm
 * merged multicard case
 *
 * Revision 1.10.2.7.2.2  2004/09/26 11:32:39  pgm
 * multiboard tstats pollstats in
 *
 * Revision 1.10.2.7.2.1  2004/09/25 09:08:31  pgm
 * first UI/test split
 *
 * Revision 1.10.2.7  2004/09/15 12:48:21  pgm
 * use internal var for llWaitDmaDone, reformat
 *
 * Revision 1.10.2.6  2004/08/10 07:35:46  pgm
 * works with ACQ196
 *
 * Revision 1.10.2.5  2004/07/31 21:48:25  pgm
 * now with Feedback - but why does it plot backwards\/
 *
 * Revision 1.10.2.4  2004/07/27 20:49:06  pgm
 * llcontrol 2G with AO - in debug
 *
 * Revision 1.10.2.3  2004/07/25 20:39:38  pgm
 * hbpolling, 96 channels
 *
 * Revision 1.10.2.2  2003/12/12 14:51:20  pgm
 * UPDATE DACS HACK
 *
 * Revision 1.10.2.1  2003/11/11 10:11:21  pgm
 * fudge LLC SET DAC
 *
 * Revision 1.10  2003/07/03 11:15:21  pgm
 * set dacs
 *
 * Revision 1.9  2002/09/02 11:01:18  pgm
 * revised debug levels
 *
 * Revision 1.8  2002/08/29 13:22:21  pgm
 * better docs
 *
 * Revision 1.7  2002/03/12 15:37:52  pgm
 * emacs format rools OK
 *
 * Revision 1.6  2001/08/23 20:00:50  pgm
 * mod protocol avoids races and works
 *
 * Revision 1.5  2001/08/17 07:25:38  pgm
 * ll, aync update, has race
 *
 * Revision 1.4  2001/05/25 12:05:15  pgm
 * its a runner - shippit quick
 *
 * Revision 1.3  2001/05/21 20:43:29  pgm
 * *** empty log message ***
 *
 * Revision 1.2  2001/05/21 17:50:36  pgm
 * reform again
 *
 * Revision 1.1  2001/05/21 17:42:38  pgm
 * reformed
 *
 * Revision 1.5  2001/05/20 21:24:10  pgm
 * first entry to cvs
 *
 *
\*****************************************************************************/


/** @file llprotocol.h API for bus level protocol. */

#ifndef __LLPROTOCOL_H__
#define __LLPROTOCOL_H__

/* this is a HACK - need local copy of acq32busprot.h */
#define BP_MB_LLC_UPDATEDAC 1
#define LLC_CSR_M_UPDATEDACS 'f'

/*******************************************************************************
 * Use these functions to enter Low Latency Mode
 */

int enterLLCSoftClock( 
	struct MU* m, int clkpos, int trpos,
	int internal_loopback,
	u32 command_mods);
/*
 * Enter LowLatency Mode for SOFT CLOCK capture
 */

int enterLLCExtClock( 
	struct MU* m, int clkpos, int trpos, 
	unsigned short divisor,
	int internal_loopback,
	u32 command_mods
	);
/**
 * Enter LowLatency Mode for EXTERNAL CLOCK capture
 */

int enterLLC_SYNC_ECM(
	struct MU* m, int clkpos, int trpos, 
	unsigned short divisor,
	int internal_loopback,
	u32 command_mods,
	u32 init_buf_baddr
	);
/*******************************************************************************
 * Now use these functions while in Low Latency Mode
 */




int leaveLLC( struct MU* m );
/*
 * Leave LowLatency Mode
 */

#include <setjmp.h>
extern sigjmp_buf G_env;


static inline u32 llPollSack( struct MU* m )
// return 0 on SACK, exit on SNACK
// private: not used outside this module 
{
	u32 status = pollMboxBits( 
		m, BP_MB_LLC_CSR, LLC_CSR_SACK|LLC_CSR_SNACK, LLC_CSR_SACK); 
    
	if ((status&LLC_CSR_SNACK) != 0){
		siglongjmp(G_env, status);
	}
	return status;
}
static inline u32 llGetCsr( struct MU* m )
// private: not used outside this module 
{
	return getMbox( m, BP_MB_LLC_CSR );
}


/* Sending commands to the board - the following command codes apply (copy from acq32busprot.h)

* LLC LLC LLC LLC LLC LLC LLC
*
* Low Latency Control Mode - works like this ...
* 
*
* prams for setup command: BP_FC_SET_MODE_LLC
* A1 - prams
* A3 - clock div for extclock mode
 
#define BP_FC_SET_MODE_LLC FCL   // switch into Low Latency Control Mode.
 
#define BP_FC_SET_MODE_LLC_SOFTCLOCK    0x80  // EXCLUSIVE!
#define BP_FC_SET_MODE_LLC_EXTCLOCK     0x40

#define BP_FC_SET_MODE_LLC_CLKPOL_POS   0x20  // set for rising edge active
#define BP_FC_SET_MODE_LLC_TRPOL_POS    0x10  // set for rising edge active
#define BP_FC_SET_RTCLOCK_TIMING        0x04

#define BP_FC_SET_MODE_LLC_INTSOFT_CLK  0x01  // test mode
#define BP_FC_SET_MODE_LLC_INTDIV_CLK   0x02  // test mode

*/


static inline u32 llSetCmd( struct MU* m, u32 cmd )
/* Send a command to the llcontrol board */
{
	PRINTF(4)( "llsetCmd( 0x%08x )\n", cmd );
    
	cmd &= ~( LLC_CSR_SACK|LLC_CSR_SNACK);
    
	setMbox( m, BP_MB_LLC_CSR, cmd );
	return llPollSack( m );
}

static inline void llSetAddr( struct MU* m, u32 addr, u32 cmd )
/* Set the target address and send a command to the llcontrol board */
{
	setMbox( m, BP_MB_LLC_DATA_ADDR, addr );
    
    
	llSetCmd( m, LLC_CSR_M_SETADDR|cmd );
}


static inline u32 llGetTlatch( struct MU* m )
/* Get the latch count (time at ADC clock) */
{
	return getMbox( m, BP_MB_LLC_TADC );
}

static inline int llSetTlatch( struct MU* m, u32 value )
/* Set (reset) the latch count */
{
	return setMbox( m, BP_MB_LLC_TADC, value );
}

static inline u32 llGetTinst( struct MU* m )
/* get the instantaneous count (time now) */
{
	return getMbox( m, BP_MB_LLC_TINST );
}

static inline u32 llGetTprocess( struct MU* m )
/* get the time taken by the capture process */
{
	return LLC_GET_TCYCLE( llGetCsr( m ) );
}

u32 llWaitDmaDone(struct MU* m);
/** polls until DMA has completed. 
 * Returns tlatch 
 * guaranteed DMA done when tlatch updated
 */

u32 llv2WaitDmaDone(struct MU *m, volatile u32* hstats);
/** polls until DMA has completed. 
 * Returns tlatch 
 * guaranteed DMA done when tlatch updated
 * V2 method does NOT poll PCI
 */

u32 llv2WaitDmaDone_2v(struct MU *m, volatile u32* hstats, unsigned tlatch);
/** polls until DMA has completed. 
 * Returns tlatch 
 * guaranteed DMA done when tlatch updated
 * V2 method does NOT poll PCI
 */

void llv2InitDmaDone(volatile u32* hstats);
/** prepare (poison) target buf for incoming DMA. */


static inline int llCounterRunning( struct MU* m, u32 csr )
/** returns TRUE when counters are running the process has started on GATE.
 */
{
	return (llSetCmd( m, csr+LLC_CSR_M_READCTR )&LLC_CSR_S_CTR_RUN) != 0;
}

#define DAC_COUNT        16
#define DAC_SAMPLE_SIZE (DAC_COUNT*sizeof(short))



static inline void llSetDacs( struct MU* mu, void* dacvals, char* bigbuf_base)
/**
 * uses i2o buffer Q.
 - obtain a descriptor (MFA) from the target Q (1)
 - copy from client buffer to slave memory ref by MFA (2)
 - post descriptor to target Q (3)
 *
 * target is notified about the post, and uses the MFA to setup a DMA direct
 * from host memory to DACs.
 *
 * For MINIMUM latency 
 - a/ split the function to mu_reserveOutbound(mu) ahead of time
 *    (don't pre-reserve more than one .. 100 this is a limited resource
 - b/ eliminate the memcpy (but this is quick, regardless).
 */
{
	MFA mfa = mu_reserveOutbound(mu);                   /* (1) */
	PRINTF(1)("llSetDacs MFA 0x%08x dst %p src %p\n", 
		  mfa, bigbuf_base+mfa, dacvals);

	memcpy( bigbuf_base+mfa, dacvals, DAC_SAMPLE_SIZE); /* (2) */
	mu_putOutbound(mu, mfa);                            /* (3) */
}

/*
 * same function split into parts to enable scheduling
 */

#define DAC_BASE(bigbuf_base, mfa)  ((void*)bigbuf_base + (mfa))


void llPrimePollHB(struct DmaBuffer *buf, int offset, int sample_len);
void llPollHB(struct DmaBuffer *buf, int offset, int sample_len);


/** V2 host buffer segmentation - for full V2, 4K is enough. */

#define LLCV2_HB_OFFSET 0x00100000 /** 1M offset into 16M, backcompatibility */

/** but for future, simpler host side drivers, we'd like a single 4K buf,
 * no offset. => use a variable and set as appropriate
 */

extern unsigned  llcv2_hb_offset;

#define LLCV2_AI_HSBT 0x000
#define LLCV2_AO_HSBS 0x400
#define LLCV2_DO_HSBS 0x800
#define LLCV2_STATUS_HSBT 0xc000

#define LLCV2_OFFSET_AI_HSBT (llcv2_hb_offset+LLCV2_AI_HSBT)
#define LLCV2_OFFSET_AO_HSBS (llcv2_hb_offset+LLCV2_AO_HSBS)
#define LLCV2_OFFSET_DO_HSBS (llcv2_hb_offset+LLCV2_DO_HSBS)
#define LLCV2_OFFSET_STATUS_HSBT (llcv2_hb_offset+LLCV2_STATUS_HSBT)

u32 llv2_extend32(u32 old32, u32 new12);
#endif
