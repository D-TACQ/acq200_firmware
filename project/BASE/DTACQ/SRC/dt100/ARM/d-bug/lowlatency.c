/*****************************************************************************
 *
 * File:
 *
 * $RCSfile: lowlatency.c,v $
 * 
 * Copyright (C) 2001 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: lowlatency.c,v 1.37.2.7 2008/07/02 11:31:40 pgm Exp $
 * $Log: lowlatency.c,v $
 * Revision 1.37.2.7  2008/07/02 11:31:40  pgm
 * drop a lot of acq32 crud
 *
 * Revision 1.37.2.6  2007/01/02 18:24:19  pgm
 * trim from 106K to 82K
 *
 * Revision 1.37.2.5  2004/07/25 15:40:26  pgm
 * ll hooks work
 *
 * Revision 1.37.2.4  2004/07/21 08:30:10  pgm
 * 2G Low Latency driver hooks
 *
 * Revision 1.37.2.3  2003/10/19 20:44:30  pgm
 * works with dt100rc
 *
 * Revision 1.37.2.2  2003/09/08 12:54:20  pgm
 * i2o file download works. ship it quick
 *
 * Revision 1.37.2.1  2003/08/29 17:41:02  pgm
 * init pci command handling
 *
 * Revision 1.37  2003/07/03 10:08:40  pgm
 * internal loopback becomes hook on DO3
 *
 * Revision 1.36  2002/03/21 11:54:51  pgm
 * untabify
 *
 * Revision 1.35  2002/03/13 21:43:09  pgm
 * correct formatting with emacs (Stroustrup)
 *
 * Revision 1.34  2002/02/28 18:02:04  pgm
 * ensure tinst is updated on request
 *
 * Revision 1.33  2002/02/20 21:41:42  pgm
 * fix overzealous ASSERT in storeman.c
 *
 * Revision 1.32  2002/02/19 19:32:10  pgm
 * clean up llcontrol and make it go forever
 *
 * Revision 1.31  2002/02/14 19:30:47  pgm
 * stubbing debugs
 *
 * Revision 1.30  2002/02/09 09:31:05  pgm
 * war on warnings
 *
 * Revision 1.29  2002/01/18 10:05:41  pgm
 * onwards and upwards
 *
 * Revision 1.28  2002/01/16 10:00:54  pgm
 * fixes decimation, instruments tlatch
 *
 * Revision 1.27  2002/01/13 22:21:58  pgm
 * acq32 fast threshtrig kludge coded
 *
 * Revision 1.26  2001/12/08 14:37:52  pgm
 * fix race to latch first count
 *
 * Revision 1.25  2001/11/19 14:28:58  pgm
 * Build 1420 - report tlatch to user before calc tproc
 *
 * Revision 1.24  2001/11/19 13:35:47  pgm
 * Build 1419, better regs use
 *
 * Revision 1.23  2001/11/19 12:46:15  pgm
 * built 1418 - LLC uses 16 bit counts
 *
 * Revision 1.22  2001/11/18 19:08:30  pgm
 * crect logice for MSB->0 rolloever detect
 *
 * Revision 1.20  2001/11/18 10:06:00  pgm
 * leaner llcontrol - has llcount probs
 *
 * Revision 1.19  2001/11/12 12:35:03  pgm
 * Build 1412, llcontrol tries for best latency
 *
 * Revision 1.18  2001/11/10 21:30:16  pgm
 * llcontrol works with first sample not 0
 *
 * Revision 1.17  2001/11/10 17:27:27  pgm
 * reworked llcontrol, still fails decimation
 *
 * Revision 1.16  2001/11/07 22:14:54  pgm
 * move div clock to DO3 for LLC
 *
 * Revision 1.15  2001/10/28 21:49:32  pgm
 * llcontrol 24 bit rolloever fixed
 *
 * Revision 1.14  2001/08/24 07:56:11  pgm
 * new protocol llc seems reliable
 *
 * Revision 1.13  2001/08/14 20:39:36  pgm
 * works, but can hang up too
 *
 * Revision 1.12  2001/08/13 18:09:13  pgm
 * ll best yet
 *
 * Revision 1.11  2001/08/13 16:43:23  pgm
 * ll counter extend works
 *
 * Revision 1.10  2001/08/13 10:47:41  pgm
 * LL timer update now works correctly, unprompted
 *
 * Revision 1.9  2001/08/10 20:47:58  pgm
 * fix lll counter overflow, add USER LEDS
 *
 * Revision 1.8  2001/06/27 22:51:44  pgm
 * nodos
 *
 * Revision 1.7  2001/05/25 12:04:47  pgm
 * command race eliminated -ll runs
 *
 * Revision 1.6  2001/05/23 20:41:42  pgm
 * enables at right time, ext clock uses correct route
 *
 * Revision 1.5  2001/05/21 20:07:53  pgm
 * llc - WIP
 *
 * Revision 1.4  2001/05/20 21:22:50  pgm
 * ll WIP - clocks, no samples
 *
 * Revision 1.3  2001/05/19 19:44:40  pgm
 * LL work in progress
 *
 * Revision 1.2  2001/05/19 12:59:56  pgm
 * low latency setup - done
 *
 * Revision 1.1  2001/05/19 10:30:04  pgm
 * low latency compiles
 *
 * Revision 1.2  2000/12/28 11:03:00  pgm
 * *** empty log message ***
 *
 * Revision 1.1  1999/10/22 16:26:49  pgm
 * first entry to cvs
 *
 *
\*****************************************************************************/

// turn this on for serious of time counts (now fixed)
//#define TLATCH_DEBUG    

#include <stdio.h>

#include "local.h"
#include "dt100api.h"

#include "acq32_drv.h"
#include "acq32_hml.h"
#include "memmap.h"
#include "dataHandling.h"


#include "dmac.h"
#include "diags.h"

#include "acq32busprot.h"

#include "hal.h"

#include <assert.h>


#define LLC_ROOT "/dev/llc"
#define LLC_MODE LLC_ROOT "/mode"
#define LLC_RUN  LLC_ROOT "/run"


FILE *acq200_fopen(const char* fname, const char* mode)
{
	FILE *fp = fopen(fname, mode);

	if (!fp){
		printf("ERROR:failed to open file \%s %s\n", fname, mode);
		_exit(-1);
	}
	return fp;
}

void acq32_setupLowLatencyControlSoftClock(
    int clkpos, int trpos, int intsoftclock
    )
{
	FILE *fp = acq200_fopen(LLC_MODE, "w");

	fprintf(fp, "SCM %d %d %d\n", clkpos, trpos, intsoftclock);
	fclose(fp);
}

void acq32_setupLowLatencyControlExtClock(
    int clkpos, int trpos, int intdivclk, unsigned divisor
    )
{
	FILE *fp = acq200_fopen(LLC_MODE, "w");

	fprintf(fp, "ECM %d %d %d %d\n", clkpos, trpos, intdivclk, divisor);
	fclose(fp);
}

void acq32_doLowLatencyControl( void )
{
	FILE *fp = acq200_fopen(LLC_RUN, "w");

	fprintf(fp, "1\n");
	fclose(fp);
}

void llSetRtClockTiming( int enable )
// use the RT clock for timing rather than the dived external clock 
{
	; /* WORKTODO */
}

