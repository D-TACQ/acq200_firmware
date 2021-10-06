/*****************************************************************************
 *
 * File: mbox-slave.c
 *
 * $RCSfile: mbox-slave.c,v $
 * 
 * Copyright (C) 1999 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description: slaved from MBOX; implements acq32busprot
 *
 * $Id: mbox-slave.c,v 1.112.4.38 2009/06/05 20:07:09 pgm Exp $
 * $Log: mbox-slave.c,v $
 * Revision 1.112.4.38  2009/06/05 20:07:09  pgm
 * removed filter_state - works faster.
 * No longer needed now device driver is cleaned up
 *
 * Revision 1.112.4.37  2007/11/14 12:24:25  pgm
 * removed mu_throttle
 *
 * Revision 1.112.4.36  2007/05/07 14:19:27  pgm
 * *** empty log message ***
 *
 * Revision 1.112.4.35  2007/04/18 19:44:20  pgm
 * handles AB buffers part 1
 *
 * Revision 1.112.4.34  2007/04/15 21:02:32  pgm
 * processFileDMA works again
 *
 * Revision 1.112.4.33  2007/04/13 17:13:20  pgm
 * B2184 hopefully fix hostwin init - both for usual [init] case and for reruns
 *
 * Revision 1.112.4.32  2007/04/03 21:58:34  pgm
 * handles mbox interrupts
 *
 * Revision 1.112.4.31  2007/01/05 12:56:49  pgm
 * restore setDIO to old mbox, also abort
 *
 * Revision 1.112.4.30  2007/01/03 23:16:16  pgm
 * double fork() is good
 *
 * Revision 1.112.4.29  2007/01/03 12:31:37  pgm
 * MU interrupts rock! - .7s for 100 commands.
 *
 * Revision 1.112.4.28  2007/01/02 18:24:19  pgm
 * trim from 106K to 82K
 *
 * Revision 1.112.4.27  2007/01/02 18:18:11  pgm
 * *** empty log message ***
 *
 * Revision 1.112.4.26  2007/01/01 14:39:35  pgm
 * mbox control works
 *
 * Revision 1.112.4.25  2005/10/20 20:49:50  pgm
 * avoid ST_STOP race
 *
 * Revision 1.112.4.24  2005/08/09 20:09:13  pgm
 * *** empty log message ***
 *
 * Revision 1.112.4.23  2005/08/09 19:45:50  pgm
 * wildcard A4 at llc start
 *
 * Revision 1.112.4.22  2004/12/17 09:26:20  pgm
 * pci getNumSamples uses A3=post, NOT A3=total
 *
 * Revision 1.112.4.21  2004/11/01 12:22:27  pgm
 * 2100
 *
 * Revision 1.112.4.20  2004/10/23 14:17:45  pgm
 * rsh single path style
 *
 * Revision 1.112.4.19  2004/10/21 09:29:16  pgm
 * various fixes to sync post process
 *
 * Revision 1.112.4.18  2004/09/23 21:07:29  pgm
 * B2081
 *
 * Revision 1.112.4.17  2004/08/29 11:48:18  pgm
 * improve number enabled channels handling
 * also ext clk
 *
 * Revision 1.112.4.16  2004/02/19 22:26:23  pgm
 * setMode getMode debugged
 *
 * Revision 1.112.4.15  2004/02/15 22:04:57  pgm
 * *** empty log message ***
 *
 * Revision 1.112.4.14  2004/02/15 21:39:20  pgm
 * local slave command handling
 *
 * Revision 1.112.4.13  2003/12/28 19:42:52  pgm
 * background parallel transform path
 *
 * Revision 1.112.4.12  2003/12/12 14:19:15  pgm
 * *** empty log message ***
 *
 * Revision 1.112.4.11  2003/11/13 14:24:46  pgm
 * fixes getInternalClock
 *
 * Revision 1.112.4.10  2003/11/08 14:48:16  pgm
 * channel mask, pre post sorted
 *
 * Revision 1.112.4.9  2003/11/07 09:06:11  pgm
 * pre A4, post A3
 *
 * Revision 1.112.4.8  2003/11/05 22:29:57  pgm
 * hooks GATED_TRANSIENT, dio
 *
 * Revision 1.112.4.7  2003/10/27 20:38:37  pgm
 * reads status from driver
 *
 * Revision 1.112.4.6  2003/10/25 15:12:01  pgm
 * gutted
 *
 * Revision 1.112.4.5  2003/10/19 20:44:30  pgm
 * works with dt100rc
 *
 * Revision 1.112.4.4  2003/09/08 12:54:20  pgm
 * i2o file download works. ship it quick
 *
 * Revision 1.112.4.3  2003/09/07 16:30:39  pgm
 * basic i2o
 *
 * Revision 1.112.4.2  2003/09/01 08:17:37  pgm
 * i2o_init_buffes() hook
 *
 * Revision 1.112.4.1  2003/08/29 17:41:03  pgm
 * init pci command handling
 *
 * Revision 1.112  2003/06/02 21:18:28  pgm
 * B1724 htstream V2
 *
 * Revision 1.111  2002/11/23 20:37:01  pgm
 * nodos
 *
 * Revision 1.110  2002/11/23 20:35:44  pgm
 * B1704 - fixes ext trig pol
 *
 * Revision 1.109  2002/10/31 10:42:38  pgm
 * fixes getEvent, attempts fix null P1
 *
 * Revision 1.108  2002/08/23 18:27:30  pgm
 * B1691 - runners, err status
 *
 * Revision 1.107  2002/08/21 19:49:20  pgm
 * B1689 - auto trig, sample set adjust OK??
 *
 * Revision 1.106  2002/08/06 19:41:38  pgm
 * lower ICACHE load, add mbox boost
 *
 * Revision 1.105  2002/08/05 19:49:34  pgm
 * status change int only on request
 *
 * Revision 1.104  2002/06/10 20:02:41  pgm
 * B1666 multi samples per pci packet
 *
 * Revision 1.103  2002/06/10 11:53:50  pgm
 * streaming runs, but the data is crap
 *
 * Revision 1.102  2002/04/26 10:51:59  pgm
 * B1638 finally handles flash num channels correctly
 *
 * Revision 1.101  2002/04/24 12:03:03  pgm
 * B1632 - qualifies queries at run, rounds clock down, selects #channels on Cal Title
 *
 * Revision 1.100  2002/03/29 00:09:21  pgm
 * B1620 reports cycle, state
 *
 * Revision 1.99  2002/03/28 15:29:42  pgm
 * cycle reset, debounce opts
 *
 * Revision 1.98  2002/03/25 08:56:05  pgm
 * cycle count
 *
 * Revision 1.97  2002/03/24 11:54:54  pgm
 * B1617 - works (sometimes)
 *
 * Revision 1.96  2002/03/21 15:07:11  pgm
 * merge from 1608 - multi trig safe, edge det etc
 *
 * Revision 1.95  2002/03/21 11:54:51  pgm
 * untabify
 *
 * Revision 1.94  2002/03/14 12:03:03  pgm
 * compiled on benbecula
 *
 * Revision 1.93  2002/03/13 21:43:09  pgm
 * correct formatting with emacs (Stroustrup)
 *
 * Revision 1.92  2002/03/07 21:19:53  pgm
 * latched abort is more reliable during startup
 *
 * Revision 1.91  2002/03/06 09:49:48  pgm
 * allows setDIO in ARM (NB ++inefficient)
 *
 * Revision 1.90  2002/02/14 19:30:35  pgm
 * set trigger DIx
 *
 * Revision 1.89  2002/02/10 17:56:51  pgm
 * usecs timing reliable over time
 *
 * Revision 1.88  2002/02/09 21:32:12  pgm
 * work on acq16 2/4/8/16 (boots)
 *
 * Revision 1.87  2002/02/09 09:31:05  pgm
 * war on warnings
 *
 * Revision 1.86  2001/11/23 22:28:07  pgm
 * fixes AO ST, direct dumdma speed demo
 *
 * Revision 1.85  2001/11/05 22:28:41  pgm
 * TEST messages, fix gut buffer overrun bug
 *
 * Revision 1.84  2001/11/04 23:35:10  pgm
 * host side fw debugs
 *
 * Revision 1.83  2001/10/28 21:49:32  pgm
 * llcontrol 24 bit rolloever fixed
 *
 * Revision 1.82  2001/10/20 19:42:59  pgm
 * intclock master/slave WORKS
 *
 * Revision 1.81  2001/10/12 20:52:22  pgm
 * edge trigger done
 *
 * Revision 1.80  2001/10/07 10:19:59  pgm
 * GPME looking good build 1370 checkin
 *
 * Revision 1.79  2001/10/06 21:03:07  pgm
 * CPEM with PXI trig works, AI overflow to cope with
 *
 * Revision 1.78  2001/10/01 21:19:58  pgm
 * fix incoming error message
 *
 * Revision 1.77  2001/09/30 14:08:20  pgm
 * i2o boot time - two commands
 *
 * Revision 1.76  2001/09/08 13:54:36  pgm
 * Build 1333 - fixes E1 overrun, opt i2o incoming
 *
 * Revision 1.75  2001/09/03 15:52:45  pgm
 * threshold triggering should now work
 *
 * Revision 1.74  2001/09/02 21:30:19  pgm
 * multi thresh trig, vranging
 *
 * Revision 1.73  2001/08/31 13:29:38  pgm
 * E1 handling OK, first pass
 *
 * Revision 1.72  2001/08/20 15:50:48  pgm
 * acq32_setUserLeds nv version of acq32_setLeds()
 *
 * Revision 1.71  2001/08/19 13:25:41  pgm
 * fireEvent works
 *
 * Revision 1.70  2001/08/18 20:00:52  pgm
 * updates route correctly, we get data
 *
 * Revision 1.69  2001/08/17 15:20:11  pgm
 * fail gracefully with duff LCA
 *
 * Revision 1.68  2001/08/10 20:47:58  pgm
 * fix lll counter overflow, add USER LEDS
 *
 * Revision 1.67  2001/07/13 16:56:26  pgm
 * AO+DO works at last
 *
 * Revision 1.66  2001/07/12 21:29:56  pgm
 * DO, AO now identical - gotta work please
 *
 * Revision 1.65  2001/07/11 21:17:58  pgm
 * AI fifo==32, should fix AO load probs
 *
 * Revision 1.64  2001/06/28 21:11:28  pgm
 * fix phys channe mixup on acq16 flavour fetch, also trheshtrig experiments
 *
 * Revision 1.63  2001/06/27 22:51:44  pgm
 * nodos
 *
 * Revision 1.62  2001/06/27 22:49:42  pgm
 * better status handling, acq16 deeper fifo
 *
 * Revision 1.61  2001/06/23 19:03:37  pgm
 * acq16 part 1 captures runs
 *
 * Revision 1.60  2001/06/11 14:47:26  pgm
 * GUT ran thru
 *
 * Revision 1.59  2001/06/09 20:28:59  pgm
 * GUT - in testing
 *
 * Revision 1.58  2001/06/02 13:42:25  pgm
 * *** empty log message ***
 *
 * Revision 1.57  2001/06/02 08:43:55  pgm
 * reserve AO, GUT STUBS
 *
 * Revision 1.56  2001/06/01 20:18:58  pgm
 * add gut stubs
 *
 * Revision 1.55  2001/05/20 21:22:50  pgm
 * ll WIP - clocks, no samples
 *
 * Revision 1.54  2001/05/19 19:44:40  pgm
 * LL work in progress
 *
 * Revision 1.53  2001/05/19 12:59:56  pgm
 * low latency setup - done
 *
 * Revision 1.52  2001/05/14 11:36:13  pgm
 * AO waveform - histos look OK
 *
 * Revision 1.51  2001/05/13 20:00:46  pgm
 * thrashing around to get AO to go
 *
 * Revision 1.50  2001/05/09 07:46:59  pgm
 * more adds to Wavegen, EmbTrig
 *
 * Revision 1.49  2001/05/07 11:38:38  pgm
 * DAC output - wrk in progress
 *
 * Revision 1.48  2001/05/04 20:37:08  pgm
 * big upgrade to improve qwual, match spec
 *
 * Revision 1.47  2001/04/19 19:55:25  pgm
 * mbox cal ops, physchan mask
 *
 * Revision 1.46  2001/04/14 20:37:50  pgm
 * CPCI routing coded, needs testing
 *
 * Revision 1.45  2001/04/14 19:06:13  pgm
 * remvoe WORTODOS - new LCA is GO
 *
 * Revision 1.44  2001/04/01 12:31:17  pgm
 * get channels right, test dist get s frame str
 *
 * Revision 1.43  2001/04/01 08:25:55  pgm
 * DUMDMA works - synch on post fixes
 *
 * Revision 1.42  2001/03/30 20:20:13  pgm
 * UDMA first cut working
 *
 * Revision 1.41  2001/03/26 20:25:07  pgm
 * fetch data compiled not tested
 *
 * Revision 1.40  2001/03/25 19:20:28  pgm
 * first cut i2o command handling
 *
 * Revision 1.39  2001/03/24 20:47:17  pgm
 * rejig I2O, add incoming hooks (outgoing still works)
 *
 * Revision 1.38  2001/03/19 23:47:22  pgm
 * updated d-bug help, nodos
 *
 * Revision 1.37  2001/03/17 12:50:26  pgm
 * builds in restructured tree
 *
 * Revision 1.36  2001/03/04 21:09:29  pgm
 * LL kludge, Data Trigger 99kHz
 *
 * Revision 1.35  2001/02/17 21:25:15  pgm
 * LL kludge part 1
 *
 * Revision 1.34  2000/11/26 21:59:10  pgm
 * fix spurious asseriotn on CAL, warnings
 *
 * Revision 1.33  2000/11/04 23:49:20  pgm
 * works pre==0, better ID
 *
 * Revision 1.32  2000/10/29 20:43:33  pgm
 * fix bigdump end buffer problem
 *
 * Revision 1.31  2000/10/08 19:40:51  pgm
 * allows internal clock with ext gating
 *
 * Revision 1.30  2000/10/07 20:09:30  pgm
 * setModeTriggeredContinuous implemented
 *
 * Revision 1.29  2000/10/01 21:27:41  pgm
 * bigdump, getFwrev work
 *
 * Revision 1.28  2000/10/01 08:16:10  pgm
 * improved GATED_CONTINUOUS, flash prog
 *
 * Revision 1.27  2000/09/05 08:26:40  pgm
 * streaming works! ship it quick
 *
 * Revision 1.26  2000/08/14 19:11:29  pgm
 * nodos
 *
 * Revision 1.25  2000/08/14 19:10:53  pgm
 * bulked messages works (partly)
 *
 * Revision 1.24  2000/06/25 19:29:27  pgm
 * acq32 push nearly works, with PIO not DMAC :-(
 *
 * Revision 1.23  2000/06/25 06:16:37  pgm
 * hacked in streaming to dataHandling et al
 *
 * Revision 1.22  2000/06/24 15:17:29  pgm
 * I2O rools OK
 *
 * Revision 1.21  2000/06/11 14:36:16  pgm
 * SC cal pot works
 *
 * Revision 1.20  2000/06/11 13:18:20  pgm
 * queries in ST_RUN allowed, also Abort works
 *
 * Revision 1.19  2000/06/05 21:33:19  pgm
 * SC1 driver stubs filled
 *
 * Revision 1.18  2000/06/04 19:52:40  pgm
 * STUB SC handling compiles
 *
 * Revision 1.17  2000/03/26 21:06:13  pgm
 * interrupt on status change implemented
 *
 * Revision 1.16  2000/01/08 22:14:09  pgm
 * status interrupt, GET_MASK
 *
 * Revision 1.15  2000/01/04 11:19:49  pgm
 * removed mbox delay, fifo reset robust
 *
 * Revision 1.14  1999/11/29 20:40:16  pgm
 * ship it Quick
 *
 * Revision 1.13  1999/11/18 13:42:15  pgm
 * revised DATA_ROUTE should work wrapped
 *
 * Revision 1.12  1999/11/16 10:52:41  pgm
 * GATED_CONTINUOUS runs
 *
 * Revision 1.11  1999/11/14 18:47:20  pgm
 * update
 *
 * Revision 1.9  1999/11/12 18:11:26  pgm
 * default mbox init, LEDS
 *
 * Revision 1.3  1999/10/30 21:00:37  pgm
 * pgm backed up
 *
 * Revision 1.1  1999/10/22 19:48:24  pgm
 * first pass mbox control, compiles gcc
 *
 * Revision 1.1  1999/10/22 16:26:49  pgm
 * first entry to cvs
 *
 *
\*****************************************************************************/


