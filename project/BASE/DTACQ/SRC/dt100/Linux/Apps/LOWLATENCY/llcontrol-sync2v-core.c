/*****************************************************************************
 *
 * File: llcontrol-sync2v-core.c
 *
 * $RCSfile: llcontrol-sync2v-core.c,v $
 * 
 * Copyright (C) 2001 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *     application implementing the LOW LATENCY CONTROL feature
 *
 * $Id: llcontrol-sync2v-core.c,v 1.3 2009/08/06 18:31:42 pgm Exp $
 * $Log: llcontrol-sync2v-core.c,v $
 * Revision 1.3  2009/08/06 18:31:42  pgm
 * README.RFM
 *
 * Revision 1.2  2009/04/02 13:19:01  pgm
 * docs away
 *
 * Revision 1.1  2009/03/26 14:52:03  pgm
 * split sync2v, acq216 from main core
 *
 * Revision 1.1.4.27  2009/03/26 12:40:36  pgm
 * reuse same dac_src for each card, avoid data overrun
 */

/** @file llcontrol-sync2v-core.c demonstrates <b>SYNC_2V</b> mode. 
 - ./set.sync2v
 - llcontrol [opts] -W SYNC2V <a>ECM</a>
 
 - SYNC2V optimises the data transfer by reducing the number of 
    transfers-per-cycle to 2:
  - VI : all the inputs in one block.
  - VO : all the outputs in one block.
   - definitions of offsets in VI, VO are in acq32busprot.h, look for
    <b>LLCV2_</b>
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

#define FLAVOR "ACQ196"
#include "llcontrol-core.h"


static void sync_2v_updateTstats(
	u32 cmd, struct Card* card, struct TimingStats* tstats)
/** updates timing stats from embedded host buffer data */
{
#define SAMPLE_SIZE (96*2)   /* WORKTODO */
	u32* stats = getVaddr(card->buf, card->sync_2v_offset_status_hsbt);
	tstats->tinst = llv2_extend32(stats[LLC_SYNC2V_IN_MBOX2], 
				      stats[LLC_SYNC2V_IN_TINST]);
	tstats->tprocess = LLC_GET_TCYCLE(stats[LLC_SYNC2V_IN_MBOX0]);
}

static u32 card_sync_2v_WaitDmaDone(struct Card* card)
{
	return card->tlatch = llv2WaitDmaDone_2v(card->mbx, 
		  getVaddr(card->buf, card->sync_2v_offset_status_hsbt),
		  card->tlatch
	);
}

void appEnterLLC_SYNC_2V(
	int icard, struct MU *mu, struct TestDescription *td)
/** set up LLCV2_INIT buffer and enter mode.
 *  Buffer set up as 4K block at offset 0
 * @todo - this overwrites settings from initV2Stats(), initV2Stats is therefore redundant.
*/
{
	u32* init_buf = getVaddr(EACHBUF(td), 0);
	u32 target_pa = getBusAddr(EACHBUF(td), 0);
	struct Card* card = &td->cards[icard];

	PRINTF(2)("appEnterLLC_SYNC_2V() va:%p pa:0x%08x\n",
		init_buf, target_pa);

			
	/** set up for single 4K buffer */
	llcv2_hb_offset = 0;
	card->sync_2v_offset_status_hsbt = 
		LLCV2_AI_HSBT + td->channels*sizeof(short);
	card->tlatch = 0;

	/** uses V2 synchronization */
	updateTstats = sync_2v_updateTstats;
	waitDmaDone  = card_sync_2v_WaitDmaDone;

	init_buf[LLCV2_INIT_MARKER] = LLCV2_INIT_MAGIC_MARKER;
	init_buf[LLCV2_INIT_AI_HSBT] = target_pa + LLCV2_AI_HSBT;
	init_buf[LLCV2_INIT_AO_HSBS] = target_pa + LLCV2_AO_HSBS;

	
	enterLLC_SYNC_ECM(
		mu, td->clkpos, td->trpos, 
		td->arg.divisor,td->internal_loopback,
		commandModifiers(td),
		target_pa );
}

