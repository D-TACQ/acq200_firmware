/*****************************************************************************
 *
 * File: llcontrol-core.c
 *
 * $RCSfile: llcontrol-core.c,v $
 * 
 * Copyright (C) 2001 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *     application implementing the LOW LATENCY CONTROL feature
 *
 * $Id: llcontrol-core.c,v 1.1.4.33 2010/08/26 16:25:52 pgm Exp $
 * $Log: llcontrol-core.c,v $
 * Revision 1.1.4.33  2010/08/26 16:25:52  pgm
 * more switchable instrumentation
 *
 * Revision 1.1.4.32  2009/09/29 13:09:14  pgm
 * RFM emulation (not complete)
 *
 * Revision 1.1.4.31  2009/04/02 13:19:01  pgm
 * docs away
 *
 * Revision 1.1.4.30  2009/03/28 22:23:05  pgm
 * ao32cpci first cut looks good
 *
 * Revision 1.1.4.29  2009/03/28 18:47:39  pgm
 * sync2VAO32 take 1
 *
 * Revision 1.1.4.28  2009/03/26 14:52:03  pgm
 * split sync2v, acq216 from main core
 *
 * Revision 1.1.4.27  2009/03/26 12:40:36  pgm
 * reuse same dac_src for each card, avoid data overrun
 *
 * Revision 1.1.4.26  2007/02/19 15:22:23  pgm
 * *** empty log message ***
 *
 * Revision 1.1.4.25  2006/02/23 21:56:40  pgm
 * remove unwanted statics
 *
 * Revision 1.1.4.24  2006/02/22 11:15:36  pgm
 * ensure tlatch zero in case of code re-use without new process create
 *
 * Revision 1.1.4.23  2006/01/28 18:18:55  pgm
 * ST_CAPDONE
 *
 * Revision 1.1.4.22  2006/01/19 20:15:28  pgm
 * *** empty log message ***
 *
 * Revision 1.1.4.21  2006/01/15 11:18:18  pgm
 * SYNC_2V
 *
 * Revision 1.1.4.20  2006/01/09 13:40:31  pgm
 * SYNC_2V mode implemented
 *
 * Revision 1.1.4.19  2005/11/30 10:52:53  pgm
 * *** empty log message ***
 *
 * Revision 1.1.4.18  2005/11/04 17:26:18  pgm
 * *** empty log message ***
 *
 * Revision 1.1.4.17  2005/10/22 12:18:56  pgm
 * *** empty log message ***
 *
 * Revision 1.1.4.16  2005/10/20 20:51:00  pgm
 * *** empty log message ***
 *
 * Revision 1.1.4.15  2005/10/03 15:03:48  pgm
 * acq216 llc with prams
 *
 * Revision 1.1.4.14  2005/09/25 20:32:34  pgm
 * LLCV2 initial poll corrected
 *
 * Revision 1.1.4.13  2005/08/12 20:28:20  pgm
 * *** empty log message ***
 *
 * Revision 1.1.4.12  2005/08/12 10:05:53  pgm
 * *** empty log message ***
 *
 * Revision 1.1.4.11  2005/08/12 09:51:25  pgm
 * *** empty log message ***
 *
 * Revision 1.1.4.10  2005/08/11 10:02:24  pgm
 * SYNC_ECM - init host side slave memory areas
 *
 * Revision 1.1.4.9  2005/08/01 12:39:55  pgm
 * includes bridge test
 *
 * Revision 1.1.4.8  2005/08/01 11:10:24  pgm
 * V2 part 1 running - local status, no hbpoll - ECM 100 achieved
 *
 * Revision 1.1.4.7  2004/12/13 21:39:15  pgm
 * correct DAC base offset
 *
 * Revision 1.1.4.6  2004/12/10 12:01:26  pgm
 * fix timing
 *
 * Revision 1.1.4.5  2004/12/09 22:31:57  pgm
 * microsecond timestamps do the BIZ - ECM20 here we go
 *
 * Revision 1.1.4.4  2004/12/09 12:58:34  pgm
 * various attempts at repartitioning for speed
 *
 * Revision 1.1.4.3  2004/12/08 14:51:39  pgm
 * *** empty log message ***
 *
 * Revision 1.1.4.2  2004/11/14 09:50:44  pgm
 * Doxygen type docs
 *
 * Revision 1.1.4.1  2004/11/04 07:13:13  pgm
 * merged multicard case
 *
 * Revision 1.1.2.8  2004/09/26 19:50:58  pgm
 * *** empty log message ***
 *
 * Revision 1.1.2.7  2004/09/26 15:28:40  pgm
 * *** empty log message ***
 *
 * Revision 1.1.2.6  2004/09/26 11:32:38  pgm
 * multiboard tstats pollstats in
 *
 * Revision 1.1.2.5  2004/09/25 21:16:28  pgm
 * multiboard case runs
 *
 * Revision 1.1.2.4  2004/09/25 11:53:03  pgm
 * first pass multi done - check it works for n==1
 *
 * Revision 1.1.2.3  2004/09/25 09:12:35  pgm
 * *** empty log message ***
 *
 * Revision 1.1.2.2  2004/09/25 09:08:31  pgm
 * first UI/test split
 *
 * Revision 1.1.2.1  2004/09/25 08:44:47  pgm
 * *** empty log message ***
 *
 */


