/*****************************************************************************
 *
 * File: acq32_drv.c
 *
 * $RCSfile: acq32_drv.c,v $
 * 
 * Copyright (C) 2001 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description: acq32 driver module
 *
 * $Id: acq32_drv.c,v 1.178.4.35 2011/11/11 19:15:39 pgm Exp $
 * $Log: acq32_drv.c,v $
 * Revision 1.178.4.35  2011/11/11 19:15:39  pgm
 * *** empty log message ***
 *
 * Revision 1.178.4.34  2011/11/11 15:25:29  pgm
 * attempt to speed ST_CAPDONE with raised priority for MGR process
 *
 * Revision 1.178.4.33  2009/05/31 07:53:12  pgm
 * *** empty log message ***
 *
 * Revision 1.178.4.32  2008/05/09 09:45:12  pgm
 * cap_status monitor
 *
 * Revision 1.178.4.31  2007/12/08 18:14:35  pgm
 * tighten up error exit codes
 *
 * Revision 1.178.4.30  2007/01/04 09:04:49  pgm
 * minimise printf output
 *
 * Revision 1.178.4.29  2007/01/03 23:16:16  pgm
 * double fork() is good
 *
 * Revision 1.178.4.28  2007/01/03 12:37:42  pgm
 * acq200_muThrottle suppress ints during capture/postprocess to eliminate priority inversion
 *
 * Revision 1.178.4.27  2007/01/02 18:24:19  pgm
 * trim from 106K to 82K
 *
 * Revision 1.178.4.26  2007/01/02 18:18:10  pgm
 * *** empty log message ***
 *
 * Revision 1.178.4.25  2007/01/01 14:39:34  pgm
 * mbox control works
 *
 * Revision 1.178.4.24  2006/04/01 14:40:03  pgm
 * ensures minimum transient time 1s.
 * rarely seems to be needed :-).
 *
 * Revision 1.178.4.23  2005/10/20 20:49:50  pgm
 * avoid ST_STOP race
 *
 * Revision 1.178.4.22  2005/05/16 08:49:58  pgm
 * *** empty log message ***
 *
 * Revision 1.178.4.21  2004/10/21 09:29:16  pgm
 * various fixes to sync post process
 *
 * Revision 1.178.4.20  2004/09/23 21:07:29  pgm
 * B2081
 *
 * Revision 1.178.4.19  2004/08/29 11:48:18  pgm
 * improve number enabled channels handling
 * also ext clk
 *
 * Revision 1.178.4.18  2004/06/05 21:41:15  pgm
 * works with pre=0, trigger=event frig
 *
 * Revision 1.178.4.17  2004/05/16 07:19:34  pgm
 * *** empty log message ***
 *
 * Revision 1.178.4.16  2004/04/25 12:57:46  pgm
 * concurrent queries work
 *
 * Revision 1.178.4.15  2004/04/25 11:40:53  pgm
 * concurrency 1 with logging
 *
 * Revision 1.178.4.14  2004/04/17 20:33:50  pgm
 * refine channel handling
 *
 * Revision 1.178.4.13  2004/04/13 09:35:46  pgm
 * no works with acq196
 *
 * Revision 1.178.4.12  2003/12/28 19:42:52  pgm
 * background parallel transform path
 *
 * Revision 1.178.4.11  2003/12/17 13:29:52  pgm
 * ACQ200_PPJOB
 *
 * Revision 1.178.4.10  2003/12/12 14:19:15  pgm
 * *** empty log message ***
 *
 * Revision 1.178.4.9  2003/11/13 14:24:46  pgm
 * fixes getInternalClock
 *
 * Revision 1.178.4.8  2003/11/07 09:06:10  pgm
 * pre A4, post A3
 *
 * Revision 1.178.4.7  2003/11/05 22:29:57  pgm
 * hooks GATED_TRANSIENT, dio
 *
 * Revision 1.178.4.6  2003/10/30 14:12:36  pgm
 * B2017
 *
 * Revision 1.178.4.5  2003/10/27 20:38:36  pgm
 * reads status from driver
 *
 * Revision 1.178.4.4  2003/10/25 15:12:00  pgm
 * gutted
 *
 * Revision 1.178.4.3  2003/10/19 20:44:29  pgm
 * works with dt100rc
 *
 * Revision 1.178.4.2  2003/09/08 12:54:20  pgm
 * i2o file download works. ship it quick
 *
 * Revision 1.178.4.1  2003/08/29 17:41:02  pgm
 * init pci command handling
 *
 * Revision 1.178  2003/06/03 10:10:02  pgm
 * B1725 pci high address swithc for HTM loc status
 *
 * Revision 1.177  2003/05/31 11:39:01  pgm
 * B1720 fix extra samples acq32 post
 *
 * Revision 1.176  2003/03/05 08:26:39  pgm
 * B1719 reliable acq16 offset adjust
 *
 * Revision 1.175  2003/02/07 09:50:51  pgm
 * B1713 BoxCar
 *
 * Revision 1.174  2002/12/10 14:13:57  pgm
 * B1707 fixes DAC pot tweak
 *
 * Revision 1.173  2002/11/23 20:37:01  pgm
 * nodos
 *
 * Revision 1.172  2002/11/23 20:35:44  pgm
 * B1704 - fixes ext trig pol
 *
 * Revision 1.171  2002/11/03 16:10:24  pgm
 * acq16 offset adj uses cmask, allows status check
 *
 * Revision 1.170  2002/11/02 21:39:10  pgm
 * B1700 ACQ16 cal adj works (I think)
 *
 * Revision 1.169  2002/11/01 20:49:15  pgm
 * env, invert, acq16 offset stub, dmac tests
 *
 * Revision 1.168  2002/08/23 18:27:30  pgm
 * B1691 - runners, err status
 *
 * Revision 1.167  2002/08/21 19:49:20  pgm
 * B1689 - auto trig, sample set adjust OK??
 *
 * Revision 1.166  2002/08/21 09:02:39  pgm
 * B1680 dhAcq12run as fast as it will get
 *
 * Revision 1.165  2002/08/20 14:48:53  pgm
 * fix buffer blat with deadband
 *
 * Revision 1.164  2002/08/20 13:19:41  pgm
 * back out some failed acq16 speed ups
 *
 * Revision 1.163  2002/08/12 18:24:15  pgm
 * fixes spurious boost value
 *
 * Revision 1.162  2002/08/06 19:41:38  pgm
 * lower ICACHE load, add mbox boost
 *
 * Revision 1.161  2002/08/05 19:46:52  pgm
 * fixes int clock (for good)
 *
 * Revision 1.160  2002/07/25 15:13:11  pgm
 * acq16 trig comp now automatic
 *
 * Revision 1.159  2002/07/24 18:07:41  pgm
 * acq16 trigger offset comp - use 1st TW
 *
 * Revision 1.158  2002/07/22 20:01:34  pgm
 * fixes gap in pre gpem P1
 *
 * Revision 1.157  2002/07/18 18:43:05  pgm
 * 44M stub, 64b copy test
 *
 * Revision 1.156  2002/06/13 19:38:22  pgm
 * fix int clk rate bug, then stub 44M as Xilinx broken
 *
 * Revision 1.155  2002/06/10 11:53:50  pgm
 * streaming runs, but the data is crap
 *
 * Revision 1.154  2002/06/09 15:57:56  pgm
 * from handa
 *
 * Revision 1.153  2002/04/26 10:51:59  pgm
 * B1638 finally handles flash num channels correctly
 *
 * Revision 1.152  2002/04/25 10:09:30  pgm
 * B1634 fixes master trig sync
 *
 * Revision 1.151  2002/04/24 12:51:28  pgm
 * B1633 TRMAS for acq16
 *
 * Revision 1.150  2002/04/24 12:03:03  pgm
 * B1632 - qualifies queries at run, rounds clock down, selects #channels on Cal Title
 *
 * Revision 1.149  2002/03/29 00:09:21  pgm
 * B1620 reports cycle, state
 *
 * Revision 1.148  2002/03/28 15:29:42  pgm
 * cycle reset, debounce opts
 *
 * Revision 1.147  2002/03/24 11:54:54  pgm
 * B1617 - works (sometimes)
 *
 * Revision 1.146  2002/03/22 20:58:41  pgm
 * B1612 resets MULTI_TRIG_BIT correctly
 *
 * Revision 1.145  2002/03/21 15:50:13  pgm
 * beautify
 *
 * Revision 1.144  2002/03/21 15:07:11  pgm
 * merge from 1608 - multi trig safe, edge det etc
 *
 * Revision 1.143  2002/03/21 11:54:51  pgm
 * untabify
 *
 * Revision 1.142  2002/03/14 12:03:03  pgm
 * compiled on benbecula
 *
 * Revision 1.141  2002/03/13 21:43:08  pgm
 * correct formatting with emacs (Stroustrup)
 *
 * Revision 1.140  2002/03/10 12:17:19  pgm
 * B1601 - tweak postlen to avoid buffer overrun
 *
 * Revision 1.139  2002/03/09 14:04:25  pgm
 * B1600 fixes bug in rouond up
 *
 * Revision 1.138  2002/03/09 13:50:25  pgm
 * force prelen to be a multiple of block length
 *
 * Revision 1.137  2002/03/09 12:20:39  pgm
 * fix FS44 sink, ensure clean hw ch change
 *
 * Revision 1.136  2002/03/08 19:32:14  pgm
 * B1591 - default no mask - its fast
 *
 * Revision 1.135  2002/03/07 21:22:09  pgm
 * B1587 - LEAN_AND_MEAN trigger callback
 *
 * Revision 1.134  2002/03/06 09:46:48  pgm
 * fix CAS2 diag, mod acq16 ai fifo num sam lut
 *
 * Revision 1.133  2002/02/16 17:53:33  pgm
 * for release, FET Multitrig nearly works
 *
 * Revision 1.132  2002/02/16 07:40:48  pgm
 * attempting to make edge trig faster
 *
 * Revision 1.131  2002/02/14 19:28:15  pgm
 * acq16 multitrig
 *
 * Revision 1.130  2002/02/10 17:56:50  pgm
 * usecs timing reliable over time
 *
 * Revision 1.129  2002/02/09 21:32:11  pgm
 * work on acq16 2/4/8/16 (boots)
 *
 * Revision 1.128  2002/02/08 22:41:24  pgm
 * multi num channel configs for acq16
 *
 * Revision 1.127  2002/02/07 16:56:35  pgm
 * update to new lca
 *
 * Revision 1.126  2002/02/02 15:13:41  pgm
 * usec timing changes from EPFL visit 01/2002
 *
 * Revision 1.125  2002/01/26 21:40:49  pgm
 * multi works, init i-cache fill good
 *
 * Revision 1.124  2002/01/20 20:54:00  pgm
 * threshtrig in debug
 *
 * Revision 1.123  2002/01/18 18:33:10  pgm
 * *** empty log message ***
 *
 * Revision 1.122  2002/01/18 10:05:41  pgm
 * onwards and upwards
 *
 * Revision 1.121  2002/01/13 22:21:58  pgm
 * acq32 fast threshtrig kludge coded
 *
 * Revision 1.120  2002/01/05 21:48:26  pgm
 * fixes DUMDMA stride probs
 *
 * Revision 1.118  2001/11/23 22:28:06  pgm
 * fixes AO ST, direct dumdma speed demo
 *
 * Revision 1.117  2001/11/20 22:23:53  pgm
 * better dregs, i2o in capture, fix AO soft trig
 *
 * Revision 1.116  2001/11/10 17:26:52  pgm
 * better channel counter
 *
 * Revision 1.115  2001/11/04 20:55:30  pgm
 * set User LED is indep contrallable
 *
 * Revision 1.114  2001/10/20 19:42:58  pgm
 * intclock master/slave WORKS
 *
 * Revision 1.113  2001/10/06 21:03:07  pgm
 * CPEM with PXI trig works, AI overflow to cope with
 *
 * Revision 1.112  2001/09/24 12:54:30  pgm
 * GPEM AO works
 *
 * Revision 1.111  2001/09/07 11:31:59  pgm
 * Build 1332 release candidate
 *
 * Revision 1.110  2001/09/03 15:52:45  pgm
 * threshold triggering should now work
 *
 * Revision 1.109  2001/08/31 09:10:20  pgm
 * acq16 has channel mask, works on rearm
 *
 * Revision 1.108  2001/08/27 15:15:40  pgm
 * acq16 - uses lut for nsamples slightly faster
 *
 * Revision 1.107  2001/08/27 12:55:18  pgm
 * acq16 runs again - needs LCA bit change, caveat embedded trigger
 *
 * Revision 1.106  2001/08/20 15:50:47  pgm
 * acq32_setUserLeds nv version of acq32_setLeds()
 *
 * Revision 1.105  2001/08/18 20:00:52  pgm
 * updates route correctly, we get data
 *
 * Revision 1.104  2001/08/17 15:20:11  pgm
 * fail gracefully with duff LCA
 *
 * Revision 1.103  2001/08/13 16:43:23  pgm
 * ll counter extend works
 *
 * Revision 1.102  2001/08/10 20:47:58  pgm
 * fix lll counter overflow, add USER LEDS
 *
 * Revision 1.101  2001/08/03 19:35:58  pgm
 * remove second event to eliminate bad sample problem
 *
 * Revision 1.100  2001/07/14 11:10:49  pgm
 * use fillIcache techs for acq16 too
 *
 * Revision 1.99  2001/07/13 20:48:47  pgm
 * Icache preload reduces init max bin to 18 - ship it quick
 *
 * Revision 1.98  2001/07/13 16:56:26  pgm
 * AO+DO works at last
 *
 * Revision 1.97  2001/07/12 21:29:55  pgm
 * DO, AO now identical - gotta work please
 *
 * Revision 1.96  2001/07/11 21:17:58  pgm
 * AI fifo==32, should fix AO load probs
 *
 * Revision 1.95  2001/07/11 15:07:26  pgm
 * remove stray printf
 *
 * Revision 1.94  2001/07/11 14:32:14  pgm
 * allow local trigger
 *
 * Revision 1.93  2001/06/28 21:11:27  pgm
 * fix phys channe mixup on acq16 flavour fetch, also trheshtrig experiments
 *
 * Revision 1.92  2001/06/27 22:51:43  pgm
 * nodos
 *
 * Revision 1.91  2001/06/27 22:49:41  pgm
 * better status handling, acq16 deeper fifo
 *
 * Revision 1.90  2001/06/23 20:21:07  pgm
 * data fills to end
 *
 * Revision 1.89  2001/06/23 19:03:36  pgm
 * acq16 part 1 captures runs
 *
 * Revision 1.88  2001/06/23 09:24:21  pgm
 * *** empty log message ***
 *
 * Revision 1.87  2001/06/23 09:35:47  pgm
 * goPreamble(), goPostAmble() - take fixups out of the loop
 *
 * Revision 1.86  2001/06/23 07:59:01  pgm
 * remove unguarded printf() in acq32_onsShotCallback()
 *
 * Revision 1.85  2001/06/13 20:44:30  pgm
 * lca-ident, fix pre- channel swap bug
 *
 * Revision 1.84  2001/06/11 19:19:52  pgm
 * better
 *
 * Revision 1.83  2001/06/11 14:47:25  pgm
 * GUT ran thru
 *
 * Revision 1.82  2001/06/09 11:56:28  pgm
 * GUT: lets get to testing
 *
 * Revision 1.81  2001/06/08 20:02:01  pgm
 * GUT builds, fifo_sink testing
 *
 * Revision 1.80  2001/06/04 19:34:49  pgm
 * GUT added (well it compiles anyway)
 *
 * Revision 1.79  2001/05/29 19:51:24  pgm
 * fixed cal, DACpot
 *
 * Revision 1.78  2001/05/21 20:07:53  pgm
 * llc - WIP
 *
 * Revision 1.77  2001/05/20 21:22:50  pgm
 * ll WIP - clocks, no samples
 *
 * Revision 1.76  2001/05/19 19:44:40  pgm
 * LL work in progress
 *
 * Revision 1.75  2001/05/19 10:30:04  pgm
 * low latency compiles
 *
 * Revision 1.74  2001/05/15 17:51:00  pgm
 * new fifo mapping scheme uses wider VM, avoids AI/AO overlap
 *
 * Revision 1.73  2001/05/15 08:50:33  pgm
 * even task handling, slicker status, err detect
 *
 * Revision 1.72  2001/05/15 06:54:11  pgm
 * set AO threshold to fill on 0xa
 *
 * Revision 1.71  2001/05/14 17:49:50  pgm
 * AO works well - shippit quick
 *
 * Revision 1.70  2001/05/14 16:24:24  pgm
 * fifo_source, increments, AO fifo 3/4 250ksps is go
 *
 * Revision 1.69  2001/05/14 11:36:13  pgm
 * AO waveform - histos look OK
 *
 * Revision 1.68  2001/05/13 20:00:45  pgm
 * thrashing around to get AO to go
 *
 * Revision 1.67  2001/05/09 07:46:59  pgm
 * more adds to Wavegen, EmbTrig
 *
 * Revision 1.66  2001/05/08 20:18:14  pgm
 * first cut wavegen in acq32_run() - compiles
 *
 * Revision 1.65  2001/05/07 19:49:11  pgm
 * loads waveforms, uses storeman OK
 *
 * Revision 1.64  2001/05/07 14:23:01  pgm
 * storeman in
 *
 * Revision 1.63  2001/05/07 11:38:37  pgm
 * DAC output - wrk in progress
 *
 * Revision 1.62  2001/05/06 18:46:04  pgm
 * embedded trigger reporting in
 *
 * Revision 1.61  2001/05/05 08:58:37  pgm
 * ext clk works, better debug <dr>, ldcal, channel ass correct
 *
 * Revision 1.60  2001/05/04 20:37:07  pgm
 * big upgrade to improve qwual, match spec
 *
 * Revision 1.59  2001/05/04 09:32:40  pgm
 * consistent pollcat now
 *
 * Revision 1.58  2001/05/01 20:59:22  pgm
 * first pass em trig runs - runs away in fact
 *
 * Revision 1.57  2001/04/29 12:29:18  pgm
 * embedded trigger incl, lets see if it works
 *
 * Revision 1.56  2001/04/23 20:22:17  pgm
 * fixed bugs in setCalDacXXX - no casts here -
 *
 * Revision 1.55  2001/04/20 19:38:12  pgm
 * 66/44 internal clock
 *
 * Revision 1.54  2001/04/19 19:55:25  pgm
 * mbox cal ops, physchan mask
 *
 * Revision 1.53  2001/04/18 20:39:09  pgm
 * built in multi cal facility - needs busprot
 *
 * Revision 1.52  2001/04/16 17:36:42  pgm
 * fifo_sink - first cut
 *
 * Revision 1.51  2001/04/16 15:33:52  pgm
 * works with LCA2
 *
 * Revision 1.50  2001/04/15 09:52:26  pgm
 * SyncSetRoute works
 *
 * Revision 1.49  2001/04/14 19:06:13  pgm
 * remvoe WORTODOS - new LCA is GO
 *
 * Revision 1.48  2001/04/13 20:05:20  pgm
 * new LCA - compiled, not tested, must check WORKTODOS
 *
 * Revision 1.47  2001/03/20 21:51:04  pgm
 * added headers
 *
 *
 \*****************************************************************************/

