/*****************************************************************************
 *
 * File: acq200_hal.c
 *
 * $RCSfile: acq200_hal.c,v $
 * 
 * Copyright (C) 2003 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: acq200_hal.c,v 1.1.2.70 2011/11/11 22:01:11 pgm Exp $
 * $Log: acq200_hal.c,v $
 * Revision 1.1.2.70  2011/11/11 22:01:11  pgm
 * raise prio DELTAPRI - this is good, else DMC0 starves
 *
 * Revision 1.1.2.69  2011/11/11 19:13:15  pgm
 * B2229 remvoe bogus sleep from acq200_setArm and runs FAST
 *
 * Revision 1.1.2.68  2011/11/11 15:25:29  pgm
 * attempt to speed ST_CAPDONE with raised priority for MGR process
 *
 * Revision 1.1.2.67  2010/02/14 13:46:06  pgm
 * all ACQ132_DEFAULT_SHIFT
 *
 * Revision 1.1.2.66  2010/02/14 13:35:38  pgm
 * set acq132 decimate 1 on acqcmd setExternalClock NOICS
 *
 * Revision 1.1.2.65  2010/02/14 13:34:40  pgm
 * set acq132 decimate 1 on acqcmd setExternalClock NOICS
 *
 * Revision 1.1.2.64  2010/02/12 14:57:46  pgm
 * B2224 fixes bug in clksel. ACQ132 selectable clocks
 *
 * Revision 1.1.2.63  2010/02/12 13:55:04  pgm
 * new clksel for ACQ132
 *
 * Revision 1.1.2.62  2009/09/07 22:06:30  pgm
 * solves the ocs527 external clock conundrum
 *
 * Revision 1.1.2.61  2009/09/06 19:01:15  pgm
 * acq200_isAcq164, fix channelMask
 *
 * Revision 1.1.2.60  2009/06/10 20:10:19  pgm
 * reduces setArm/setAbort race, still needs set.Arm to be perfect
 *
 * Revision 1.1.2.59  2009/06/05 20:07:09  pgm
 * removed filter_state - works faster.
 * No longer needed now device driver is cleaned up
 *
 * Revision 1.1.2.58  2009/05/31 07:52:21  pgm
 * remote state filtering, speed up
 *
 * Revision 1.1.2.57  2008/05/09 09:45:12  pgm
 * cap_status monitor
 *
 * Revision 1.1.2.56  2008/05/07 09:56:51  pgm
 * B2208 fixes mas_clk output
 *
 * Revision 1.1.2.55  2008/03/25 15:52:00  pgm
 * acq132
 *
 * Revision 1.1.2.54  2007/12/08 18:14:35  pgm
 * tighten up error exit codes
 *
 * Revision 1.1.2.53  2007/11/14 12:26:49  pgm
 * remove setLength - setMode does it now.
 *
 * Revision 1.1.2.52  2007/11/14 12:24:25  pgm
 * removed mu_throttle
 *
 * Revision 1.1.2.51  2007/04/13 17:13:20  pgm
 * B2184 hopefully fix hostwin init - both for usual [init] case and for reruns
 *
 * Revision 1.1.2.50  2007/04/02 18:00:41  pgm
 * B2174 window leng
 *
 * Revision 1.1.2.49  2007/01/05 12:56:48  pgm
 * restore setDIO to old mbox, also abort
 *
 * Revision 1.1.2.48  2007/01/04 09:04:49  pgm
 * minimise printf output
 *
 * Revision 1.1.2.47  2007/01/03 23:16:16  pgm
 * double fork() is good
 *
 * Revision 1.1.2.46  2007/01/03 12:37:42  pgm
 * acq200_muThrottle suppress ints during capture/postprocess to eliminate priority inversion
 *
 * Revision 1.1.2.45  2007/01/01 18:15:23  pgm
 * ST_STOP filter works
 *
 * Revision 1.1.2.44  2007/01/01 14:39:34  pgm
 * mbox control works
 *
 * Revision 1.1.2.43  2006/09/13 15:23:57  pgm
 * *** empty log message ***
 *
 * Revision 1.1.2.42  2006/06/25 11:58:40  pgm
 * works with new style dynamic hostbufs
 *
 * Revision 1.1.2.41  2005/10/20 20:49:50  pgm
 * avoid ST_STOP race
 *
 * Revision 1.1.2.40  2005/09/17 18:31:06  pgm
 * computes elapsed samples correctly
 *
 * Revision 1.1.2.39  2005/05/16 08:49:58  pgm
 * *** empty log message ***
 *
 * Revision 1.1.2.38  2005/03/05 16:11:11  pgm
 * new get.vin
 *
 * Revision 1.1.2.37  2005/02/06 17:45:42  pgm
 * *** empty log message ***
 *
 * Revision 1.1.2.36  2005/02/06 16:14:48  pgm
 * *** empty log message ***
 *
 * Revision 1.1.2.35  2004/12/16 17:06:09  pgm
 * fix zero prelen kludges
 *
 * Revision 1.1.2.34  2004/11/14 09:46:38  pgm
 * clear ext clk before update int_clk
 *
 * Revision 1.1.2.33  2004/11/01 12:22:27  pgm
 * 2100
 *
 * Revision 1.1.2.32  2004/10/23 14:17:45  pgm
 * rsh single path style
 *
 * Revision 1.1.2.31  2004/10/21 09:29:16  pgm
 * various fixes to sync post process
 *
 * Revision 1.1.2.30  2004/09/23 21:07:29  pgm
 * B2081
 *
 * Revision 1.1.2.29  2004/09/04 22:48:40  pgm
 * B2073 - event0
 *
 * Revision 1.1.2.28  2004/08/29 11:48:18  pgm
 * improve number enabled channels handling
 * also ext clk
 *
 * Revision 1.1.2.27  2004/08/14 14:04:16  pgm
 * really get the forking subprocess right
 *
 * Revision 1.1.2.26  2004/08/14 11:46:54  pgm
 * refactor fork on Arm to cover WAV232
 *
 * Revision 1.1.2.25  2004/06/05 21:41:15  pgm
 * works with pre=0, trigger=event frig
 *
 * Revision 1.1.2.24  2004/05/25 14:06:11  pgm
 * plumb TriggeredContinuous
 *
 * Revision 1.1.2.23  2004/05/18 20:12:07  pgm
 * B2060 setChannelMask ok ACQ196
 *
 * Revision 1.1.2.22  2004/05/16 07:16:48  pgm
 * DioString ops
 *
 * Revision 1.1.2.21  2004/04/25 12:57:46  pgm
 * concurrent queries work
 *
 * Revision 1.1.2.20  2004/04/25 11:40:52  pgm
 * concurrency 1 with logging
 *
 * Revision 1.1.2.19  2004/04/17 20:33:50  pgm
 * refine channel handling
 *
 * Revision 1.1.2.18  2004/04/13 09:35:46  pgm
 * no works with acq196
 *
 * Revision 1.1.2.17  2004/03/26 11:11:48  pgm
 * acq196 mu
 *
 * Revision 1.1.2.16  2004/03/23 22:45:45  pgm
 * *** empty log message ***
 *
 * Revision 1.1.2.15  2004/02/19 22:26:23  pgm
 * setMode getMode debugged
 *
 * Revision 1.1.2.14  2004/02/15 21:39:20  pgm
 * local slave command handling
 *
 * Revision 1.1.2.13  2004/02/14 18:04:11  pgm
 * wav232_setArm
 *
 * Revision 1.1.2.12  2004/02/06 20:42:17  pgm
 * unwind abortive c++ expt - compiles but no libs
 *
 * Revision 1.1.2.11  2003/12/12 14:19:15  pgm
 * *** empty log message ***
 *
 * Revision 1.1.2.10  2003/11/13 14:24:46  pgm
 * fixes getInternalClock
 *
 * Revision 1.1.2.9  2003/11/08 14:48:16  pgm
 * channel mask, pre post sorted
 *
 * Revision 1.1.2.8  2003/11/07 09:06:10  pgm
 * pre A4, post A3
 *
 * Revision 1.1.2.7  2003/11/05 22:29:57  pgm
 * hooks GATED_TRANSIENT, dio
 *
 * Revision 1.1.2.6  2003/10/30 14:12:36  pgm
 * B2017
 *
 * Revision 1.1.2.5  2003/10/27 20:38:36  pgm
 * reads status from driver
 *
 * Revision 1.1.2.4  2003/10/25 15:12:00  pgm
 * gutted
 *
 * Revision 1.1.2.3  2003/10/19 20:43:42  pgm
 * works with dt100rc
 *
 * Revision 1.1.2.2  2003/10/06 19:44:51  pgm
 * new style downstream window hook
 *
 * Revision 1.1.2.1  2003/09/04 21:08:42  pgm
 * getFwrev works (x156)
 *
 *
 *
\*****************************************************************************/


