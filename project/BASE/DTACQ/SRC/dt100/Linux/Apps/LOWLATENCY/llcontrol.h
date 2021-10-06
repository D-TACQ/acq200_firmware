/*****************************************************************************
 *
 * File: llcontrol.h
 *
 * $RCSfile: llcontrol.h,v $
 * 
 * Copyright (C) 2001 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *     application implementing the LOW LATENCY CONTROL feture
 *
 * $Id: llcontrol.h,v 1.1.4.17 2009/09/29 13:09:14 pgm Exp $
 * $Log: llcontrol.h,v $
 * Revision 1.1.4.17  2009/09/29 13:09:14  pgm
 * RFM emulation (not complete)
 *
 * Revision 1.1.4.16  2009/04/02 13:19:01  pgm
 * docs away
 *
 * Revision 1.1.4.15  2009/03/28 18:47:39  pgm
 * sync2VAO32 take 1
 *
 * Revision 1.1.4.14  2006/02/23 21:56:40  pgm
 * remove unwanted statics
 *
 * Revision 1.1.4.13  2006/01/15 11:18:18  pgm
 * SYNC_2V
 *
 * Revision 1.1.4.12  2006/01/09 13:40:31  pgm
 * SYNC_2V mode implemented
 *
 * Revision 1.1.4.11  2005/12/07 10:21:15  pgm
 * *** empty log message ***
 *
 * Revision 1.1.4.10  2005/12/06 21:22:40  pgm
 * ACQ216HS channels=2 pulls in antiphase
 *
 * Revision 1.1.4.9  2005/12/06 18:11:31  pgm
 * ACQ216HS (ChannelMask) option
 *
 * Revision 1.1.4.8  2005/11/04 17:26:18  pgm
 * *** empty log message ***
 *
 * Revision 1.1.4.7  2005/10/03 15:03:48  pgm
 * acq216 llc with prams
 *
 * Revision 1.1.4.6  2005/09/25 20:31:50  pgm
 * blocklen
 *
 * Revision 1.1.4.5  2005/08/11 10:02:24  pgm
 * SYNC_ECM - init host side slave memory areas
 *
 * Revision 1.1.4.4  2005/08/01 11:10:24  pgm
 * V2 part 1 running - local status, no hbpoll - ECM 100 acheived
 *
 * Revision 1.1.4.3  2004/12/09 22:31:57  pgm
 * microsecond timestamps do the BIZ - ECM20 here we go
 *
 * Revision 1.1.4.2  2004/12/09 12:58:34  pgm
 * various attempts at repartitioning for speed
 *
 * Revision 1.1.4.1  2004/11/04 07:13:13  pgm
 * merged multicard case
 *
 * Revision 1.1.2.7  2004/09/26 19:50:58  pgm
 * *** empty log message ***
 *
 * Revision 1.1.2.6  2004/09/26 11:32:39  pgm
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
 * Revision 1.1.2.1  2004/09/25 08:46:04  pgm
 * *** empty log message ***
 *
 * Revision 1.1.2.1  2004/09/25 08:44:47  pgm
 * *** empty log message ***
 *
 */


/** @file llcontrol.h shared data structs for llcontrol. */

#ifndef __LLCONTROL_H__
#define __LLCONTROL_H__

extern int verbose;
extern int G_quit;

#define MAXTEST 12

/** defines resources dedicated to each card. */
struct Card {
	int slot;
	struct MU* mbx;           /* mapping to mailboxes on card */
	struct DmaBuffer* buf;    /* mapping of host-side buffer  */

	unsigned tlatch;
	unsigned sync_2v_offset_status_hsbt;
};

/** defines timing of the shot. */
struct TimingStats {
	int iter;         /** iteration. */
	u32 tlatch;       /** Tlatch: time the adc was clocked at. */
	u32 tinst;        /** Tinst: time the counter was read at. */
	u32 tprocess;	  /** Process time on target */
	int hb_poll;      /** Host buffer poll count.
			   *  first card, indicates amount of slack time
	                   * other cards, should be small 
			   */
	int target_poll;  /* expect to be 1 for each card, a big number 
			     indicates that hb_poll mechanism failed */
	u32 test_points[MAXTEST];
};



#define ACQ196_BIGBUF_AREA 0x1000000  /* 16MB buffer assigned to each card */
#define MAXCARDS 16
#define MAXAO32	 6		      /** Max AO32 Slave Cards */