#define LEAN_AND_MEAN

#define __ACQ32_DRV_C__

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>

#include <assert.h>
#include <libgen.h>

#include "local.h"
#include "dt100api.h"

#include "acq32_drv.h"
#include "acq32_hml.h"
#include "memmap.h"
#include "dataHandling.h"


#include "hal.h"
#include "mbox_slave.h"                 // mbox_abort()
#include "dmac.h"
#include "diags.h"

#include "acq32busprot.h"       //      ACQXX_FLAVOR - WORKTODO bad encapsulation

#include "acq200.h"
#include "acq200_hal.h"

/*
 * ACQ200 port
 */
#include "acq200_hal.h"

unsigned flash_image[1024];

#define FLASH_IMAGE flash_image
#ifdef PGMCOMOUT
#define FLASH_IMAGE (unsigned*)0x410b0074
#endif
/* Instatiate globs here - these be environment variables */

unsigned G_InvertedInputs;
unsigned G_Acq16OffsetAdjust;
unsigned G_UsesNewClock;
const char* G_PostProcessCommand;

static void goPostamble( void );

static struct timeval tstart_run;    /* record time of transition to RUN */


int isAcq16( void )
{
	return 0;
}
int isAcq32( void )
{
	return 0;            /* KLUDGE ALERT!! REMOVE ME PLEASE */
}