#include <stdio.h>
#include "local.h"
#include <stdlib.h>                   /* getenv() */

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>

#include <errno.h>
#include <unistd.h>
#include <stdarg.h>

#include "acq200.h"
#include "acq32busprot.h"
#include "dt100api.h"

#include "acq200_hal.h"


#include "mbox_slave.h"

#include <assert.h>

int vsscanf(const char *str, const char *format, va_list ap);

#define SYSDWN "/dev/bridge/downstream_window"

#define DRIVER_ROOT "/dev/dtacq_drv"
#define DEVICE_ROOT "/dev/dtacq"
#define CORE_ROOT   "/dev/acq200.core"
#define MU_ROOT     "/dev/mu0"

#define ONESHOT "/dev/oneshot"
#define WAVEGEN "/dev/wavegen"

#define STATUS_ROOT "/proc/driver/acq200/status/"

#define STATUS "status"
#define NUMSAM "samples"

#define DELTAPRI 	4

static int acq200_fileExists(char *path)
{
	struct stat buf;

	if (stat(path, &buf) == 0){
		return 1;
	}else{
		return 0;
	}
}

static int acq200_rootExists(char * ROOT, char *fname)
{
	char fullname[80];

	sprintf(fullname, "%s/%s", ROOT, fname);	
	return acq200_fileExists(fullname);
}