#include <stdio.h>
#include <stdlib.h>

#include "local.h"
#include "dt100api.h"
#include "acq32busprot.h"

#include "acq32_drv.h"
#include "acq32cpci_drv.h"

#include "dataHandling.h"

#include "acq32_hml.h"

#include "stream.h"

#include "hal.h"

#include "mbox_slave.h"

#include <errno.h>
#include <sys/select.h>
#include <sys/time.h>

static int G_command_interrupt_ack;

static int mbox_abort_received = FALSE;


static void ack( void )
{
	*mbox( BP_MB_COMMAND ) |= BP_CI_ACK;
	if ( G_command_interrupt_ack&BP_INT_COMMAND_ACK ){
		acq32_assertPciInterrupt( BP_INT_COMMAND_ACK );
	}
}

static void nack( unsigned command )
{
	PRINTF( "mbox-slave:NACK 0x%08x\n", command );
	if ( G_command_interrupt_ack&BP_INT_COMMAND_ACK ){
		acq32_assertPciInterrupt( BP_INT_COMMAND_ACK );
	}
}

static enum STATE _mbox_reportState(void)
{
	enum STATE state = acq200_getState();
	unsigned status     = *mbox( BP_MB_STATUS );
	unsigned new_status = BP_SET_STATE(state);
	int has_changed     = (status&BP_SET_STATE(0xffU))^new_status;

