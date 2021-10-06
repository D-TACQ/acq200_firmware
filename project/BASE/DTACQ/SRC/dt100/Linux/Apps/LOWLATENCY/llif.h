/*****************************************************************************
 *
 * File: llif.h
 *
 * $RCSfile: llif.h,v $
 * 
 * Copyright (C) 2001 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description: interface to device driver - gets mappings for
 * control-mailboxes, and data-slave memory, 
 * provides convenience functions for access                
 *
 * $Id: llif.h,v 1.9.4.8 2006/01/19 20:15:28 pgm Exp $
 * $Log: llif.h,v $
 * Revision 1.9.4.8  2006/01/19 20:15:28  pgm
 * *** empty log message ***
 *
 * Revision 1.9.4.7  2006/01/15 15:04:26  pgm
 * *** empty log message ***
 *
 * Revision 1.9.4.6  2005/11/04 17:26:18  pgm
 * *** empty log message ***
 *
 * Revision 1.9.4.5  2004/11/04 07:13:13  pgm
 * merged multicard case
 *
 * Revision 1.9.4.4.2.2  2004/09/26 11:32:39  pgm
 * multiboard tstats pollstats in
 *
 * Revision 1.9.4.4.2.1  2004/09/25 11:53:03  pgm
 * first pass multi done - check it works for n==1
 *
 * Revision 1.9.4.4  2004/09/15 12:48:21  pgm
 * use internal var for llWaitDmaDone, reformat
 *
 * Revision 1.9.4.3  2004/07/31 21:48:25  pgm
 * now with Feedback - but why does it plot backwards\/
 *
 * Revision 1.9.4.2  2004/07/27 20:49:06  pgm
 * llcontrol 2G with AO - in debug
 *
 * Revision 1.9.4.1  2004/07/25 20:39:38  pgm
 * hbpolling, 96 channels
 *
 * Revision 1.9  2002/09/25 09:50:49  pgm
 * opt to use bigbuf
 *
 * Revision 1.8  2002/09/02 15:36:14  pgm
 * hook up int masking
 *
 * Revision 1.7  2001/05/25 12:05:15  pgm
 * its a runner - shippit quick
 *
 * Revision 1.6  2001/05/21 17:42:38  pgm
 * reformed
 *
 * Revision 1.5  2001/05/20 21:24:10  pgm
 * ll WIP - almost works
 *
 * Revision 1.4  2001/05/19 19:46:15  pgm
 * enters LL mode with good bufs, mboxes
 *
 * Revision 1.3  2001/05/19 07:03:40  pgm
 * LL done for SOFT CLOCK, compiles
 *
 * Revision 1.2  2001/05/18 20:21:13  pgm
 * compiles. needs algorithms
 *
 * Revision 1.1  2001/05/18 07:09:37  pgm
 * first cut - wont compile :-(
 *
 * Revision 1.2  2000/12/28 11:03:00  pgm
 * *** empty log message ***
 *
 * Revision 1.1  1999/10/22 16:26:49  pgm
 * first entry to cvs
 *
 *
\*****************************************************************************/

/** @file llif.h interface to device driver. - gets mappings for
 * control-mailboxes, and data-slave memory, 
 * provides convenience functions for access 
 */
#ifndef __LLIF_H__
#define __LLIF_H__

extern int verbose;

#define PRINTF( n ) if ( verbose>=(n) ) printf

#ifndef __U32__
typedef unsigned short u16;
typedef unsigned u32;        // define a 32 bit quantity
#endif

/*
 * define interface to mailboxes.
 * struct MU is effectively "this" - much cleaner in C++!!
 *
 */

typedef u32 MFA;       /* Message Frame Address - unit of Q */




struct MU;   /* Message Unit */


/** set a mail mbox register: ibox [0-3]. return 0 on success */
int setMbox( struct MU* m, int ibox, u32 value );      


/** get contents of mailbox register: ibox [0-3] */
u32 getMbox( struct MU* m, int ibox );      

/** get last contents of mailbox register: ibox [0-3] */
u32 getMboxShadow(struct MU* m, int ibox );

/** just set these bits. */
int setMboxBits( struct MU* m, int ibox, u32 bits_to_set );


/** just clr these bits. */
int clrMboxBits( struct MU* m, int ibox, u32 bits_to_clr );

/** set the field in the register only. */
int setMboxField( struct MU* m, int ibox, u32 field_mask, u32 field_value );


/** return when any of the bits become true. */
u32 pollMboxBits( struct MU* m, int ibox, u32 mask, u32 goal );

/** polls regular ack from acq32. */
int pollAck( struct MU* m );

/** iboard [1-3]. return mapping on success. */
struct MU* mmapMbox( int iboard );



void setMboxPollcount(struct MU* m, int poll_count);
int getMboxPollcount(struct MU* m);


void showLastWrites( struct MU* m );

/** define interface to buffer memory.
 * again, for multiple boards this will be far more palatable as a C++ class
 */
struct DmaBuffer;   /* opaque struct for clients */

struct DmaBuffer* mmapDmaBuffer( int iboard, unsigned nbytes ); 
// maps dma buffer for board iboard {1..4}, nbytes long. ret mapping on success
u32 getBusAddr( struct DmaBuffer* b, u32 offset );
u32* getVaddr( struct DmaBuffer* b, u32 offset );
int getDmaBufferLen(struct DmaBuffer* b);


struct DmaBuffer* mmapBigBuffer( int iboard, unsigned nbytes ); 
// maps dma buffer from driver bigbuff section.

void mmapValidateDmaBuffer( struct MU* mbx, int nsamples );
// tell driver to allow normal data device access to red dmabuffer



void acq32_enableInts( struct MU* mbx, unsigned mask );
void acq32_maskInts  ( struct MU* mbx, unsigned mask );

/*
 * hbPolling - place a marker in Host Buffer, poll for overwrite
 */
int hbPoll(struct DmaBuffer *buf, int offset, 
	   int sample_len, volatile int* user_abort);
void hbPrimePoll(struct DmaBuffer *buf, int offset, int sample_len);
void hbPrimeBuffer(struct DmaBuffer *buf);


/**
 * I2O Q ports:
 * OUTBOUND: host reserves free MFA, puts MFA as command
 * INBOUND:  host gets message MFA, replaces MFA when done
 *
 * NB: direction with respect to HOST! 
 */

MFA mu_reserveOutbound(struct MU* mu);
int mu_putOutbound(struct MU* mu, MFA mfa);
/* return 0 on success */

MFA mu_getInbound(struct MU* mu);
int mu_returnInbound(struct MU* mu, MFA mfa);



static inline void memset32(u32 *buf, u32 value, int count)
{
	int ii;

	for (ii = 0; ii < count; ++ii){
		buf[ii] = value;
	}
}



#define MFA_FAIL 0xffffffff
#endif