int acq200_getRoot(
	char * ROOT, char *fname, int nval, char *fmt, ... )
{
	va_list ap;
	char buf[80];
	char fullname[80];
	FILE *fp;

	sprintf(fullname, "%s/%s", ROOT, fname);
	fp = fopen(fullname, "r");

	memset(buf, 0, sizeof(buf));

	dbg(4, "reading from %s", fullname);

	va_start(ap, fmt);
	if (!fp){
		char errs[128];
		sprintf(errs, "ERROR: failed to open %s\n", fullname);
		perror(errs);
		_exit(errno);
	}

	if (fgets(buf, 79, fp) == 0){
		char errs[128];
		sprintf(errs, "ERROR: EOF on %s\n", fname);
		perror(errs);
		_exit(errno);
	}
	if (vsscanf(buf, fmt, ap) != nval){
		char errs[128];
		sprintf(errs, "ERROR: vsscanf failed on %s\n", buf);
		perror(errs);
		_exit(errno);
	}		

	va_end(ap);
	fclose(fp);
	return 0;
}


static void acq200_setRoot(char* ROOT, char *fname, char* fmt, ...)
{
	va_list ap;
	char fullname[80];
	FILE *fp;

	va_start(ap, fmt);
	sprintf(fullname, "%s/%s", ROOT, fname);

	fp = fopen(fullname, "w");

	if (!fp){
		char errs[128];
		sprintf(errs, "ERROR: failed to open %s\n", fullname);
		perror(errs);
		_exit(errno);
	}

	if (acq200_debug>1){
		info( "writing to %s", fullname);
		vprintf( fmt, ap );
		printf( "\n" );
	}

	vfprintf(fp, fmt, ap);
	fclose(fp);
	va_end(ap);
}




static void __acq200_init_downstream_window( 
	const char* sysfile, u32 base, u32 length )
{
	FILE *fp = fopen(sysfile, "w+");

	if ( !fp ){
		perror(sysfile);
		_exit(errno);
	}

	fprintf(fp, "0x%08x 0x%08x\n", base, length);
	fclose(fp);
}