	if (has_changed){
		status &= ~BP_SET_STATE( 0xffU );
		status |= BP_SET_STATE( state );

		switch(state){
		case ST_UNKNOWN:
			return state;
		case ST_ARM: {
			unsigned cycle = BP_GET_CYCLE( status );
    
			status &= ~ BP_SET_CYCLE( 0xf );
			status |= BP_SET_CYCLE( ++cycle );
		}	break;
		case ST_STOP:
			break;
		default:
			;
		}

		*mbox( BP_MB_STATUS ) = status;

		if ( G_command_interrupt_ack&BP_INT_STATUS_CHANGE ){
			acq32_assertPciInterrupt( BP_INT_STATUS_CHANGE );
		}
	}

	return state;
}

enum STATE mbox_getState(void)
{
	return _mbox_reportState();
}



void mbox_reportState( enum STATE state )
{
	_mbox_reportState();
}


enum MODE G_mode;

void mbox_reportMode( enum MODE mode )
{
	unsigned status = *mbox( BP_MB_STATUS );

	status &= ~BP_SET_MODE( 0x7fU ) ;
	status &= ~BP_SET_CYCLE( 0xf );
	status |= BP_SET_MODE( mode );

	G_mode = mode;
	*mbox( BP_MB_STATUS ) = status;
}