int acq32_getAIcount( void )
{
	if (isAcq32()){
		return 32;
	}else if (isAcq16()){
		return 16; /*return acq16_getActiveChannels();*/
	}else{
		return acq200_getNumChannels();
	}
}
int acq32_getAOcount( void )
{
	return isAcq32()? 2: acq200_getAOcount();
}

static int AITRIG_DIX;

void acq32_setAItrigDIx( int DIx )
{
	if ( DIx >=(1<<0) && DIx <= (1<<3) ){
		AITRIG_DIX = DIx;
	}
	dbg( 1, "( %d )\n", AITRIG_DIX );
}
int  __inline acq32_getAItrigDIx( void )
{
	return AITRIG_DIX;
}

static struct CAPTURE_DEF S_;


void acq32_setMode( int mode, int prelen, int postlen )
{


	int n_enabled = numberOfEnabledChannels();
	unsigned max_samples;          // max samples available in store


	if ( n_enabled == 0 ){
		PRINTF( "WARNING: zero channels enabled, baling out\n" );
		return;
	}

    
	max_samples = acq200_getBigBufLen()/sizeof(short)/n_enabled;


	prelen = MIN(prelen, max_samples);
	postlen = MIN(postlen, max_samples-prelen);

	S_.actual_samples.pre       = 
		S_.actual_samples.post  = 0;
	S_.mode = mode;

	dbg(1, "m=%d post=%d pre=%d max=%d\n", 
	    mode, postlen, prelen, max_samples );

	S_.user_request_samples.pre = prelen;
	S_.user_request_samples.post = postlen;

	acq200_setMode(mode, prelen, postlen);
}



