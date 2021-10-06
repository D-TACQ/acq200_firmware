/*****************************************************************************
 *
 * File: llcontrol-sync2VRFM-core.c
 *
 * $RCSfile: llcontrol-sync2VRFM-core.c,v $
 * 
 * Copyright (C) 2001 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *     passive monitoring from remote RFM
 *
 * $Id: llcontrol-sync2VRFM-core.c,v 1.1 2011/05/18 13:31:11 pgm Exp $
 * $Log: llcontrol-sync2VRFM-core.c,v $
 * Revision 1.1  2011/05/18 13:31:11  pgm
 * *** empty log message ***
 *
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

/** @file llcontrol-sync2VRFM-core.c demonstrates <b>SYNC_2V with RFM Alternate Target</b> mode. 
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

void appEnterLLC_SYNC_2VRFM(void)
/** set up LLCV2_INIT buffer and enter mode.
 *  Buffer set up as 4K block at offset 0
 * @todo - this overwrites settings from initV2Stats(), initV2Stats is therefore redundant.
*/
{

}

int runSYNC_2VRFM(struct TestDescription *td, int soft_clock)
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
	struct TimingStats tstats[1] = {};
#define icard 0
	u32 cmd = LLC_MAKE_DECIM(td->decimation);

	INIT_TIMER;
	/* WORKTODO - assumes all cards now running */

	for ( td->iter = 0; td->iter != td->iterations; ++td->iter ){ /* (4) */
		                                  /* (6)+(7) */
		MARK_TIME(1, "waitDmaDone() before");
		tstats[icard].tlatch = waitDmaDone(EACHCARD(td));
		MARK_TIME(2, "waitDmaDone() after");

		if (!td->min_latency_test) {        /* (6)+(7) */
			updateTstats(cmd, EACHCARD(td), &tstats[icard]);    
		}

		
		if (td->do_work){
			doApplicationWork(td, 0);
		}


		MARK_TIME(9, "after llCounterRunning check");

		if (td->tlog){          
			tstats[icard].target_poll =
					getMboxPollcount(EACHMBX(td));
			MARK_TIME(10, "10");
			updateTimingStats(
				td->stats_buf[icard], 
				td->iter, 
				&tstats[icard]);
		}       
		TIMER_CHECK_OVERFLOW;
	}

	G_quit = 1;
	return 0;
}