void mbox_reportMsecs( void )
{
	static unsigned msecs;
	unsigned status = *mbox( BP_MB_STATUS );
//    unsigned msecs = msecTimerGetValue();
	unsigned mask = 0x0000ffff;
    
	status &= ~BP_SET_HEARTBEAT( mask );
	status |= BP_SET_HEARTBEAT( ++msecs );
    
	*mbox( BP_MB_STATUS ) = status;
}


static int mboxSetStreaming()
{
#ifdef PGMCOMOUT
	return acq32_setStreaming( 
		BP_GET_A1(*mbox(BP_MB_COMMAND)), 
		BP_GET_A2(*mbox(BP_MB_COMMAND)) );
#else
	return SUCCESS;
#endif
}

#define MBOX_PRE  (*mbox(BP_MB_A4))
#define MBOX_POST (*mbox(BP_MB_A3))

int  mbox_doSetMode( enum MODE mode, int pre, int post )
{
	dbg(1, "mode %d pre %d post %d", mode, pre, post);

	acq32_setMode( mode, pre, post );
	mbox_reportMode( mode );
	return SUCCESS;
}
static int mbox_setMode()
{
	enum MODE mode = (enum MODE)BP_GET_A1( *mbox(BP_MB_COMMAND) );

	PRINTF( "mbox_setMode()\n" );

	switch(mode){
	case M_GATED_TRANSIENT:
	case M_SOFT_TRANSIENT:
		return mbox_doSetMode(mode, 0, MBOX_POST);
	default:
		return mbox_doSetMode(mode, MBOX_PRE, MBOX_POST);
	}
}

static void mbox_configureTrigger( void )
{
	unsigned sel = BP_GET_A2( *mbox(BP_MB_COMMAND) );
	unsigned dix = sel&~(BP_FC_SM_FLAGS_DEB|BP_FC_SM_FLAGS_NEM);
#ifdef PGMCOMOUT    
	unsigned deb = (sel&BP_FC_SM_FLAGS_DEB) != 0;
	unsigned nem = (sel&BP_FC_SM_FLAGS_NEM) != 0;


	acq32_setTrigDebounce( deb );
	acq16_disableEmbeddedTrigger( nem );
#endif
	acq32_setAItrigDIx( dix );
}
static int mbox_setModeGatedContinuous()
{
	PRINTF( "mbox_setModeGatedContinuous()\n" );
    
	mbox_configureTrigger();
	return mbox_doSetMode(M_GATED_CONTINUOUS, MBOX_PRE, MBOX_POST);
}


static int mbox_setModeTriggeredContinuous()
{
	PRINTF( "mbox_setModeTriggeredContinuous()\n" );
    
	mbox_configureTrigger();
	return mbox_doSetMode(M_TRIGGERED_CONTINUOUS, MBOX_PRE, MBOX_POST);
}

int mbox_setArm()
{
	PRINTF( "mbox_setArm()\n" );
	mbox_abort_received = 0;
	ack();
	if (hasStarted() && !hasStopped()){
		stopPostprocess();
	}
	acq32_setArm();
	return SUCCESS;
}

static int mbox_setAbort(void)
{
	PRINTF( "mbox_setAbort()\n" );
	acq200_setAbort();
	mbox_reportState( ST_STOP );
	return SUCCESS;
}

static int mbox_setMask( void )
{
	acq32_setChannelMask(*mbox( BP_MB_A3 ));
	return SUCCESS;
}


static int mboxDoSetDataThreshold( void )
{
	short channel = BP_GET_A1( *mbox( BP_MB_COMMAND ) );
	short trigger_below = (short)*mbox( BP_MB_A3 );
	short trigger_above = (short)*mbox( BP_MB_A4 );

	if ( channel == 'X' ){
		int ichan;

		for ( ichan = 0; ichan != acq32_getAIcount(); ichan++ ){
			acq32_setDataThreshold( ichan, trigger_below, trigger_above );
		}
	}else{
		acq32_setDataThreshold( channel, trigger_below, trigger_above );
	}
	return SUCCESS;
}


