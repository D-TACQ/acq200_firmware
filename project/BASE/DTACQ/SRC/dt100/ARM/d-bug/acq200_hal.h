/*****************************************************************************
 *
 * File: acq200_hal.c
 *
 * $RCSfile: acq200_hal.h,v $
 * 
 * Copyright (C) 2003 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: acq200_hal.h,v 1.1.2.33 2010/02/14 13:46:06 pgm Exp $
 * $Log: acq200_hal.h,v $
 * Revision 1.1.2.33  2010/02/14 13:46:06  pgm
 * all ACQ132_DEFAULT_SHIFT
 *
 * Revision 1.1.2.32  2010/02/14 13:34:40  pgm
 * set acq132 decimate 1 on acqcmd setExternalClock NOICS
 *
 * Revision 1.1.2.31  2010/02/12 13:55:04  pgm
 * new clksel for ACQ132
 *
 * Revision 1.1.2.30  2009/09/08 05:57:28  pgm
 * B2222 provides ExternalClock defaults for ob_clock cards
 *
 * Revision 1.1.2.29  2009/09/07 22:06:30  pgm
 * solves the ocs527 external clock conundrum
 *
 * Revision 1.1.2.28  2009/09/06 19:01:15  pgm
 * acq200_isAcq164, fix channelMask
 *
 * Revision 1.1.2.27  2009/06/10 20:10:20  pgm
 * reduces setArm/setAbort race, still needs set.Arm to be perfect
 *
 * Revision 1.1.2.26  2008/05/09 09:45:12  pgm
 * cap_status monitor
 *
 * Revision 1.1.2.25  2007/04/13 17:13:20  pgm
 * B2184 hopefully fix hostwin init - both for usual [init] case and for reruns
 *
 * Revision 1.1.2.24  2007/01/05 12:56:48  pgm
 * restore setDIO to old mbox, also abort
 *
 * Revision 1.1.2.23  2007/01/03 23:16:16  pgm
 * double fork() is good
 *
 * Revision 1.1.2.22  2007/01/03 12:37:42  pgm
 * acq200_muThrottle suppress ints during capture/postprocess to eliminate priority inversion
 *
 * Revision 1.1.2.21  2007/01/02 18:18:10  pgm
 * *** empty log message ***
 *
 * Revision 1.1.2.20  2007/01/01 14:39:34  pgm
 * mbox control works
 *
 * Revision 1.1.2.19  2005/10/20 20:49:50  pgm
 * avoid ST_STOP race
 *
 * Revision 1.1.2.18  2005/09/17 18:31:06  pgm
 * computes elapsed samples correctly
 *
 * Revision 1.1.2.17  2004/11/01 12:22:27  pgm
 * 2100
 *
 * Revision 1.1.2.16  2004/10/23 14:17:45  pgm
 * rsh single path style
 *
 * Revision 1.1.2.15  2004/09/23 21:07:29  pgm
 * B2081
 *
 * Revision 1.1.2.14  2004/08/29 11:48:18  pgm
 * improve number enabled channels handling
 * also ext clk
 *
 * Revision 1.1.2.13  2004/06/05 21:41:15  pgm
 * works with pre=0, trigger=event frig
 *
 * Revision 1.1.2.12  2004/05/16 07:17:32  pgm
 * *** empty log message ***
 *
 * Revision 1.1.2.11  2004/04/17 20:33:50  pgm
 * refine channel handling
 *
 * Revision 1.1.2.10  2004/03/26 11:11:48  pgm
 * acq196 mu
 *
 * Revision 1.1.2.9  2004/02/15 21:39:20  pgm
 * local slave command handling
 *
 * Revision 1.1.2.8  2004/02/06 20:42:17  pgm
 * unwind abortive c++ expt - compiles but no libs
 *
 * Revision 1.1.2.7  2003/12/12 14:19:15  pgm
 * *** empty log message ***
 *
 * Revision 1.1.2.6  2003/11/13 14:24:46  pgm
 * fixes getInternalClock
 *
 * Revision 1.1.2.5  2003/11/05 22:29:57  pgm
 * hooks GATED_TRANSIENT, dio
 *
 * Revision 1.1.2.4  2003/10/30 14:12:36  pgm
 * B2017
 *
 * Revision 1.1.2.3  2003/10/25 15:12:00  pgm
 * gutted
 *
 * Revision 1.1.2.2  2003/10/19 20:43:53  pgm
 * works with dt100rc
 *
 * Revision 1.1.2.1  2003/09/04 21:08:42  pgm
 * getFwrev works (x156)
 *
 *
 *
\*****************************************************************************/