static void __acqX00_init_downstream_window(u32 base, u32 length)
{
	if (acq200_fileExists(MU_ROOT)){
		acq200_setRoot(MU_ROOT, "downstream_window", "0x%x 0x%x", 
			       base, length);
	}else{
		info("STUB: 0x%08x 0x%08x\n", base, length);
	}
}

void acq200_init_downstream_window( u32 base, u32 length )
{
	if (acq200_fileExists(SYSDWN)){
		__acq200_init_downstream_window( SYSDWN, base, length );
	}
	__acqX00_init_downstream_window(base, length);
}

static int S_length = -1;
static int S_prelen = -1;
static int S_postlen = -1;

int numberOfEnabledChannels()
{
	unsigned mask = acq200_getChannelMask();
	int ibit;
	int nchan = 0;

	for (ibit = 31; ibit >= 0; --ibit){
		if (mask &(1<<ibit)){
			++nchan;
		}
	}
	if (acq200_isAcq196()){
		nchan *= 32;       /* channel mask is block of 32 ch */
	}
	return nchan;
}



static void acq200_setInternalClockMaster(int master, int DOx)
{
	char cmd[80];
	int rc;

	if (master){
		sprintf(cmd, "set.mas_clk DO%d", DOx);
		rc = system(cmd);
		dbg(1, "%s returned %d", cmd, rc);
		/* now clock local from _the_same_ clock as remote */
		sprintf(cmd, "set.ext_clk DI%d falling", DOx);
		rc = system(cmd);
		dbg(1, "%s returned %d", cmd, rc);
	}else{
		sprintf(cmd, "set.mas_clk none");
		rc = system(cmd);
		dbg(1, "%s returned %d", cmd, rc);
	}
}
void acq200_setInternalClock(int hz, int master, int DOx)
{
	acq200_setRoot(DRIVER_ROOT, "int_clk", "%d", hz);
	acq200_setInternalClockMaster(master, DOx);
}

unsigned acq200_getInternalClock(void)
{
	unsigned intclk = 0;

	acq200_getRoot(DRIVER_ROOT, "int_clk", 1, "%u",  &intclk);
	return intclk;
}

void acq2xx_setArm(void)
/* generic acquisition board arm */
{
	char *fname = ONESHOT;
	FILE *fp;
	unsigned mybuf[32];

	fp = fopen( fname, "r" );
	
	S_length = -1;

	if (!fp){
		char errs[128];
		sprintf(errs, "ERROR: failed to open %s\n", fname);
		perror(errs);
		_exit(errno);
	}

	fread(mybuf, 1, 32, fp);
	fclose(fp);
}


void acq200_setOneshot(int oneshot)
{
	acq200_setRoot(DEVICE_ROOT, "oneshot", "%d", oneshot);
}

void wav232_setArm(void)
/* wavegen specific setArm */
{
	char *fname = WAVEGEN;
	FILE *fp;
	unsigned gash = 0xdeadbeef;

	fp = fopen( fname, "w" );
	
	S_length = -1;

	if (!fp){
		char errs[128];
		sprintf(errs, "ERROR: failed to open %s\n", fname);
		perror(errs);
		_exit(errno);
	}

	fwrite(&gash, 1, sizeof(gash), fp);
	fclose(fp);
}


static void (*_acq200_setArm)(void)  = acq2xx_setArm;


extern void clear_signals(void);

void acq200_setArm(void (*cb)(unsigned status))
{
	pid_t arm_man_pid;

	if ((arm_man_pid = fork()) == 0){   /* split from main */
		int priority = getpriority(PRIO_PROCESS, getpid());

		pid_t pid;
		clear_signals();		/* we want to wait() */

		/* ident long ST_CAPDONE time .. try to limit */
		setpriority(PRIO_PROCESS, getpid(), priority-DELTAPRI);

		if ((pid = fork()) != 0){    /* split again, manager process */

			int status;

			dbg(1, "|%d| wait %d", getpid(), pid);
			wait(&status);
			dbg(1, "|%d| status %d", getpid(), status);
			cb((unsigned)status);
			exit(__LINE__);
		}else{
			_acq200_setArm();
			exit(__LINE__);
		}
	}
	if (arm_man_pid < 0){
		err("arm_man_pid %d", arm_man_pid);
	}
}