static int mbox_getMask()
{
	*mbox( BP_MB_A3 ) = acq32_getChannelMask();
	*mbox( BP_MB_COMMAND ) |= BP_CI_A3;
	return SUCCESS;
}

static int mbox_getDio()
{
	*mbox( BP_MB_A3 ) = acq200_getDio();
	*mbox( BP_MB_COMMAND ) |= BP_CI_A3;
	return SUCCESS;
}
static int mbox_getNumSamples()
{
	PRINTFL(2)( "mbox_getNumSamples %d\n", acq200_getNumSamples() );
	*mbox( BP_MB_A3 ) = acq200_getPostSamples();
	*mbox( BP_MB_A4 ) = acq200_getPreSamples();
	*mbox( BP_MB_COMMAND ) |= BP_CI_A3;
    
	return SUCCESS;
}

static int mbox_getNChannels()
{
	unsigned command = *mbox( BP_MB_COMMAND );  
    
	command &= ~ ( BP_SET_A1(0xff)|BP_SET_A2(0xff) );
    
	command |= BP_SET_A1( acq32_getAIcount() ) + 
		BP_SET_A2( acq32_getAOcount() );
    
	*mbox( BP_MB_COMMAND ) = command;
	return SUCCESS;
}

static int mbox_getRange()
{
	unsigned command = *mbox( BP_MB_COMMAND );  
    
	command &= ~ ( BP_SET_A1(0xff)|BP_SET_A2(0xff) );
    
	// WORKTODO ... this could vary!!
	// acq32 - depends on SC option
	// acq16 - comes ina variety of flavors
    
	if ( isAcq32() ){
		command |= BP_SET_A1( BPFC_RANGE_10_10 )+BP_SET_A2( BPFC_RANGE_10_10 );
	}else if ( isAcq16() ){
		command |= BP_SET_A1( BPFC_RANGE_2p5_2p5 );    
	}else{
		VRANGE ivr;
		u8 a1;

		acq200_getVRange(ivr);
		if (ivr[0] < 1.0f || ivr[0] > 5.0f){
			a1 = BPFC_RANGE_10_10;       /* defacto default */
		}else if (ivr[0] < 2.0f ){
			a1 = BPFC_RANGE_01_01;
		}else if (ivr[0] < 2.5f){
			a1 = BPFC_RANGE_2p5_2p5;
		}else if (ivr[0] <= 5.0f){
			a1 = BPFC_RANGE_05_05;
		}else{
			a1 = BPFC_RANGE_UNKNOWN;     /* "doesn't happen" */
		}
		command |= BP_SET_A1(a1);
	}
    
	*mbox( BP_MB_COMMAND ) = command;

	return SUCCESS;
}

static int mbox_getInternalClock()
{
	*mbox( BP_MB_A3 ) = acq200_getInternalClock();
	// MASTER in A2 - WORKTODO
	return SUCCESS;
}


static int mboxSpin()
{
	printf( "spinning\n" );
	for (;; )
		;
	return SUCCESS; // not reached
}

// hack to validate mfa's later (not used)
unsigned G_hb_base;
unsigned G_hb_len;

static int mboxSetHostDmabuf()
{
	int old_debug = P_debug;
    
	P_debug = 2;
	i2o_initBuffers( *mbox( BP_MB_A3 ), *mbox( BP_MB_A4 ) );

	G_hb_base = *mbox( BP_MB_A3 );
	G_hb_len  = *mbox( BP_MB_A4 );
        
	P_debug = old_debug;        
    
	init_host_window_mapping(G_hb_len);
	return SUCCESS;
}

void acq32_setCommandInterruptAck( int mask )
{
	G_command_interrupt_ack = mask;
	PRINTFL(2)( "commandInterruptAck( %x )\n", mask );
}


static int mboxDoBigDump( void )
{
	return SUCCESS;
}

static int mbox_setModeLowLatencyControl( void )
{
	unsigned a1 = BP_GET_A1(*mbox( BP_MB_COMMAND ));

	if ( (a1&BP_FC_SET_MODE_LLC_SOFTCLOCK) ){

		acq32_setupLowLatencyControlSoftClock( 
			(a1&BP_FC_SET_MODE_LLC_CLKPOL_POS)    != 0,
			(a1&BP_FC_SET_MODE_LLC_TRPOL_POS)     != 0,
			(a1&BP_FC_SET_MODE_LLC_INTSOFT_CLK)   != 0
			);
	}else{
		acq32_setupLowLatencyControlExtClock(
			(a1&BP_FC_SET_MODE_LLC_CLKPOL_POS)   != 0,
			(a1&BP_FC_SET_MODE_LLC_TRPOL_POS)    != 0,
			(a1&BP_FC_SET_MODE_LLC_INTDIV_CLK)   != 0,
			*mbox( BP_MB_A3 )                    // divisor             
			);
	}
	llSetRtClockTiming ( (a1&BP_FC_SET_RTCLOCK_TIMING) != 0 );

	*mbox( BP_MB_COMMAND ) = 0;
	*mbox( BP_MB_A3 ) = 0;

	if ((a1&BP_FC_SET_LLCV2_INIT) == 0){
		*mbox(BP_MB_A4) = 0;
	}

	*mbox( BP_MB_STATUS ) = 0;

	PRINTF( "going into LLC %s csr %x\n", 
		(a1&BP_FC_SET_MODE_LLC_SOFTCLOCK)? "SOFT": "EXT", 
		*mbox( BP_MB_COMMAND )  );
        
	acq32_doLowLatencyControl();        // does a SACK, which is an ack()
        
	PRINTF( "leaving LLC\n" );
	return SUCCESS;
}

