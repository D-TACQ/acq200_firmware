/*****************************************************************************
 *
 * File: llcontrol-core.h
 *
 * $RCSfile: llcontrol-core.h,v $
 * 
 * Copyright (C) 2001 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *     application implementing the LOW LATENCY CONTROL feature
 *
 * $Id: llcontrol-core.h,v 1.4 2009/09/29 13:09:14 pgm Exp $
 * $Log: llcontrol-core.h,v $
 * Revision 1.4  2009/09/29 13:09:14  pgm
 * RFM emulation (not complete)
 *
 * Revision 1.3  2009/04/02 13:19:01  pgm
 * docs away
 *
 * Revision 1.2  2009/03/28 18:47:39  pgm
 * sync2VAO32 take 1
 *
 * Revision 1.1  2009/03/26 14:52:03  pgm
 * split sync2v, acq216 from main core
 *
 * Revision 1.1.4.27  2009/03/26 12:40:36  pgm
 * reuse same dac_src for each card, avoid data overrun
 */

/** @file llcontrol-core.h llcontrol-core shared definitions. */


#ifndef __LLCONTROL_CORE_H__
#define __LLCONTROL_CORE_H__


#define HOST_TIMING 1
#define IGNORE_COUNTER_STOP 0

#define POLLALERT(ipoll, message) \
    if ( (++ipoll&0xfffff) == 0 ) fprintf( stderr, message )

#if HOST_TIMING > 1
#define INIT_TIMER get_elapsed_microseconds(1)
#define MARK_TIME(t, ident) do { \
        tstats[icard].test_points[t] = get_elapsed_microseconds(0);\
	fprintf(stderr, "MARK_TIME %s\n", ident); \
        } while(0)
#define FOREACHCARD_MARK_TIME(t, ident) FOREACHCARD{ MARK_TIME(t, ident); }
#define TIMER_CHECK_OVERFLOW \
      if (get_elapsed_microseconds(0) > 99000) INIT_TIMER
#warning HOST_TIMING enabled
#elif HOST_TIMING
#define INIT_TIMER get_elapsed_microseconds(1)
#define MARK_TIME(t, ident) do { \
        tstats[icard].test_points[t] = get_elapsed_microseconds(0);\
        } while(0)
#define FOREACHCARD_MARK_TIME(t, ident) FOREACHCARD{ MARK_TIME(t, ident); }
#define TIMER_CHECK_OVERFLOW \
      if (get_elapsed_microseconds(0) > 99000) INIT_TIMER
#warning HOST_TIMING enabled
#else
#define INIT_TIMER
#define MARK_TIME(t, ident)
#define FOREACHCARD_MARK_TIME(t, ident) 
#define TIMER_CHECK_OVERFLOW
#endif


void appEnterLLC_SYNC_ECM(
	int icard, struct MU *mu, struct TestDescription *td);
int runSYNC_ECM(struct TestDescription *td, int soft_clock);



void appEnterLLC_SYNC_2V(
	int icard, struct MU *mu, struct TestDescription *td);

int runSYNC_2V(struct TestDescription *td, int soft_clock);

void appEnterLLC_SYNC_2VAO32(
	int icard, struct MU *mu, struct TestDescription *td);

void appEnterLLC_SYNC_2VRFM(void);

int runSYNC_2VAO32(struct TestDescription *td, int soft_clock);

int runSYNC_2VRFM(struct TestDescription *td, int soft_clock);


int runSCM216(struct TestDescription* td);


extern void (*updateTstats)(
	u32 cmd, struct Card* card, struct TimingStats* tstats);
/** @fn updateTstats() virtual function to update timing stats.
 *  defaults to V1 mbox. 
 */

extern u32 (*waitDmaDone)(struct Card *c);
/** @fn waitDmaDone() virtual function to block until DMA done.
 *  defaults to V1 mbox
 */

u32 commandModifiers(struct TestDescription *td);


static inline void updateTargetAddr(
	u32 cmd, struct Card* card, unsigned offset)
{
	u32 baddr = getBusAddr(card->buf, offset);
	llSetAddr(card->mbx, baddr, cmd );
	PRINTF(2)("llSetAddr [%d] 0x%08x\n", card->slot, baddr);
}


static inline void* td_get_next_dac_data(struct TestDescription* td)
{
	void* dac_data = td->dac_data + td->dac_cursor;

	int new_cursor = td->dac_cursor + DAC_SAMPLE_SIZE;

	if (new_cursor+DAC_SAMPLE_SIZE > td->dac_data_samples*DAC_SAMPLE_SIZE){
		td->dac_cursor = 0;
	}else{
		td->dac_cursor = new_cursor;
	}
	return dac_data;
}

extern void doApplicationWork(struct TestDescription* td, u32 offset);


extern int user_abort;    /** @todo could be set by a signal */


extern void initV2Stats(struct TestDescription* td);

extern void v2_updateTstats(
	u32 cmd, struct Card* card, struct TimingStats* tstats);
/** updates timing stats from embedded host buffer data */


extern u32 card_v2_WaitDmaDone(struct Card* card);
#endif	// __LLCONTROL_CORE_H__