/** @file llcontrol-core.c module implements the core of llcontrol loop.
 *
 - mmapMboxes() - get app mapping for mailboxes

 - mmapBuffer() - get dmabuff and phys addresses

 - runSCM() - capture using soft trigger

 - runECM() - capture using external clock
<p>
<h2>Linux 2.4 Host Only:</h2>
2.4 2.4 2.4 2.4 2.4 2.4 2.4 2.4 2.4 2.4 2.4 2.4 2.4 2.4 2.4 2.4 2.4 2.4 2.4 2.4

  Note on HOST MEMORY MAP: assumes bootcommand configured as follows,
  to allow a 16MB host area per card in at the top of memory.
  This host area will be slaved off the pci backplane

 - mem=NNN restrict memory for Linux. Must allow Nx16MB + gap
 - acq32_big_buf_base=0xnnnn : informs driver of start of area
 - acq32_big_buf_len=0xxxxx  : informs driver of length of area

eg

[dt100@cp605 dt100]$ cat /proc/cmdline mem=320M acq32.load_order=slotbased 

 acq32_big_buf_base=0x18000000 acq32_big_buf_len=0x08000000

For the purpose of LLCONTROL, each 16MB card are is used as follows:

- 0x000000 .. 0xefffff (15MB) - AI reception area (controlled by client app).
- 0xf00000 .. 0xffffff (1MB)  - message buffers (owned by target).

2.4 2.4 2.4 2.4 2.4 2.4 2.4 2.4 2.4 2.4 2.4 2.4 2.4 2.4 2.4 2.4 2.4 2.4 2.4 2.4


This means that if the llcontrol app is using an incrementing memory
strategy in host buffer for AI, this is subject to max 15MB
(80K samples at 96 channels).

The message buffers are used for DAC output. 
The application reserves a Mesage Frame Address (MFA) from the target 
before copying data to slave memory.
The MFA is an index into the message buffer memory.

</p>

<h2>Linux 2.6 Host: </h2>
the host buffer is setup dynamically, but is LIMITED to 4MB, 
of which the top 1M is used for messaging.

*/



#include "local.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

#include <popt.h>

#include "acq32ioctl.h"
#include "acq32busprot.h"

#include "llif.h"
#include "llprotocol.h"


#include "llcontrol.h"
#include "x86timer.h"

#ifdef __ACQ196__
#define FLAVOR "ACQ196"
#else
#define FLAVOR "ACQ32"
#endif


#include "llcontrol-core.h"

const char* core_ident = "$Revision: 1.1.4.33 $ B1102\n";





static void mbox_updateTstats(
	u32 cmd, struct Card* card, struct TimingStats* tstats)
/** update timing stats from card mailbox. */
{
	llSetCmd(card->mbx, cmd+LLC_CSR_M_READCTR);
	tstats->tinst      = llGetTinst(card->mbx);       
	tstats->tprocess   = llGetTprocess(card->mbx);

/** @todo - with hbpoll ON, WaitDmaDone should be a formality -
 * single poll to confirm done - we should CHECK THIS IS TRUE
 */
	PRINTF(2)( "Capture Done: [%d] Tsample:%d Tprocess:%d Tnow:%d\n",
		   card->slot,
		   tstats->tlatch, tstats->tprocess, tstats->tinst );
}