static int mbox_getFwrev( void )
{
	ack();
	PRINTF( "mbox_getFwrev()\n" );
	acq32_getFwrev();
	return SUCCESS;
}

static int mbox_getConfig( void )
{
	ack();
	acq32_getConfig();
	return SUCCESS;
}

static int mbox_getCalInfo( void )
{
	ack();
	acq32_getCalInfo();
	return SUCCESS;
}


static int mbox_getCaptureStats( void )
{
	ack();
	acq32_getCaptureStats( BP_GET_A1(*mbox(BP_MB_COMMAND)) );
	return SUCCESS;
}


static int mbox_cpci_setRoute( void ) 
{
	enum Q32C_SR_FUNC func =(enum Q32C_SR_FUNC)BP_GET_A1(*mbox(BP_MB_COMMAND));
	enum Q32C_SR_DEST dest =(enum Q32C_SR_DEST)BP_GET_A2(*mbox(BP_MB_COMMAND));
    
	if ( IN_RANGE( func, Q32C_SR_FUNC_DI0, Q32C_SR_FUNC_DI5 ) &&
	     IN_RANGE( dest, 0, 0xff )                               ){
		acq32cpci_setRoute( func, dest );
		return SUCCESS;
	}else{
		return !SUCCESS;
	}
}

static int convert_ACQ32_DIOSELECT_to_INT_CLKSRC( int DI_mask, int* DI_select )
// return true if if a DI line has been selected
{
	switch( DI_mask ){
	case ACQ32_DIO_NONE:
	default:
		return 0;
	case ACQ32_DI_0:
		*DI_select = INT_CLKSRC_DI0; return 1;
	case ACQ32_DI_1:
		*DI_select = INT_CLKSRC_DI1; return 1;
	case ACQ32_DI_2:
		*DI_select = INT_CLKSRC_DI2; return 1;
	case ACQ32_DI_3:
		*DI_select = INT_CLKSRC_DI3; return 1;
	case ACQ32_DI_4:
		*DI_select = INT_CLKSRC_DI4; return 1;
	case ACQ32_DI_5:
		*DI_select = INT_CLKSRC_DI5; return 1;
	}
}

static int convert_ACQ32_DIOSELECT_to_CLKSEL( int DO_mask, int* DO_select )
// return true is a DO line has been selected
{
	switch( DO_mask ) {
	case ACQ32_DIO_NONE:
	default:
		return 0;
	case ACQ32_DO_0:
		*DO_select = INT_CLKSEL_DO0; return 1;
	case ACQ32_DO_1:
		*DO_select = INT_CLKSEL_DO1; return 1;
	case ACQ32_DO_2:
		*DO_select = INT_CLKSEL_DO2; return 1;
	case ACQ32_DO_3:
		*DO_select = INT_CLKSEL_DO3; return 1;
	case ACQ32_DO_4:
		*DO_select = INT_CLKSEL_DO4; return 1;
	case ACQ32_DO_5:
		*DO_select = INT_CLKSEL_DO5; return 1;
	}
}

static inline int getBitNum(int mask)
{
	if (mask == 0){
		return -1;
	}else{
		int dN = 0;

		while(((1<<dN) & mask) == 0){
			++dN;
		}
		return dN;
	}		
}

static int mbox_setInternalClock( void )
{
	int DO_mask = BP_GET_A2(*mbox(BP_MB_COMMAND));
	int master = DO_mask != 0;
	int DO = getBitNum(DO_mask);

	acq200_setInternalClock(*mbox( BP_MB_A3 ), master, DO);
	return SUCCESS;
}

static int mbox_setExternalClock( void )
{
	u32 command = *mbox( BP_MB_COMMAND );
	int DI_mask = BP_GET_A1( command );
	int DO_mask = BP_GET_A2( command );
	u32 div = *mbox( BP_MB_A3 );
	int DI_select, DO_select;
    
	if ( convert_ACQ32_DIOSELECT_to_INT_CLKSRC( DI_mask, &DI_select ) ){
		if ( convert_ACQ32_DIOSELECT_to_CLKSEL( DO_mask, &DO_select ) ){
			acq200_setExternalClock( DI_select, div, TRUE, DO_select ); 
		}else{
			acq200_setExternalClock( DI_select, div, FALSE, 0 );
		}
		acq32_setDIOinputs( DI_mask );
		return SUCCESS;
	}else{
		return !SUCCESS;
	}
}

static int mbox_getExternalClock( void )
{
	// WORKTODO
	return SUCCESS;
}


static int mbox_adjustTrigger( void )
{
	return SUCCESS;
}
static int mbox_getATrigger( void )
{
	printf( "mbox_getATrigger STUB\n" );
	return !SUCCESS;
}


extern void pgm_speed_debug( int );

static int mbox_setDebug( void )
{
	P_debug = BP_GET_A1(*mbox(BP_MB_COMMAND));
    
	PRINTF( "mbox_set_debug %d\n", P_debug );
    
	// non zero - dorp the tty, zero bring the tty back in
	pgm_speed_debug( P_debug );
	return SUCCESS;
}


static int mbox_setUser( void )
{
	unsigned flags = *mbox( BP_MB_A3 );
#define LED3_MASK							\
	(BP_FC_USER_LED3_ON|BP_FC_USER_LED3_OFF|BP_FC_USER_LED3_FLIP)
	unsigned led3 = flags&LED3_MASK;
#define LED4_MASK							\
	(BP_FC_USER_LED4_ON|BP_FC_USER_LED4_OFF|BP_FC_USER_LED4_FLIP)      
	unsigned led4 = flags&LED4_MASK;
  
	switch( led3&LED3_MASK ) {
	case BP_FC_USER_LED3_ON:
		acq200_setUserLed(USER_LED3, USER_LED_ON);
		break;
	case BP_FC_USER_LED3_OFF:
		acq200_setUserLed(USER_LED3, USER_LED_OFF);
		break;
	case BP_FC_USER_LED3_FLIP:
		acq200_setUserLed(USER_LED3, USER_LED_FLASH);
		break;
	default:
		;
	}  
	switch( led4&LED4_MASK ) {
	case BP_FC_USER_LED4_ON:
		acq200_setUserLed(USER_LED4, USER_LED_ON); 
		break;
	case BP_FC_USER_LED4_OFF:
		acq200_setUserLed(USER_LED4, USER_LED_OFF);
		break;
	case BP_FC_USER_LED4_FLIP:
		acq200_setUserLed(USER_LED4, USER_LED_FLASH);
	default:    
		;
	}
	return SUCCESS;
}