int acq32_trigger_is_off( void )
{
	return ((*Q32_DIO_DATA^G_InvertedInputs)&Q32_DIO_DATA_GATE) != 0;
}

int acq32_trigger_is_on( void )
{
	return ((*Q32_DIO_DATA^G_InvertedInputs)&Q32_DIO_DATA_GATE) == 0;
}

int acq32_clock_is_hi( void )
{
	return ((*Q32_DIO_DATA^G_InvertedInputs)&Q32_DIO_DATA_CLK) != 0;
}



int acq32_hasTriggered() 
{
	return S_.has_triggered;
}


static void postamble_cb(unsigned status)
{
	dbg(1,"status:%d (NOT CHECKED)", status);
	goPostamble();
}

void acq32_setArm()
{
	dbg(1, "");

	switch(S_.mode){
	case M_GATED_CONTINUOUS:
	case M_TRIGGERED_CONTINUOUS:
	case M_SOFT_CONTINUOUS:
		acq200_setOneshot(0);
		break;
	default:
		acq200_setOneshot(1);
	}
	gettimeofday(&tstart_run, 0);
	dbg(1, "gettimeofday : %ld", (long)tstart_run.tv_sec);
	mbox_reportState( ST_ARM );
	acq200_setArm(postamble_cb);
}