int runSYNC_2V(struct TestDescription *td, int soft_clock)
/** runs the test SYNC_2V mode.
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
 - (b)     write data to host side buffer(LLCV2_AO_HSBS) 
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
 */
{
	struct TimingStats tstats[MAXCARDS] = {};
	struct DacBuf {
		MFA mfa;
		void* bbb;
	} dac_buf[MAXCARDS];
#define EACHBBB      (dac_buf[icard].bbb)
#define EACHDAC_BASE (EACHBBB + LLCV2_AO_HSBS)
#define EACHDAC_BASE16 ((u16*)EACHDAC_BASE)
#define V2SETDACS(src, icard) \
        memcpy(EACHDAC_BASE, (src)+(icard)*32, DAC_SAMPLE_SIZE)

	u32 cmd = LLC_MAKE_DECIM(td->decimation);
#define OFFSET 0

	int uses_dacs = td->update_dacs || td->feedback;
	void* dac_data;

	EACHCARD_INIT;

	if (uses_dacs) FOREACHCARD { /* (a) */
		EACHBBB = getVaddr(EACHBUF(td), 0);
		printf("card %d uses_dacs EACHBBB %p "
		       "EACHDAC_BASE %p bus 0x%08x\n",
		       icard, EACHBBB, EACHDAC_BASE,
		       getBusAddr(EACHBUF(td), LLCV2_AO_HSBS) );
	}

	FOREACHCARD{ /* (1) */
		llSetTlatch(EACHMBX(td), tstats[icard].tlatch = 0xffffffff);
		llv2InitDmaDone(
			getVaddr(EACHBUF(td), 
				 EACHCARD(td)->sync_2v_offset_status_hsbt));
	}
   
	FOREACHCARD{ /* (2) */
		updateTargetAddr(cmd+LLC_CSR_M_ARM, EACHCARD(td), OFFSET);
	}

	if (td->update_dacs){
		dac_data = td_get_next_dac_data(td);	
		FOREACHCARD{
			V2SETDACS(dac_data, icard);
		}
	}	

#if 0
	// wait for gate /* (3) */

	while( !llCounterRunning(FIRSTMBX(td), cmd) ){ 
		POLLALERT(ipoll, "Polling for Counter Running\n");
	}
#endif
	INIT_TIMER;
	/* WORKTODO - assumes all cards now running */

	for ( td->iter = 0; td->iter != td->iterations; ++td->iter ){ /* (4) */
		if (soft_clock) FOREACHCARD{ /* (5) */
			llSetCmd(EACHMBX(td), cmd+LLC_CSR_M_SOFTCLOCK);
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
			doApplicationWork(td, OFFSET);
		}

		if (td->feedback) { /* (b.1) */
			u16 *pvin = (u16 *)getVaddr(FIRSTBUF(td), OFFSET);
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
			}
		}else if (td->update_dacs){ /** (b.2) */
			dac_data = td_get_next_dac_data(td);
			FOREACHCARD{
				V2SETDACS(dac_data, icard);
				MARK_TIME(5, "update_dacs");
			}
		}

#if IGNORE_COUNTER_STOP
/** @todo - check counter running in local memory */
	/* check process completion.
         * if hardware_gateoff is employed, then the app would need
         * to make use of a pre-arranged DIO encoding for on/off instead
         */		 
		if (!td->hardware_gate_off &&
                     !llCounterRunning(FIRSTMBX(td), cmd )){          /* (8) */
			fprintf( stderr, "Trigger is off - stop\n" );
			break;
		}
#endif		
		FOREACHCARD_MARK_TIME(9, "after llCounterRunning check");

		if (td->tlog){          
			FOREACHCARD{
				tstats[icard].target_poll =
					getMboxPollcount(EACHMBX(td));
				MARK_TIME(10, "10");
				updateTimingStats(
					td->stats_buf[icard], 
					td->iter, 
					&tstats[icard]);
			}
		}       
		TIMER_CHECK_OVERFLOW;
	}

	G_quit = 1;
	return 0;
#undef EACHMFA
#undef EACHBBB
#undef EACHDAC_BASE
#undef EACHDAC_BASE16
}