void v2_updateTstats(
	u32 cmd, struct Card* card, struct TimingStats* tstats)
/** updates timing stats from embedded host buffer data */
{
	u32* stats = getVaddr(card->buf, LLCV2_OFFSET_STATUS_HSBT);
	tstats->tinst = llv2_extend32(stats[BP_MB_LLC_TINST], 
				      stats[LLCV2_STATUS_TINST]);
	tstats->tprocess = LLC_GET_TCYCLE(stats[BP_MB_LLC_CSR]);
}


void (*updateTstats)(u32 cmd, struct Card* card, struct TimingStats* tstats) = 
	mbox_updateTstats;
/**< virtual function to retrieve timing stats.
 *  defaults to V1 mode mbox_updateTstats().
 */



void doApplicationWork(struct TestDescription* td, u32 offset)
/* copy data to a user buffer. A real app may do more than this ;-) */
{
	EACHCARD_INIT;
	int work_len = td_sample_size(td);
	int work_offset = td->iter * work_len;

	FOREACHCARD{
		memcpy(td->work_buf[icard]+work_offset,
		       getVaddr(EACHBUF(td), offset),
		       work_len);
	}
}



static u32 card_v1_WaitDmaDone(struct Card* card)
{
	return llWaitDmaDone(card->mbx);
}

u32 card_v2_WaitDmaDone(struct Card* card)
{
	return llv2WaitDmaDone(card->mbx, 
			       getVaddr(card->buf, LLCV2_OFFSET_STATUS_HSBT));
}



u32 (*waitDmaDone)(struct Card *c) = card_v1_WaitDmaDone;
/**< virtual function to block until DMA done.
 *  defaults to V1 mbox
 */

	
int user_abort = 0;    /** @todo could be set by a signal */