int acq32_getNumSamples()
{
	return S_.actual_samples.post;
}

int acq32_getPreSamples( void )
{
	return S_.actual_samples.pre;
}




static pid_t pp_pid;
extern void clear_signals(void);


static void spawnPp(const char* cmd, void (*cb)(unsigned status))
{
	/* maybe we need to keep tabs on the manager process too: */
	static pid_t ppm_pid;

	if (ppm_pid != 0){
		int status = 0;
		pid_t ppm_pid2 = wait4(ppm_pid, &status, WNOHANG, NULL);

		dbg(1, "ppm_pid %d ppm_pid2 %d status %d", ppm_pid, ppm_pid2, status);
		
	}

	if ((ppm_pid = fork()) == 0){	/* split off from main */
		clear_signals();	/* we want to wait() */
		if ((pp_pid = fork()) != 0){ /* split again, manager process */
			int status;

			dbg(1,"ppm |%d| wait for %d", getpid(), pp_pid);
			wait(&status);
			dbg(1,"ppm |%d| status %d", getpid(), status);
			cb((unsigned)status);
			exit(__LINE__);
		}else{
			char *cmd2 = malloc(strlen(cmd)+1);
			/* child side */
			int rc;

			assert(cmd2);
			strcpy(cmd2, cmd);

			rc = execl(cmd, basename(cmd2), (char*)NULL);
		
			err("ERROR: failed to exec PP_JOB \"%s\" rc %d\n", cmd, rc);
			if (rc == -1){
				perror( "ERROR:" );
				exit(-1);
			}
		}
	}

	if (ppm_pid < 0){
		err("failed to fork pp manager %d", errno);
	}else{
		dbg(1, "ppm_pid %d", ppm_pid);
	}
}