static unsigned _getEnvNum(const char *key)
{
	char *value = getenv(key);

	if (value != 0 && atoi(value) > 0){
		return atoi(value);
	}else{
		return 0;
	}
}
unsigned acq200_getNumChannels(void)
{
	unsigned nchan = _getEnvNum("AICHAN");

	if (nchan){
		return nchan;
	}else{
		return acq200_isAcq216()? 16:
			acq200_isAcq232()? 32:
			acq200_isAcq196()? 96: 
			acq200_isAcq164()? 64: 32;
	}
}



unsigned acq200_getAOcount(void)
{
	unsigned nchan = _getEnvNum("AOCHAN");

	if (nchan){
		return nchan;
	}else if (acq200_isWav232()){
		return 32;
	}else{
		struct stat buf;
		if (stat("/dev/acq196/AO/16", &buf) == 0){
			return 16;
		}else if (stat("/dev/acq196/AO/08", &buf) == 0){
			return 8;
		}else if (stat("/dev/acq196/AO/04", &buf) == 0){
			return 4;
		}else{
			return 0;
		}
	}
}

static unsigned long long S_elapsed;

static void _updateNumSamples(void)
{
	acq200_getRoot(STATUS_ROOT, NUMSAM,
		       4, "%d %d %d %Lu",
		       &S_length, &S_prelen, &S_postlen, &S_elapsed);
}
int acq200_getNumSamples(void)
{
	_updateNumSamples();
	return S_prelen + S_postlen;
}
int acq200_getPreSamples(void)
{
	_updateNumSamples();
	return S_prelen;
}
int acq200_getPostSamples(void)
{
	_updateNumSamples();
	return S_postlen;
}

unsigned long long acq200_getElapsedSamples(void)
{
	return S_elapsed;
}

unsigned acq200_getChannelMask(void)
{
	unsigned mask = 0xffffffffUL;

	if (acq200_rootExists(DEVICE_ROOT, "channel_mask")){
		acq200_getRoot(DEVICE_ROOT, "channel_mask", 1, "%x", &mask);
	}else{
		info("FPGA not loaded, use default");
	}
	return mask;
}
unsigned acq200_setChannelMask(unsigned mask)
/* this needs to be set on the FPGA */
{
	dbg(1,"setting channel_mask %x", mask);

	if (acq200_rootExists(DEVICE_ROOT, "channel_mask")){
		acq200_setRoot(DEVICE_ROOT, "channel_mask", "%x\n", mask);
		acq200_getRoot(
			DEVICE_ROOT, "channel_mask", 1, "%x", &mask);
		dbg(1,"return   channel_mask %08x", mask);
	}else{
		info("FPGA not loaded, use default");
	}
	return mask;
}

void acq200_setAbort(void)
{
	if(acq200_rootExists(DEVICE_ROOT, "user_abort")){
		acq200_setRoot(DEVICE_ROOT, "user_abort", "abort\n");
	}else{
		err("no root for user_abort");
	}
}

unsigned acq200_getBigBufLen(void)
{
	static unsigned bb_len;

	if (bb_len == 0){
#define BBLEN "bb_len"
		bb_len = 0x08000000;

		if (acq200_rootExists(DRIVER_ROOT, BBLEN)){
			acq200_getRoot(
				DRIVER_ROOT, BBLEN, 1, "%u", &bb_len);
		}else if (acq200_rootExists(CORE_ROOT, BBLEN)){
			acq200_getRoot(
				CORE_ROOT, BBLEN, 1, "%u", &bb_len);
		}

		info("BBLEN %x\n", bb_len);
	}

	return bb_len;
}


void acq200_setMode(int mode, int prelen, int postlen)
{
	switch(mode){
	case M_SOFT_CONTINUOUS:
		if (postlen && !prelen){
			prelen = postlen;
		}
		break;
	case M_TRIGGERED_CONTINUOUS:
		break;
	case M_SOFT_TRANSIENT:
		break;
	default:
		mode = M_GATED_TRANSIENT;  /* WORKTODO - hack for now */
	}

	dbg(1, "setMode %d %d %d\n", mode, prelen, postlen);

	acq200_setRoot(DEVICE_ROOT, "mode", "%d %d %d\n", 
		       mode, prelen, postlen);
}