#ifndef _ACQ200_HAL_H_
#define _ACQ200_HAL_H_

#ifdef __cplusplus
extern "C" {
#endif
void acq200_init_downstream_window( u32 base, u32 length );

void acq200_setMode(int mode, int prelen, int postlen);
void acq200_setLength(int length);
void acq200_setInternalClock(int hz, int master, int DO);
unsigned acq200_getInternalClock(void);
extern void acq200_setExternalClock( 
	int DIx, unsigned divisor, int is_master, int DOx );
unsigned acq200_getDio(void);
void acq200_setDio(unsigned outputs, unsigned ones);
void configure_abort_code(void);

void acq200_getDioString(char *buf, int maxbuf);
void acq200_setDioString(char *string);


void acq200_setArm(void (*cb)(unsigned status));
void acq200_setOneshot(int oneshot);

int acq200_getNumSamples(void);
int acq200_getPreSamples(void);
int acq200_getPostSamples(void);
unsigned long long acq200_getElapsedSamples(void);


unsigned acq200_setChannelMask(unsigned mask);
unsigned acq200_getChannelMask(void);

unsigned acq200_getBigBufLen(void);
unsigned acq200_getNumChannels(void);
unsigned acq200_getAOcount(void);

int acq200_isAcq(void);
int acq200_isAcq216(void);
int acq200_isAcq232(void);
int acq200_isAcq196(void);
int acq200_isAcq132(void);
int acq200_isAcq164(void);

int acq200_isWav232(void);

int acq200_has_ics527(void);

int acq200_captureWasSuccess(void);


int set_ics527(int fin_khz, int fout_khz, char *errb, int maxerrb);

#define USER_LED3 3
#define USER_LED4 4
#define USER_LED_ON 1
#define USER_LED_OFF 0
#define USER_LED_FLASH -1

void acq200_setUserLed(int led, int on_off_flash);


#ifdef __cplusplus
}
#endif

#include <unistd.h>   /* usleep() */

enum STATE acq200_getState(void);
void acq200_setAbort(void);


typedef float VRANGE[4];

#define VRANGE_AI_MIN 0
#define VRANGE_AI_MAX 1
#define VRANGE_AO_MIN 2
#define VRANGE_AO_MAX 3

int acq200_getVRange(VRANGE vrange);

int acq200_getRange(char buf[], int maxbuf);

int acq200_muThrottle(int throttle_enable);
/** throttle_enable==1 reduces MU response during critical activity 
 * (disables int) 
 */

void fill_state_filter(int state);

void acq200_setState(enum STATE state);


void init_OIMR(char *value);
/* value to write (repeatedly) to OIMR */

void acq200_service(void);
/* service this value repeatedly */

#define HW_DEFLEN 0
void init_host_window_mapping(int len);
/* len == 0 means use system default */


int acq132_setBestDecim(int khz, int *khz_clock);
/* returns best fit actual clock, setting decimation if required */

/* OB_CLOCK definitions in kHz */
#define EXT_CLK_DEFAULT_KHZ		1000
#define ACQ132_SAMPLE_CLK_DEFAULT_KHZ	1000
#define ACQ164_SAMPLE_CLK_DEFAULT_KHZ	32768	/* /256 = 128 kSPS */

int set_acq132_clksel(const char* clksel);

#define ACQ132_DEFAULT_DECIMATE	1
#define ACQ132_DEFAULT_SHIFT	-2

void set_acq132_decimate(int decim, int shift);
#endif /* _ACQ200_HAL_H_ */