static int do_run( struct TestDescription* td, int soft_clock )
/** runs the test.
 * PSEUDO-CODE:
 *
 - (1) Clear the latch timer
 - (2) Set a local memory target address and arm the capture
 - (3) Poll for counter running (hardware counter starts on external gate)
 - (4) Iterate for required number of samples:
 - (5)     [optionally send a soft clock command]  trigger an acquisition
 - (6)     Wait for DMA Done - at this point data is available in target mem.
 *         A "real" control application is probably getting most of its calcs 
 *         done here rather than simply polling
 - (7)     [Get the latch (sample) and current uSec counters from the boards - 
 *          only if interested]
 - (8)     Check the process has not stopped (external gate high)
 - (9)     [optionally update the target address - else it gets overwritten]
 - (a)     using DACS - preallocate a MFA for dac data.
 - (b)     write data to host side buffer(f(MFA)) and post MFA
 - (b.1)
	 * take the incoming value on feedback_channel
	 * and propagate to all DACS.
	 * default is to assume HAWG on DAC0
	 * (so feedback_channel better be 0 !),
         * but td->update_dacs makes a better test.
 - (b.15)  
	 * special case where we are DRIVING the
	 * initial DAC signal from host side. 
 - (b.2)
	 *  simple feedforward case - just drive all DACS
	 *  from AWG pattern.

 - (c)     later, pick up another MFA
 - (d)     finally, return final set of MFA's. might be a good idea to ensure 
           data is zero at this point
 */
{
	struct TimingStats tstats[MAXCARDS] = {};
	struct DacBuf {
		MFA mfa;
		void* bbb;
	} dac_buf[MAXCARDS];
#define EACHMFA      (dac_buf[icard].mfa)
#define EACHBBB      (dac_buf[icard].bbb)
#define EACHDAC_BASE (EACHBBB + EACHMFA)
#define EACHDAC_BASE16 ((u16*)EACHDAC_BASE)

	u32 cmd = LLC_MAKE_DECIM(td->decimation);
	u32 offset = 0;

	unsigned ipoll = 0;
	int uses_dacs = td->update_dacs || td->feedback;

	EACHCARD_INIT;

	if (uses_dacs) FOREACHCARD { /* (a) */
		EACHBBB = getVaddr(EACHBUF(td), 0);
		EACHMFA = mu_reserveOutbound(EACHMBX(td));
	}
	FOREACHCARD{ /* (1) */
		llSetTlatch(EACHMBX(td), tstats[icard].tlatch = 0xffffffff);
	}
   
	FOREACHCARD{ /* (2) */
		updateTargetAddr(cmd+LLC_CSR_M_ARM, EACHCARD(td), offset);
	}


	if (td->update_dacs){
		void *dac_src = td_get_next_dac_data(td);
		FOREACHCARD{
			llSetDacs(EACHMBX(td), dac_src,
					(char*)getVaddr(EACHBUF(td), 0));
		}
	}
	// wait for gate /* (3) */

	while( !llCounterRunning(FIRSTMBX(td), cmd) ){ 
		POLLALERT(ipoll, "Polling for Counter Running\n");
	}

	INIT_TIMER;
	/* WORKTODO - assumes all cards now running */

	
	for ( td->iter = 0; td->iter != td->iterations; ++td->iter ){ /* (4) */
		if (soft_clock) FOREACHCARD{ /* (5) */
			llSetCmd(EACHMBX(td), cmd+LLC_CSR_M_SOFTCLOCK);
		}
	
		if (td->hb_polling) FOREACHCARD{
			MARK_TIME(0, "hb_polling");
			tstats[icard].hb_poll = 
				hbPoll(EACHBUF(td), offset, 
					td->channels*2, &user_abort);
		}

		
		FOREACHCARD{                                  /* (6)+(7) */
			MARK_TIME(1, "waitDmaDone() before");
			tstats[icard].tlatch = waitDmaDone(EACHCARD(td));
			MARK_TIME(2, "waitDmaDone() after");
		}

		if (!td->min_latency_test) FOREACHCARD{        /* (6)+(7) */
			updateTstats(cmd, EACHCARD(td), &tstats[icard]);    
		}

		
		if (td->do_work){
			doApplicationWork(td, offset);
		}

		if (td->feedback) { /* (b.1) */
			u16 *pvin = (u16 *)getVaddr(FIRSTBUF(td), offset);
			u16 vin = pvin[td->feedback_channel];
			u32 feedback = vin << 16 | vin;

			PRINTF(1)("feedback 0x%08x\n", feedback);

			FOREACHCARD{   
				MARK_TIME(3, "feedback 1");
				memset32(EACHDAC_BASE, feedback, DAC_COUNT/2);

				if (td->update_dacs){ /* (b.15) */

					u16* excite = td_get_next_dac_data(td);
				
					EACHDAC_BASE16[td->feedback_channel] 
					 = excite[td->feedback_channel];
				}
				MARK_TIME(4, "feedback 2");
				mu_putOutbound(EACHMBX(td), EACHMFA); 
				MARK_TIME(5, "feedback 3");
			}
		}else if (td->update_dacs){ /** (b.2) */
			void* dac_data = td_get_next_dac_data(td);
			FOREACHCARD{
				memcpy(EACHDAC_BASE, 
				       dac_data+icard*32,
				       DAC_SAMPLE_SIZE);
				mu_putOutbound(EACHMBX(td), EACHMFA);
			}

		}
		
		/* check process completion.
                 * if hardware_gateoff is employed, then the app would need
                 * to make use of a pre-arranged DIO encoding for on/off instead                 */
		 
		if (!td->hardware_gate_off &&
                     !llCounterRunning(FIRSTMBX(td), cmd )){          /* (8) */
			fprintf( stderr, "Trigger is off - stop\n" );
			break;
		}
		
		FOREACHCARD_MARK_TIME(6, "after llCounterRunning check");

		// maybe set new target addr

		if ( !td->overwrite ){
			offset += td->sample_offset;

			FOREACHCARD{                                    /* 9 */
				updateTargetAddr(cmd, EACHCARD(td), offset);
				MARK_TIME(7, "after updateTargetAddr");
			}
		}else{
			if (td->hb_polling) FOREACHCARD{
				hbPrimePoll(EACHBUF(td), 
					    offset, td_sample_size(td));
				MARK_TIME(7, "after hbPrimePoll");
			}
		}

		// maybe wait for an interval 
		// (crudely simulate the effect of control calculation!)

		if ( td->arg.interval ){
			u32 tinst;

			printf("hello interval %d\n", td->arg.interval);
			do {
				llSetCmd(FIRSTMBX(td), cmd+LLC_CSR_M_READCTR);
				tinst = llGetTinst(FIRSTMBX(td));
			}
			while ( tinst - tstats[0].tlatch < td->arg.interval );
		}

		if (uses_dacs){
			FOREACHCARD{ /* (c) */
				MARK_TIME(8, "mu_reserveOutbound() before");
				EACHMFA = mu_reserveOutbound(EACHMBX(td));
				MARK_TIME(9, "mu_reserveOutbound() after");
			}			
		}

		if (td->tlog){          
			FOREACHCARD{
				tstats[icard].target_poll =
					getMboxPollcount(EACHMBX(td));
				MARK_TIME(10, "10");
				updateTimingStats(
					td->stats_buf[icard], 
					td->iter, 
					&tstats[icard]);
				MARK_TIME(11, "11");
			}
		}       
		TIMER_CHECK_OVERFLOW;
	}
        
	if (uses_dacs) FOREACHCARD{ /* (d) */
	       mu_putOutbound(EACHMBX(td), EACHMFA);
	}			
	G_quit = 1;
	return 0;
#undef EACHMFA
#undef EACHBBB
#undef EACHDAC_BASE
#undef EACHDAC_BASE16
}