void configure_abort_code(void)
/* tell the driver how to detect an abort */
{
#ifdef PGMCOMOUT   /* prefer USER_ABORT */
	unsigned mask = BP_CI_COMMAND|BP_SET_FUNCODE(0xff);
	unsigned cmd = BP_CI_COMMAND|BP_SET_FUNCODE(BP_FC_SET_ABORT);

	acq200_setRoot(DRIVER_ROOT, "pci_abort", "%x %x\n", mask, cmd);
#endif
}


#define MAXBITS 8

#define C_INPUT   '-'
#define C_OUTPUT1 '1'
#define C_OUTPUT0 '0'

#define C_INPUT_H 'H'
#define C_INPUT_L 'L'

void acq200_setDioString(char *command)
{
	acq200_setRoot(DEVICE_ROOT, "dio", command);
}
void acq200_getDioString(char *buf, int maxbuf)
{
	acq200_getRoot(DEVICE_ROOT, "dio", 1, "%s", buf);
}

void acq200_setDio(unsigned outputs, unsigned ones)
{
	int ibit;
	char command[MAXBITS+2];
	unsigned mask;

	for (ibit = 0; ibit != MAXBITS; ++ibit){
		mask = 1 << ibit;

		command[ibit] = 
			(outputs&mask) == 0? C_INPUT:
			(ones&mask)    != 0? C_OUTPUT1: C_OUTPUT0;
	}
	command[ibit++] = '\n';
	command[ibit] = '\0';
	acq200_setDioString(command);
}
unsigned acq200_getDio(void)
{
	char buf[20];
	int ibit;
	unsigned mask;
	unsigned value = 0;

	acq200_getDioString(buf, 20);
	
	for (ibit = 0; ibit != MAXBITS; ++ibit){
		mask = 1 << ibit;
		switch( buf[ibit] ){
		case C_INPUT_H:
		case C_OUTPUT1:
			value |= mask;
			break;
		default:
			;
		}
	}
	return value;
}


extern void acq200_setExternalClock( 
	int DIx, unsigned divisor, int is_master, int DOx)
{
	char cmd[80];
	int rc;

	if (divisor == 1){
		sprintf(cmd, "set.ext_clk DI%d", DIx);
		rc = system(cmd);
		dbg(1, "%s returned %d", cmd, rc);
		acq200_setInternalClockMaster(0, DOx);
	}else{
		sprintf(cmd, "set.ext_clk none;set.int_clk_src DI%d", DIx);
		rc = system(cmd);
		dbg(1, "%s returned %d", cmd, rc);

		acq200_setInternalClockMaster(is_master, DOx);
		acq200_setRoot(DRIVER_ROOT, "int_clk_div", "%d", divisor);
	}
}

void acq200_setUserLed(int led, int on_off_flash)
{
	char ledname[10];

	switch(led){
	case USER_LED3:
	case USER_LED4:
		sprintf(ledname, "led%d", led);
		break;
	default:
		return;
	}

	acq200_setRoot(CORE_ROOT, ledname, "%d",	on_off_flash);
}


static int boardType;
int acq200_isAcq(void)
{
	return (boardType&BT_ACQ) != 0;
}
int acq200_isAcq216(void)
{
	return (boardType&BT_ACQ216) != 0;
}
int acq200_isAcq232(void)
{
	return (boardType&BT_ACQ232) != 0;
}
int acq200_isAcq196(void)
{
	return (boardType&BT_ACQ196) != 0;
}
int acq200_isAcq132(void)
{
	return (boardType&BT_ACQ132) != 0;
}

int acq200_isAcq164(void)
{
	return (boardType&BT_ACQ164) != 0;
}

int acq200_isWav232(void)
{
	return (boardType&BT_WAV232) != 0;
}


int acq200_has_ics527(void)
{
	return acq200_isAcq132() || acq200_isAcq164();
}

void acq200_setBoardType(int bt)
{
	boardType = bt;
	if (acq200_isWav232()){
		_acq200_setArm = wav232_setArm;
	}
}


