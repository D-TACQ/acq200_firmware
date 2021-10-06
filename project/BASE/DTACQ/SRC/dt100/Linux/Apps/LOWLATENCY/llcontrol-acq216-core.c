/*****************************************************************************
 *
 * File: llcontrol-acq216-core.c
 *
 * $RCSfile: llcontrol-acq216-core.c,v $
 * 
 * Copyright (C) 2001 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *     application implementing the LOW LATENCY CONTROL feature
 *
 * $Id: llcontrol-acq216-core.c,v 1.2 2009/04/02 13:19:01 pgm Exp $
 * $Log: llcontrol-acq216-core.c,v $
 * Revision 1.2  2009/04/02 13:19:01  pgm
 * docs away
 *
 * Revision 1.1  2009/03/26 14:52:03  pgm
 * split sync2v, acq216 from main core
 *
 * Revision 1.1.4.27  2009/03/26 12:40:36  pgm
 * reuse same dac_src for each card, avoid data overrun
 */

/** @file llcontrol-acq216-core.c demonstrates ACQ216 useage. */

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

void setAcq216Parameters(
	struct TestDescription* td, 
	struct LLC200_INIT *buffer, 
	int card,
	int iter)
/** configure card parameters for iteration iter */
{
	if (td->llc200_init_count){
		memcpy(buffer, &td->llc200_init[iter%td->llc200_init_count],
		       sizeof(struct LLC200_INIT));
	}
}

/* WORKTODO: cache issue when prams overwritten, don't see next write?? 
 * so we boost it right out of the way ..
 */
#define PRAMS_OFFSET 0x100000  


static void doApplicationWork216(struct TestDescription* td, u32 offset)
/* copy data to a user buffer. A real app may do more than this ;-) */
{
	EACHCARD_INIT;
	int work_len = td->samples * td->channels*sizeof(short);
	int work_offset = td->iter * work_len;

	FOREACHCARD{
		memcpy(td->work_buf[icard]+work_offset,
		       getVaddr(EACHBUF(td), offset),
		       work_len);

/* now apply shot-based calibration factor to each value */
	}
}



int runSCM216(struct TestDescription* td)
{
/** runs the test SCM mode, ACQ216 only.
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
 */
	struct LLC200_INIT *bbb[MAXCARDS];
#define EACHBBB bbb[icard]
	struct TimingStats tstats[MAXCARDS] = {};
	u32 cmd = LLC_MAKE_DECIM(td->decimation);
	u32 offset = 0;
#define POLLOFF(td) (offset + (td->samples-1) * td_sample_size(td))
	unsigned ipoll = 0;
	u32 cmd2;

	EACHCARD_INIT;

	FOREACHCARD{ /* (1) */
		llSetTlatch(EACHMBX(td), tstats[icard].tlatch = 0xffffffff);
	}
   
	FOREACHCARD{ /* (2) */
		EACHBBB = (struct LLC200_INIT *)getVaddr(EACHBUF(td), 0);
		updateTargetAddr(cmd+LLC_CSR_M_ARM, EACHCARD(td), offset);
	}


	// wait for gate /* (3) */

	while( !llCounterRunning(FIRSTMBX(td), cmd) ){ 
		POLLALERT(ipoll, "Polling for Counter Running\n");
	}

	INIT_TIMER;
	/* WORKTODO - assumes all cards now running */

	
	for ( td->iter = 0; td->iter != td->iterations; ++td->iter ){ /* (4) */
		cmd2 = cmd;
		FOREACHCARD{ /* (5) */
			hbPrimePoll(
				EACHBUF(td), POLLOFF(td), td_sample_size(td));

			if (td->llc200_init_count){
				setAcq216Parameters(
					td, 
					(struct LLC200_INIT *)
					   getVaddr(EACHBUF(td),PRAMS_OFFSET), 
					THIS_CARD, td->iter);
				setMbox(EACHMBX(td), BP_MB_A4, 
					getBusAddr(EACHBUF(td),PRAMS_OFFSET));
				cmd2 |= LLC_CSR_M_LLC200_INIT;
			}
		}

		FOREACHCARD{
			llSetCmd(EACHMBX(td), cmd2+LLC_CSR_M_SOFTCLOCK);
		}
	
		if (td->hb_polling) FOREACHCARD{
			MARK_TIME(0, "hb_polling");
			tstats[icard].hb_poll = hbPoll(
				EACHBUF(td), POLLOFF(td), 
					td_sample_size(td), &user_abort);
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
			doApplicationWork216(td, offset);
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

	G_quit = 1;
	return 0;
}