static int runSCM( struct TestDescription* td )
/** Run a Test - Soft Clock Master. */
{
	return do_run( td, 1 );
}
 



 
static int runECM( struct TestDescription* td )
/** Run a Test - External Clock Master. */
{
	return do_run ( td, 0 );
}

u32 commandModifiers(struct TestDescription *td) {
	u32 mods = 0;

	if (td->V2_BUFS[LLCV2_INIT_MARKER] == LLCV2_INIT_MAGIC_MARKER){
		mods |= BP_FC_SET_LLCV2_INIT;
	}

	return mods;
}



static int measureBridge(struct MU *mu) 
/** Run a test - measureBridge performance. */
{
#define NMEASURE 20
	static struct {
		unsigned elapsed;
		u32 mbox;
	} stats[NMEASURE];
	int im;
	unsigned sum = 0;
	
	for (im = 0; im != NMEASURE; ++im){
		INIT_TIMER;
		stats[im].mbox = getMbox(mu, 3);
		stats[im].elapsed = get_elapsed_microseconds(0);

		usleep(10000);
	}      

	fprintf(stderr, "[%2s], %10s, %10s\n","id", "mbox3", "usecs");
	for (im = 0; im != NMEASURE; ++im){
		sum += stats[im].elapsed;

		fprintf(stderr, "[%2d], 0x%08x, %10d\n",
			im, stats[im].mbox, stats[im].elapsed);
	}

	fprintf(stderr, "mean  %10s, %12.1f\n", "", (float)sum/NMEASURE);
	return 0;
}

static int measureBridgeStats(struct TestDescription *td, struct MU *mu)
{
	static struct {
		int min;
		int max;
		int sum;
		int samples;
	} stats = { 1000000, -1000000, 0, 0 };

	int im;
	int current;
	EACHCARD_INIT;


	if ( td->mask_ints ){
		PRINTF(1)( "mask ints 0x%04x\n", td->mask_ints_mask );
		FOREACHCARD{
			acq32_maskInts(EACHMBX(td), td->mask_ints_mask);
		}
	}

	for (im = 0; im != td->iterations; ++im){
		INIT_TIMER;
		getMbox(mu, 3);
		
		current = get_elapsed_microseconds(0);

		if (current < stats.min){
			stats.min = current;
		}else if (current > stats.max){
			stats.max = current;
		}
		stats.sum += current;
		stats.samples++;
	}

	if (td->mask_ints){
		FOREACHCARD{
			acq32_enableInts(EACHMBX(td), td->mask_ints_mask);
		}
	}

	fprintf(stderr, "measureBridgeStats results from %d polls\n", 
		stats.samples);
	fprintf(stderr, "%10s %d\n", "min", stats.min);
	fprintf(stderr, "%10s %d\n", "max", stats.max);
	fprintf(stderr, "%10s %.2f\n", "mean", (float)stats.sum/stats.samples);
	return 0;
}