/** defines a test [shot] . */
struct TestDescription {
	enum Mode { 
		M_NULL,                 /**< null test			*/
		M_SCM,                  /**< Soft Clock Master		*/
		M_ECS,                  /**< External Clock Slave	*/
		M_ECM,                  /**< ECM External Clock Master	*/
		M_SYNC_ECM,             /**< ECM, sync AO update	*/
		M_SYNC_2V,              /**< ECM, 2 Vector sync		*/
		M_SYNC_2VAO32,		/**< ECM, 2 Vector sync + AO32 cards */
		M_MEASURE_BRIDGE,       /**< measure bridge latency	*/
		M_SYNC_2VRFM		/**< M_SYNC_2V, monitor at remote */
	} mode;
        
	struct ARG {
		int interval;    /**< SCM: usecs delay	*/
		int divisor;     /**< ECM: external clock divisor	*/
	}
		arg;
	int iter;              /**< current iteration #			*/
            
	int iterations;        /**< #samples to acquire			*/
	int decimation;        /**< {0..15} (0 won't do much!)		*/
	int overwrite;         /**< NOT append				*/
	const char*outfname;    /**< root name of output file (if output rqd)*/
	int do_work;           /**< do work (like read data in RT)	*/
	int clkpos;            /**< 1=> clock positive edge		*/
	int trpos;             /**< 1=> trigger positive edge		*/
	int internal_loopback; 
	int min_latency_test;   /**< trim for abs minimum latency	*/

	int mask_ints;         /**< optionally mask interrupts		*/
	int mask_ints_mask;

	int ncards;
	struct Card cards[MAXCARDS];

	struct TimingStats* stats_buf[MAXCARDS];

	void* work_buf[MAXCARDS];

	int channels;
	int samples;                 /* # samples per capture, default:1 */
	int sample_size;
	int hb_polling;

	int update_dacs;
	char* dac_data_file;
	char* dac_data;
	int dac_data_samples;
	int dac_cursor;

	int feedback;
	int feedback_channel;

	int tlog;                      /* log times if set */
	const char* tlog_binfile;      /* to this file     */
	int sample_offset;             /* bytes to incr tgt point each smpl */

	int hardware_gate_off;         /* gate off is using by hardware, so
					* don't poll for it */
	u32 V2_BUFS[8];	

	int ao32_count;
	int ao32_ids[MAXAO32];

	struct LLC200_INIT *llc200_init;
	int llc200_init_count;
	int acq216hs_ch12ch34_bonding;
};

#define LLC200_INIT_SZ sizeof(struct LLC200_INIT)

#define V2_NOBUF 1   /**< no buffer required,  0 offset may be valid! */

#define IS_SYNC_2V(mode) \
	((mode) == M_SYNC_2V || (mode) == M_SYNC_2VAO32 || \
		(mode) == M_SYNC_2VRFM)
/*
 * define iterator for cards, assumes icard is a volatile variable
 * this is _much_ better represented in C++, see htstream for example
 */

/*
 * iterator variable must be in scope for each call:
 */
#define EACHCARD_INIT int icard

#define EACHCARD(td) (&(td)->cards[icard])
#define EACHMBX(td) ((td)->cards[icard].mbx)
#define EACHBUF(td) ((td)->cards[icard].buf)
#define EACHSLOT(td) ((td)->cards[icard].slot)

/*
 * This is the iterator. WARNING: unsafe bracketing, easy to misuse!
 */
#define FOREACHCARD for (icard = 0; icard != td->ncards; ++icard)

#define FIRSTCARD(td) (&(td)->cards[0])
#define FIRSTMBX(td) ((td)->cards[0].mbx)
#define FIRSTBUF(td) ((td)->cards[0].buf)
#define FIRSTSLOT(td) ((td)->cards[0].slot)

#define THIS_CARD icard
#define MASTER	(icard == 0)

/* 
 * Core module EXPORTS these functions: 
 */

int runTest( struct TestDescription* td );
void setupAbortHandler( struct TestDescription* td );
void setupMonitor( int millisec );
void initCardResource(struct Card* card);



/*
 * Code module IMPORTS these functions
 */


static inline int td_sample_size(struct TestDescription* td)
{
	return td->sample_size;
//	return td->channels * sizeof(short);
}
static inline int td_dss(struct TestDescription* td)
{
	return td_sample_size(td) * td->iterations;
}


static inline int isAcq216(void) {
	return getenv("ACQ216") != 0 || getenv("ACQ216HS") != 0;
}
static inline int isAcq216HS(void) {
	return getenv("ACQ216HS") != 0;
}


void updateTimingStats(
	struct TimingStats* buffer, int iter, struct TimingStats* stats); 
#endif