int acq132_setBestDecim(int khz, int *khz_clock)
{
	acq200_setRoot(DEVICE_ROOT, "best_decimation", "%d", khz);
	if (acq200_getRoot(
		   DEVICE_ROOT, "best_decimation", 1, "%d", khz_clock) == 0){
		return 0;
	}
	err("FAILED to set best_decimation");
	return -1;
}

enum STATE acq200_getState(void)
{
	enum STATE state = ST_UNKNOWN;

	if (acq200_getRoot(DEVICE_ROOT, "state", 1, "%d", &state) == 0){
		return state;
	}

	err("FAILED to get Driver state");
	return state;
}


void acq200_setState(enum STATE state)
{
	acq200_setRoot(	DEVICE_ROOT, "state", "%d", state);
}

int acq200_getVRange(VRANGE vrange)
{
	FILE* fp = popen("/usr/local/bin/get.vin largest-range", "r");
	char buf[80];

	if (!fp) return -1;

	if (fgets(buf, 80, fp)){
		switch(sscanf(buf, "%f,%f", 
			      &vrange[VRANGE_AI_MIN], 
			      &vrange[VRANGE_AI_MAX])){
		case 2:
			break;
		case 1:
			vrange[1] = vrange[0];  
			vrange[VRANGE_AI_MIN] = -vrange[VRANGE_AI_MIN];
			break;
		default:
			vrange[0] = vrange[1] = 10.0f;
		}
	}

	pclose(fp);

	return 0;
}

int acq200_getRange(char buf[], int maxbuf)
{
	VRANGE range;

	if (acq200_getVRange(range) == 0){
		return sprintf(buf, 
			       "%s AI=%.3fV,%.3fV AO=%.1fV,%.1fV", 
			       "getVoltsRange", 
			       range[VRANGE_AI_MIN], range[VRANGE_AI_MAX],
			       -10.0, 10.0);
	}else{
		return sprintf(buf, 
			       "%s AI=%.1fV,%.1fV AO=%.1fV,%.1fV", 
			       "getVoltsRange", 
			       -10.0, 10.0, -10.0, 10.0);
	}
}


int acq200_muThrottle(int throttle_enable)
/** enable==1 reduces MU response during critical activity 
 *  (disables int) 
 * NB inverts sense wrt to driver entry
 */
{
	acq200_setRoot(MU_ROOT, "mu_ipq_int_enable", "%d", throttle_enable==0);
	return 0;
}

static char* OIMR;

void init_OIMR(char *value)
/* value to write (repeatedly) to OIMR */
{
	OIMR = value;
}

void acq200_service(void)
/* service this value repeatedly */
{
	if (OIMR){
		acq200_setRoot(MU_ROOT, "OIMR", "%s", OIMR);
	}
}


int acq200_captureWasSuccess(void)
{
	char status[128];

	int rc = acq200_getRoot(DEVICE_ROOT, "cap_status", 1, "%s", status);

	if (rc != 0){
		return 0;
	}else{
		return strncmp(status, "OK", 2) == 0;
	}
}


int set_ics527(int fin_khz, int fout_khz, char *errb, int maxerrb)
{
	char command[128];
	char *cursor = errb;
	FILE *pp;

	sprintf(command, "%s --fin %d  %d",
		"/usr/local/bin/ob_calc_527", fin_khz, fout_khz);
	pp = popen(command, "r");
	if (pp == 0){
		sprintf(errb, "Failed to run %s", command);
		return -1;
	}
	while(fgets(cursor, maxerrb - (cursor - errb), pp)){
		cursor += strlen(cursor);
	}
	pclose(pp);
	return 0;
}

int set_acq132_clksel(const char* clksel)
{
	acq200_setRoot(DEVICE_ROOT, "clksel", "%s", clksel);
	return 0;
}

#define ACQ132_DECIMATE "/usr/local/bin/set.all.acq132.decimate"

void set_acq132_decimate(int decim, int shift)
{
	char cmd[80];
	sprintf(cmd, "%s %d %d", ACQ132_DECIMATE, decim, shift);
	system(cmd);
}