int runTest( struct TestDescription* td )
/** generic top level text exec. */
{
	int rc = 0;
	int status;
	EACHCARD_INIT;

	PRINTF(1)("runTest mode %d\n", td->mode);

	if ((status = sigsetjmp(G_env, 1)) == 0){
		if ( td->tlog ){
			int ne = td->iterations;
			FOREACHCARD{
				td->stats_buf[icard] = 
					calloc(sizeof(struct TimingStats), ne);
				assert(td->stats_buf[icard]);
			}
		}

		if ( td->mask_ints ){
			PRINTF(1)( "mask ints 0x%04x\n", td->mask_ints_mask );
			FOREACHCARD{
				acq32_maskInts(EACHMBX(td), 
					       td->mask_ints_mask);
			}
		}

		switch( td->mode ){
		case M_NULL:
			FOREACHCARD{
				enterLLCSoftClock(EACHMBX(td), 
						  td->clkpos, 
						  td->trpos, 
						  td->internal_loopback,
						  0);
			}
			rc  = 0;
			break;
		case M_SCM:
			FOREACHCARD{
				enterLLCSoftClock(EACHMBX(td), 
						  td->clkpos, td->trpos, 
						  td->internal_loopback,
						  commandModifiers(td));
			}
			if (isAcq216()){
				rc = runSCM216(td);
			}else{
				rc = runSCM( td );
			}
			break;
		case M_ECS:
		case M_ECM:
			FOREACHCARD{
				enterLLCExtClock(
					EACHMBX(td),
					td->clkpos, td->trpos, 
					td->arg.divisor,td->internal_loopback,
					commandModifiers(td));
			}
			rc = runECM( td );
			break;
		case M_SYNC_ECM:
			FOREACHCARD{
				appEnterLLC_SYNC_ECM(
					THIS_CARD, EACHMBX(td), td);
			}
			rc = runSYNC_ECM(td, 0);
			break;
		case M_SYNC_2V:
			FOREACHCARD{
				appEnterLLC_SYNC_2V(
					THIS_CARD, EACHMBX(td), td);
			}
			rc = runSYNC_2V(td, 0);
			break;	
		case M_SYNC_2VAO32:
			FOREACHCARD{
				appEnterLLC_SYNC_2VAO32(
					THIS_CARD, EACHMBX(td), td);
			}
			rc = runSYNC_2VAO32(td, 0);
			break;		
		case M_SYNC_2VRFM:
			appEnterLLC_SYNC_2VRFM();
			rc = runSYNC_2VRFM(td, 0);
			break;
		case M_MEASURE_BRIDGE:
			if (td->iterations > 20){
				FOREACHCARD(measureBridgeStats(
						    td, EACHMBX(td)));
			}else{
				FOREACHCARD(measureBridge(EACHMBX(td)));
			}
			return 0;
		default:
			assert( 0 );
		}

	}else{
		fprintf( stderr, "longjmp!\n");
		fprintf( stderr, "ERROR:runTest got a SNACK, 0x%08x\n", 
			 status );
		rc = -1;
	}

		
	FOREACHCARD{
		leaveLLC(EACHMBX(td));
	}

	if ( td->mask_ints ){
		FOREACHCARD{
			acq32_enableInts(EACHMBX(td), td->mask_ints_mask);
		}
		PRINTF(1)( "mask ints 0x%04x clear\n", td->mask_ints_mask );
	}

	return rc;
}

#include <signal.h>

static struct TestDescription* S_td;