static int mbox_getUser( void )
{
	return SUCCESS;    // WORKTODO!!
}




static int mbox_cpci_getRouteStatus( void )
{
	*mbox(BP_MB_COMMAND) |= BP_SET_A1(acq32cpci_getCurrentFuncState());
	return SUCCESS;
}

static int processShortCommand(unsigned command)
/** handle commands acceptable during capture */
{
	switch( BP_GET_FUNCODE( command ) ){
	case BP_FC_SET_INTERRUPTS:
		acq32_setCommandInterruptAck(
			BP_GET_A1(*mbox(BP_MB_COMMAND)));
		return SUCCESS;
	case BP_FC_SC_SET_DIO:
		acq200_setDio(*mbox(BP_MB_A3), *mbox(BP_MB_A4));
		return SUCCESS; 
	case BP_FC_SET_ABORT:
		acq200_setAbort();
		mbox_abort_received = TRUE;
		return SUCCESS;
	case BP_FC_STREAM:
		return mboxSetStreaming();
	case BP_FC_SET_DEBUG:
		mbox_setDebug();
		return SUCCESS;
	default:
		return 0;
	}
}
static int processCommand( unsigned command )
// returns true on success
{
	switch( BP_GET_FUNCODE( command ) ){
	case BP_FC_SET_MODE:
		return mbox_setMode();
	case BP_FC_SET_MODE_GC:
		return mbox_setModeGatedContinuous();
	case BP_FC_SET_MODE_TC:
		return mbox_setModeTriggeredContinuous();
	case BP_FC_SET_MODE_LLC:
		return mbox_setModeLowLatencyControl();
	case BP_FC_SET_ARM:
		return mbox_setArm();
	case BP_FC_SET_ABORT:
		return mbox_setAbort();
	case BP_FC_SET_MASK:
		return mbox_setMask();
	case BP_FC_SET_INTERRUPTS:
		acq32_setCommandInterruptAck( BP_GET_A1(*mbox(BP_MB_COMMAND)));
		return 1;
	case BP_FC_SET_INTCLOCK:
		return mbox_setInternalClock();
	case BP_FC_SET_EXTCLOCK:
		return mbox_setExternalClock();
	case BP_FC_SET_TAGGING:
#ifdef PGMCOMOUT
		acq32_setTagging( BP_GET_A1(*mbox(BP_MB_COMMAND)) );
#endif
		return 1;
	case BP_FC_SET_HOST_DMABUF:
		return mboxSetHostDmabuf();
	case BP_FC_STREAM:  
		return mboxSetStreaming();
	case BP_FC_SET_SPIN:
		return mboxSpin();
	case BP_FC_BIG_DUMP:
		return mboxDoBigDump();
	case BP_FC_SET_DATA_THRESHOLD:
		return mboxDoSetDataThreshold();
	case BP_FC_CPCI_SET_ROUTE:
		return mbox_cpci_setRoute();
	case BP_FC_USER:
		return mbox_setUser();
	case BP_FC_SET_DEBUG:
		return mbox_setDebug();
	case BP_FC_ADJUST_TRIGGER:
		return mbox_adjustTrigger();
	default:
		PRINTF( "mbox-slave: ERROR command not accepted %x %c\n", 
			BP_GET_FUNCODE( command ), BP_GET_FUNCODE( command ) );
		return 0;     
	}
}

static int processShortQuery( unsigned command )
{
	switch( BP_GET_FUNCODE( command ) ){
	case BP_FC_GET_NUMSAMPLES:
		return mbox_getNumSamples();
	case BP_FC_GET_MASK:
		return mbox_getMask();
	case BP_FC_SC_GET_DIO:
		return mbox_getDio();
	case BP_FC_GET_INTCLOCK:
		return mbox_getInternalClock();
	case BP_FC_GET_EXTCLOCK:
		return mbox_getExternalClock();
	case BP_FC_CPCI_GETROUTE_STATUS:
		return mbox_cpci_getRouteStatus();
	case BP_FC_USER:
		return mbox_getUser();
	case BPFC_GET_NCHANNELS:
		return mbox_getNChannels();
	case BPFC_GET_RANGE:
		return mbox_getRange();
	case BP_FC_GET_ATRIGGER:
		return mbox_getATrigger(); 
	default:
		return 0;
	}
}

static int processLongQuery( unsigned command ) 
{
	switch( BP_GET_FUNCODE( command ) ){
	case BP_FC_GET_FWREV:
		return mbox_getFwrev();
	case BP_FC_GET_CONFIG:
		return mbox_getConfig();
	case BP_FC_GET_CALINFO:
		return mbox_getCalInfo();
	case BP_FC_GET_CAPTURESTATS:    
		return mbox_getCaptureStats();
	default:
		return 0;
	}
}
#define BP_NEWCOMMAND   (BP_CI_QUERY|BP_CI_COMMAND)