#ifdef PGMCOMOUT

#define MINTIME 1000000             // min time in usecs 
	
static int delta_tv(struct timeval *tv){
	return tv->tv_sec * 1000000 + tv->tv_usec;
}

static void ensure_min_run_time(void)
/** shots must take a minimum time, otherwise the stuff 
 * underneath gets confused
 */
{
	struct timeval now;
	struct timeval delta;
	int nthrottle = 0;

	while(nthrottle < 100) {
		gettimeofday(&now, 0);
		timersub(&now, &tstart_run, &delta);
		nthrottle++;

		dbg(2, "nthrottle: %d delta : %d", 
		    nthrottle, delta_tv(&delta));

		if (delta_tv(&delta) < MINTIME) {
			usleep(10000);
		}else{
			break;
		}
	}
	if (nthrottle > 1){
		dbg(1, "throttle count %d delta %d", 
		    nthrottle, delta_tv(&delta));
	}
}
#endif

static void onStop(void)
{
#ifdef PGMCOMOUT
	ensure_min_run_time();
#endif
	acq200_setState(ST_STOP);
	mbox_reportState(ST_STOP);
}

int hasStarted(void)
{
	return pp_pid != 0;
}

static int _hasStopped(int wait)
{
	int status;
	pid_t rc;
	int opts = wait? 0: WNOHANG;

	if (pp_pid){
		rc = wait4(pp_pid, &status, opts, NULL);

		if (rc == pp_pid){
			pp_pid = 0;
			onStop();
			return 1;
		}else{
			return 0;
		}
	}
	return 0;
}
int hasStopped(void)
{
	return _hasStopped(0);
}


void stopPostprocess(void)
{
	kill(pp_pid, SIGKILL);
	_hasStopped(1);
}



static void stop_cb(unsigned status)
{
	dbg(1,"status:%d (NOTCHECKED)", status);
	onStop();
}

static void goPostamble( void )
{
	static char *cmd;

	if (!cmd){
		(cmd = getenv( "ACQ200_PPJOB")) ||
		(cmd = getenv( "ACQ200_POSTPROCESS_SCRIPT"));
	}

	if (acq200_captureWasSuccess()){ 
		acq200_setState(ST_POSTPROCESS);
		mbox_reportState(ST_POSTPROCESS);
		if (cmd){
			spawnPp(cmd, stop_cb);
			return;
		}
	}

	onStop();
}