static void quit_handler( int signum )
{
	EACHCARD_INIT;
	struct TestDescription* td = S_td;

	fprintf( stderr, "Quitting Time - mailboxes were\n" );

	FOREACHCARD{
		fprintf(stderr,	"0x%08x  0x%08x  0x%08x  0x%08x\n",
			getMbox(EACHMBX(td), 0), 
			getMbox(EACHMBX(td), 1),    
			getMbox(EACHMBX(td), 2),
			getMbox(EACHMBX(td), 3) );
	}

	FOREACHCARD{
		showLastWrites(EACHMBX(S_td));
	}

	FOREACHCARD{
		leaveLLC(EACHMBX(S_td));
	}
	exit( signum );
}

void setupAbortHandler( struct TestDescription* td )
{
	static struct sigaction  def_action = { { quit_handler } };

	S_td = td;    // ugly global. use class var in C++
    
	sigaction( SIGINT, &def_action, 0 );
}




static void monitor_handler( int signum )
{
	static u32 old_mboxes[4];
	u32 new_mboxes[4];
	int ibox;
	int changed = 0;

	for ( ibox = 0; ibox != 4; ++ibox ){
		new_mboxes[ibox] = getMbox( S_td->cards[0].mbx, ibox );
		if ( new_mboxes[ibox] != old_mboxes[ibox] ){
			changed = 1;
		}
	}
        
	if ( changed || verbose > 2 ){
		fprintf( stderr, "%8d ", S_td->iter );
		for ( ibox = 0; ibox != 4; ++ibox ){
			fprintf( stderr, "0x%08x%c ", 
				 new_mboxes[ibox], 
				 new_mboxes[ibox]!=old_mboxes[ibox]? '*': ' ' );
                            
			old_mboxes[ibox] = new_mboxes[ibox];
		}
		fprintf( stderr, "\n" );
	}
	if ( G_quit ){
		setupMonitor( 0 );
		fprintf( stderr, "monitor quitting\n" );
	}           
}

void setupMonitor( int millisec )
{
	static struct sigaction monitor_action = { { monitor_handler } };
	static struct itimerval new_timer = {};

	new_timer.it_value.tv_usec    =
		new_timer.it_interval.tv_usec = millisec*1000;
    
	sigaction( SIGALRM, &monitor_action, 0 );
	setitimer( ITIMER_REAL, &new_timer, 0 );
}

void initCardResource(struct Card* card)
{
	card->mbx = mmapMbox(card->slot);
	card->buf = mmapBigBuffer(card->slot, ACQ196_BIGBUF_AREA);
	hbPrimeBuffer(card->buf);

	get_cpu_clock_speed();
}



static void debug_prompt(int icard, int ibuf, u32 addr)
/** promnpt user to ensure pram set (temporary pre-update measure). */
{
	static const char * LLCV2_LOOKUP[] = {
		[LLCV2_INIT_AI_HSBT] = "AI_target",
		[LLCV2_INIT_AO_HSBS] = "AO_src",
		[LLCV2_INIT_DO_HSBS] = "DO src",
		[LLCV2_INIT_STATUS_HSBT] = "STATUS_target"
	};
	fprintf(stderr, "V2: please ensure card %d has pram %s set 0x%08x\n",
		icard, LLCV2_LOOKUP[ibuf], addr);
}
void initV2Stats(struct TestDescription *td)
{
	updateTstats = v2_updateTstats;
	waitDmaDone  = card_v2_WaitDmaDone;

	EACHCARD_INIT;

	FOREACHCARD{
		u32* init = getVaddr(EACHBUF(td), 0);
		int ibuf;
		
		for (ibuf = 0; ibuf != LLCV2_INIT_LAST; ++ibuf){
			switch(ibuf){
			case LLCV2_INIT_MARKER:
				init[ibuf] = td->V2_BUFS[ibuf];
				break;
			case LLCV2_INIT_AI_HSBT:
			case LLCV2_INIT_AO_HSBS:
			case LLCV2_INIT_DO_HSBS:
			case LLCV2_INIT_STATUS_HSBT:
				if (td->V2_BUFS[ibuf] != V2_NOBUF){
					init[ibuf] = 
						td->V2_BUFS[ibuf] |
						getBusAddr(EACHBUF(td),0);
					debug_prompt(icard, ibuf, init[ibuf]);
				}else{
					init[ibuf] = 0;
				}
				break;
			default:
				init[ibuf] = td->V2_BUFS[ibuf];
				break;
			}
		}
	}
}