static void processMboxCommand(int long_ok)
{
	unsigned command = *mbox( BP_MB_COMMAND );

	if ( (command&BP_CI_ACK) == 0 && (command&BP_NEWCOMMAND) != 0 ){

		int is_command = (command&BP_CI_COMMAND) != 0;

		dbg(2, "got command %08x A3=%08x A4=%08x\n", 
			    command, *mbox( BP_MB_A3 ), *mbox( BP_MB_A4 ));

		*mbox( BP_MB_COMMAND ) = command &= ~BP_NEWCOMMAND;

		if ( is_command ){
			if (processShortCommand(command) || 
			    (long_ok && processCommand(command))){
				ack();
			}else{
				nack(command);
			}
		}else{
			if ( processShortQuery( command ) || 
			     (long_ok && processLongQuery(command))){
				ack();
			}else{
				err("unknown Query 0x%08x\n", command);
				nack(command);
			}
		}
	}
}



static void processTest( Message* message ) 
{
	static int itest;
    
	if ( message == (Message*)0 ){
		itest = 0;
	}else{
		if ( itest < 3 || itest%100 == 0 ){
#ifdef DANGEROUSE
			PRINTF( "T:%5d I:%5d %s\n", 
				trim( message->payload.cdata ) );
#endif
		}
	}
        
	itest += 1;
}

extern void acq200_processHostbound(Message *message);
extern void acq200_processAcqbound(Message *message);


// option to debug incoming i2o
extern int G_dump_i2o;
extern void i2o_dump( void* pmessage );
extern int d_bug_line( char* the_line );

void mboxProcessI2OIncoming( int init )
{
	static unsigned short expected_id;
	static enum MBOXI2OIN state;
    
	switch( init ){
	case MBOXI2OIN_INIT:
		expected_id = 1;
		state = MBOXI2OIN_INIT;
		return;
	case MBOXI2OIN_ENABLE:
		state = MBOXI2OIN_ENABLE;
		return;
	default:
		if ( state != MBOXI2OIN_ENABLE ){
			return;
		}
	}
                     
	{
		Message* message = i2o_getInboundMessage();
        
        
		if ( message != NO_MESSAGE ) {
        
			if ( G_dump_i2o ) i2o_dump( message );
                
			if ( GET_IDN(message->header.id) != expected_id ) {
				info("ERROR: expected id %d got %d\n", 
				     expected_id, GET_IDN(message->header.id));
				// WORKTODO ... report back to host
				expected_id = GET_IDN(message->header.id);
			}
                
			dbg(2, "message: %p id:%04x len:%d type:0x%08x "
			       "MT_ID %x MT_PRAM:%d\n",
				message, 
				message->header.id, 
				message->header.length, 
				message->header.type,
				MT_ID(message->header.type),
				MT_PRAM(message->header.type)
			);
                
			if (MT_ID(message->header.type) == MT_COMMAND_PACKET){
				switch( MT_PRAM(message->header.type) ){
				case MTC_DEBUG_TEXT:
					d_bug_line( message->payload.cdata );
					break;
				case MTC_TEST:
					processTest( message );
					break;
				case  MTC_HOST_REQUEST_DATA_HOSTBOUND:
					acq200_processHostbound(message);
					break;
				case MTC_HOST_REQUEST_DATA_ACQBOUND:
					acq200_processAcqbound(message);
					break;
				default:
					err("WORKTODO - SUBTYPE %d "
						"not implemented\n",
					      MT_PRAM(message->header.type));
				}
			}else{
				err("bad type 0x%x\n", message->header.type );
			}
                
			i2o_returnInboundMessage( message );
			expected_id = (expected_id+1)&0x7f;
		}   
	}
}


int G_mbto = 20000;

extern int i2o_getFd(void);
extern int i2o_getMbox(void);



void mboxSlaveOnce(int block)
{
	static int first_time = 1;
	static fd_set watchset;
	static int maxfd;
	int fd_mbox = i2o_getMbox();
	int fd_i2o = i2o_getFd();
	
	if (first_time){
		FD_ZERO(&watchset);
		FD_SET(fd_i2o, &watchset);
		maxfd = fd_i2o+1;
		first_time = 0;
	}

	FD_SET(fd_mbox, &watchset);

	if (fd_mbox+1 > maxfd){
		maxfd = fd_mbox + 1;
	}
	if (block){
		int rc;
		// select on mu, ipq 
		fd_set inset = watchset;
	        struct timeval timeout = {
			.tv_sec = 0, 
			.tv_usec = G_mbto
		};

		rc = select(maxfd, &inset, 0, 0, &timeout);
		if ( rc < 0 ){
			if (errno == EINTR){
				dbg(1, "select was interrupted");
			}else{		
				err("select return error %d", errno);
				_exit(-errno);
			}		
		}else if (rc > 0){
			dbg(2, "select scores go turbo ..");
			if (FD_ISSET(fd_i2o, &inset)){
				mboxProcessI2OIncoming(MBOXI2OIN_POLL);
			}
		}
	}

       	switch(mbox_getState()){
	case ST_ARM:
	case ST_RUN:
		processMboxCommand(0);
		break;
	default:
		processMboxCommand(1);
	}
        mboxProcessI2OIncoming( MBOXI2OIN_POLL );

	mbox_reportMsecs();                
	hasStopped();
	acq200_service();
}

int mbox_abort()
// if an Abort is present, acks it and returns TRUE
{
	unsigned command = *mbox( BP_MB_COMMAND );

	if ((command&BP_CI_ACK) == 0 && (command&BP_NEWCOMMAND) != 0){

		int is_command = (command&BP_CI_COMMAND) != 0;
		int should_ack = TRUE;

		dbg(2, "got command 0x%08x\n", command );

		*mbox( BP_MB_COMMAND ) = command &= ~BP_NEWCOMMAND;

		if (is_command){
			should_ack = processShortCommand(command);
		}else{
			should_ack = processShortQuery(command);
			if ( !should_ack ){
				err( "unknown Query 0x%08x\n", command);
			}
		}

		if (should_ack){
			ack();
		}else{
			nack( command );
		}
	}    

	return mbox_abort_received;
}
