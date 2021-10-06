/*****************************************************************************
 *
 * File: local-slave.c
 *
 * $RCSfile: local-slave.c,v $
 * 
 * Copyright (C) 2003 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description: local slave mode command handling. Implements ICD
 *
 * $Id: local-slave.c,v 1.1.2.40 2010/04/15 19:13:28 pgm Exp $
 * $Log: local-slave.c,v $
 * Revision 1.1.2.40  2010/04/15 19:13:28  pgm
 * channelMask for ACQ164 is same as ACQ196
 *
 * Revision 1.1.2.39  2010/02/14 13:46:06  pgm
 * all ACQ132_DEFAULT_SHIFT
 *
 * Revision 1.1.2.38  2010/02/14 13:34:40  pgm
 * set acq132 decimate 1 on acqcmd setExternalClock NOICS
 *
 * Revision 1.1.2.37  2010/02/12 13:55:04  pgm
 * new clksel for ACQ132
 *
 * Revision 1.1.2.36  2009/09/08 06:32:43  pgm
 * B2223 really handle defaults
 *
 * Revision 1.1.2.35  2009/09/08 05:57:28  pgm
 * B2222 provides ExternalClock defaults for ob_clock cards
 *
 * Revision 1.1.2.34  2009/09/07 22:06:31  pgm
 * solves the ocs527 external clock conundrum
 *
 * Revision 1.1.2.33  2009/09/06 19:01:15  pgm
 * acq200_isAcq164, fix channelMask
 *
 * Revision 1.1.2.32  2009/05/31 07:54:24  pgm
 * change ok if NOT_RUN rather than IDLE
 *
 * Revision 1.1.2.31  2009/03/30 07:35:20  pgm
 * B2212 - locks out commands in !=ST_STOP
 *
 * Revision 1.1.2.30  2008/07/02 11:31:40  pgm
 * drop a lot of acq32 crud
 *
 * Revision 1.1.2.29  2007/01/03 23:16:16  pgm
 * double fork() is good
 *
 * Revision 1.1.2.28  2006/06/16 11:46:35  pgm
 * int + rounded value output from getInternalClock
 *
 * Revision 1.1.2.27  2006/01/28 18:18:55  pgm
 * ST_CAPDONE
 *
 * Revision 1.1.2.26  2005/10/20 20:49:50  pgm
 * avoid ST_STOP race
 *
 * Revision 1.1.2.25  2005/09/17 18:31:06  pgm
 * computes elapsed samples correctly
 *
 * Revision 1.1.2.24  2005/07/26 12:19:15  pgm
 * B2111
 *
 * Revision 1.1.2.23  2005/02/06 16:14:18  pgm
 * setUserLed
 *
 * Revision 1.1.2.22  2004/12/16 17:06:09  pgm
 * fix zero prelen kludges
 *
 * Revision 1.1.2.21  2004/10/25 09:18:51  pgm
 * correct len 0x behanviour
 *
 * Revision 1.1.2.20  2004/10/23 14:18:16  pgm
 * rsh single path style
 *
 * Revision 1.1.2.19  2004/10/23 14:17:45  pgm
 * rsh single path style
 *
 * Revision 1.1.2.18  2004/10/21 09:28:44  pgm
 * DOx arg optional in setExternalClock
 *
 * Revision 1.1.2.17  2004/09/23 21:07:29  pgm
 * B2081
 *
 * Revision 1.1.2.16  2004/08/29 11:48:18  pgm
 * improve number enabled channels handling
 * also ext clk
 *
 * Revision 1.1.2.15  2004/06/12 16:56:11  pgm
 * fixes setChannelMask 196 bug
 *
 * Revision 1.1.2.14  2004/06/05 21:41:15  pgm
 * works with pre=0, trigger=event frig
 *
 * Revision 1.1.2.13  2004/05/18 20:12:08  pgm
 * B2060 setChannelMask ok ACQ196
 *
 * Revision 1.1.2.12  2004/05/16 07:22:18  pgm
 * full string setChannelMask, 3-blocked for acq196
 *
 * Revision 1.1.2.11  2004/04/25 12:57:47  pgm
 * concurrent queries work
 *
 * Revision 1.1.2.10  2004/04/17 20:33:50  pgm
 * refine channel handling
 *
 * Revision 1.1.2.9  2004/04/03 19:19:52  pgm
 * TRADITIONAL getStatus, getMode
 *
 * Revision 1.1.2.8  2004/03/26 11:11:48  pgm
 * acq196 mu
 *
 * Revision 1.1.2.7  2004/02/17 10:14:40  pgm
 * setSampleTagging stub required
 *
 * Revision 1.1.2.6  2004/02/15 22:04:57  pgm
 * *** empty log message ***
 *
 * Revision 1.1.2.5  2004/02/15 21:39:20  pgm
 * local slave command handling
 *
 * Revision 1.1.2.4  2004/02/15 16:19:00  pgm
 * local-slave help works
 *
 * Revision 1.1.2.3  2004/02/15 16:08:07  pgm
 * local-slave help works
 *
 * Revision 1.1.2.2  2004/02/15 15:08:03  pgm
 * *** empty log message ***
 *
 * Revision 1.1.2.1  2004/02/15 14:40:27  pgm
 * local-slave stub
 *
 */



#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/select.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>
#include <signal.h>

#include <assert.h>
#include <errno.h>

#include "local.h"

#include "acq200.h"
#include "usc.h"

#include "acq32_drv.h"


#include "popt.h"

#include "../../Linux/include/acq32ioctl.h" /* command strings def */

#include "acq200_hal.h"

#include "mbox_slave.h"
#include "dataHandling.h"

int acq32_emulation = 1;


#define TRADITIONAL 1   /* traditional: getMode, getState short form */


typedef int (*CmdFn)( int argc, char* argv[], struct Buf *buf );

#define CMDFN(name) static int name(int argc, char* argv[], struct Buf *buf)

struct command_def {
	char* key;
	char *prams;
	char* help;
	CmdFn cmd;
	u32 flags;
};

#define CMD_DEF_FLAGS_OK_WHEN_ARMED 1

#define BB (buf->buf+buf->len)
#define BB_HEADROOM (buf->maxlen - buf->len)

static int split(char *str, char *argv[], int maxargs);
static int process_command(int argc, char* argv[],  struct Buf *buf);

int ls_debug;

#define DBG if (ls_debug) dbg

void localSlave(struct IoBuf *cmd)
{       
	static int icommand = 1;

	char *argv[20];
	int argc = split(cmd->in.buf, argv, 20);
	int len;

	if (acq32_emulation){
		len = sprintf(cmd->out.buf, "ACQ32:");
	}else{
		len = sprintf(cmd->out.buf, "acq200.%d>", icommand++);
	}

	DBG(3, "output so far %s", cmd->out.buf );

	cmd->out.len = len;
	process_command(argc, argv, &cmd->out);
	strcat(cmd->out.buf, "\n");
	cmd->out.len++;

}


/*
 * cut'n paste alert : straight crib from acq32_command.c
 * "old code never dies"
 */

struct KeyLut {
    int   ikey;
    char* skey;
};


static struct KeyLut S_state_lut[] = {
    { ST_STOP,     MC_STATE_STOP    },
    { ST_ARM,      MC_STATE_ARM     },
    { ST_RUN,      MC_STATE_RUN     },
    { ST_TRIGGER,  MC_STATE_TRIGGER },
    { ST_POSTPROCESS, MC_STATE_POSTPROCESS },
    { ST_CAPDONE,  MC_STATE_CAPDONE },
    {}
};

#ifdef PGMCOMOUT
static int lkupKey( const char* value, struct KeyLut* lut, int nlut ) {
    int ii;
    
    for ( ii = 0; ii != nlut; ++ii ){
        if ( STREQ( lut[ii].skey, value ) ){
	    return lut[ii].ikey;
	}
    }
    
    return -1;
}
#endif

static char* getKeyFromState( enum STATE state )
{
    int ilut;

    for ( ilut = 0; S_state_lut[ilut].skey; ++ilut ){
        if ( state == (enum STATE)S_state_lut[ilut].ikey ){
            return S_state_lut[ilut].skey;
        }
    }
    
    return "ST_UNKNOWN";
}

static struct KeyLut S_mode_lut[] = {
    { M_GATED_TRANSIENT,      MC_MODE_GATED_TRANSIENT         },
    { M_GATED_CONTINUOUS,     MC_MODE_GATED_CONTINUOUS        },
    { M_SOFT_TRANSIENT,       MC_MODE_SOFT_TRANSIENT          },
    { M_SOFT_CONTINUOUS,      MC_MODE_SOFT_CONTINUOUS         },
    { M_TRIGGERED_CONTINUOUS, MC_MODE_TRIGGERED_CONTINUOUS    },
    {}  
};
#if 0
static int getModeFromKey( enum MODE* pmode, char* key )
{
    int ilut;

    for ( ilut = 0; S_mode_lut[ilut].skey; ++ilut ){
        if ( STREQ( key, S_mode_lut[ilut].skey ) ){
            *pmode = (enum MODE)S_mode_lut[ilut].ikey;
            return TRUE;
        }
    }
    
    return FALSE;
}
#endif
static char* getKeyFromMode( enum MODE mode )
{
    int ilut;

    for ( ilut = 0; S_mode_lut[ilut].skey; ++ilut ){
        if ( mode == (enum MODE)S_mode_lut[ilut].ikey ){
            return S_mode_lut[ilut].skey;
        }
    }
    
    return "M_UNKNOWN";
}


CMDFN(doHelp);
CMDFN(doGetFwrev);
CMDFN(doGetConfig);
CMDFN(doGetNumChannels);
CMDFN(doGetNumSamples);
CMDFN(doGetState);
CMDFN(doSetChannelMask);
CMDFN(doGetChannelMask);
CMDFN(doSetArm);
CMDFN(doSetAbort);
CMDFN(doSetMode);
CMDFN(doGetMode);
CMDFN(doSetMode);
CMDFN(doSetMode);
CMDFN(doSetInternalClock);
CMDFN(doSetExternalClock);
CMDFN(doGetInternalClock);
CMDFN(doGetFwrev);
CMDFN(doGetConfig);
CMDFN(doGetCalinfo);
CMDFN(doGetCaptureStats);
CMDFN(doGetMaxChannels);
CMDFN(doGetVrange);
CMDFN(doSetSampleTagging);
CMDFN(doSetDio);
CMDFN(doGetDio);
CMDFN(doSetUserLed);

static struct command_def command_def_tab[] = {
	{       .key = MC_SET_ABORT,
		.cmd = doSetAbort,
		.help = "abort capture",
		.flags = CMD_DEF_FLAGS_OK_WHEN_ARMED
	},
	{       .key = MC_GET_NUMCHANNELS,
		.help = "return available channels",
		.cmd = doGetNumChannels,
		.flags = CMD_DEF_FLAGS_OK_WHEN_ARMED
	},
	{       .key = MC_GET_NUMSAMPLES,
		.help = "return captured samples",
		.cmd = doGetNumSamples,
		.flags = CMD_DEF_FLAGS_OK_WHEN_ARMED
	},
	{       .key = MC_GET_STATE,
		.help = "return state of system",
		.cmd = doGetState,
		.flags = CMD_DEF_FLAGS_OK_WHEN_ARMED
	},
	{       .key = MC_SET_CHANNEL_MASK,
		.cmd = doSetChannelMask },
	{       .key = MC_GET_CHANNEL_MASK,
		.cmd = doGetChannelMask,
		.flags = CMD_DEF_FLAGS_OK_WHEN_ARMED
	},
	{       .key = MC_SET_ARM,
		.cmd = doSetArm },
	{       .key = MC_SET_MODE,
		.cmd = doSetMode },
	{       .key = MC_GET_MODE,
		.cmd = doGetMode,
		.flags = CMD_DEF_FLAGS_OK_WHEN_ARMED
	},
	{       .key = MC_SET_MODE_GC,
		.cmd = doSetMode },
	{       .key = MC_SET_MODE_TRC,
		.cmd = doSetMode },
	{       .key = MC_SET_INTERNAL_CLOCK,
		.cmd = doSetInternalClock },
	{       .key = MC_SET_EXTERNAL_CLOCK,
		.cmd = doSetExternalClock },
	{       .key = MC_GET_INTERNAL_CLOCK,
		.cmd = doGetInternalClock,
		.flags = CMD_DEF_FLAGS_OK_WHEN_ARMED
	},
	{       .key = MC_GET_FWREV,
		.cmd = doGetFwrev },
	{       .key = MC_GET_CONFIG,
		.cmd = doGetConfig },
	{       .key = MC_GET_CALINFO,
		.cmd = doGetCalinfo },
	{       .key = MC_GET_CAPTURESTATS,
		.cmd = doGetCaptureStats },
	{       .key = MC_GET_MAX_CHANNELS,
		.cmd = doGetMaxChannels },
	{       .key = MC_GET_VRANGE,
		.cmd = doGetVrange },
	{       .key = MC_SET_SAMPLE_TAGGING,
		.cmd = doSetSampleTagging },
	{       .key = SC_SET_DIO,
		.cmd = doSetDio,
		.flags = CMD_DEF_FLAGS_OK_WHEN_ARMED		
	},
	{       .key = SC_GET_DIO,
		.cmd = doGetDio,
		.flags = CMD_DEF_FLAGS_OK_WHEN_ARMED
	},
	{       .key = MC_SET_USER_LED,
		.cmd = doSetUserLed,
		.flags = CMD_DEF_FLAGS_OK_WHEN_ARMED
	},
	{ 	.key = "help", 
		.prams = "all|fun", 
		.help = "help on commands", 
		.cmd = doHelp 	},
	{0,}
};

#include <stdarg.h>

int iobPrintf(struct Buf *buf, const char *fmt, ...)
{
	va_list ap;
	int rc;

	va_start(ap, fmt);
	rc = vsprintf(buf->buf+buf->len, fmt, ap);
	va_end(ap);
	buf->len += rc;

	return rc;
}

int iobSpace(struct Buf *buf)
{
	return buf->maxlen - buf->len;
}

void iobSetStr(struct Buf *buf)
{
	buf->buf[buf->maxlen-1] = '\0';
	buf->len = strlen(buf->buf);
}


static int doSetDio(int argc, char* argv[], struct Buf *buf)
{
	acq200_setDioString(argv[1]);
	return 0;
}

static int doGetDio(int argc, char* argv[], struct Buf *buf)
{
	char dio_buf[128];
	acq200_getDioString(dio_buf, 128);
	sprintf(BB, "%s=%s", argv[0], dio_buf);
	iobSetStr(buf);
	return 0;
}

static int doGetConfig(int argc, char* argv[], struct Buf *buf)
{
	getConfig(buf->buf+buf->len, iobSpace(buf));
	iobSetStr(buf);
	return 0;
}
static int doGetFwrev(int argc, char* argv[], struct Buf *buf) 
{
	dbg(1, "iobSpace returns %d", iobSpace(buf));

	getBuildDate(buf->buf+buf->len, iobSpace(buf));
	iobSetStr(buf);
	return 0;
}


static int doGetNumChannels(int argc, char *argv[], struct Buf *buf)
{
        int ichannel;
	int nchan = acq200_getNumChannels();
        int imask = 0x1;
	unsigned long the_mask = acq32_getChannelMask(); /* 32 ch  WORKTODO*/
	int active = 0;

        for ( ichannel = 0; ichannel != nchan; ++ichannel){
		if ((the_mask&imask) != 0){
			++active;
		}

		if (acq200_isAcq196()){
			if ((ichannel&0x1f) == 0x1f){
				imask <<= 1;
			}
		}else{
			imask <<= 1;
		}
        }

	sprintf(BB, "%s=%d", argv[0], active);
	iobSetStr(buf);
	return 0;
}

static int doGetNumSamples(int argc, char *argv[], struct Buf *buf)
{
/* getNumSamples=12582912 pre=0 post=12582912 */
	sprintf(buf->buf+buf->len, "%s=%d pre=%d post=%d elapsed=%llu",
		argv[0],
		acq200_getNumSamples(),
		acq200_getPreSamples(),
		acq200_getPostSamples(),
		acq200_getElapsedSamples());
	iobSetStr(buf);
	return 0;
}

static int doGetState(int argc, char *argv[], struct Buf *buf)
{
	int state = mbox_getState();
#ifdef TRADITIONAL
	sprintf(BB, "%d %s", state, getKeyFromState(state) );
#else
	sprintf(BB, "%s=%d %s", argv[0], state, getKeyFromState(state) );
#endif
	iobSetStr(buf);
	return 0;
}


/*
 * Channel Mask WORKTODO: will NOT accomodate 96ch in 32 bit word! 
 */

static int doSetChannelMask(int argc, char *argv[], struct Buf *buf)
{
	unsigned cmask = 0;

	char *smask = argv[1];
	int nchan = acq200_getNumChannels();
	int ic;

	if (acq200_isAcq196() || acq200_isAcq164()){
		int got_a_one = 0;
		int block_mask = 1;

		for (ic = 0; ic != nchan && *smask != 0; ++ic){
			if (*smask++ == '1'){
				got_a_one = 1;
			}
			if ((ic&0x1f) == 0x1f || *smask == 0){
				if (got_a_one){
					cmask |= block_mask;
				}
				block_mask <<= 1;
				got_a_one = 0;
			}
		}
	}else{
		for ( ic = 0; ic != nchan && *smask != 0; ++ic ){
			cmask |= *smask++ == '1'? 1<<ic: 0;

			if ( !*smask ){
				break;
			}
		}
        }

	acq32_setChannelMask(cmask);
	return 0;
}
static int doGetChannelMask(int argc, char *argv[], struct Buf *buf)
{
        char mbuf[128];  /* fits 96 ch! */

        int ichannel;
	int nchan = acq200_getNumChannels();
        int imask = 0x1;
	unsigned long the_mask = acq200_getChannelMask(); /* 32 ch  WORKTODO*/

        for ( ichannel = 0; ichannel != nchan; ++ichannel){
		mbuf[ichannel] = the_mask&imask? '1': '0';

		if (acq200_isAcq196() || acq200_isAcq164()){
			if ((ichannel&0x1f) == 0x1f){
				imask <<= 1;
			}
		}else{
			imask <<= 1;
		}
        }
        mbuf[ichannel] = '\0';

	sprintf(BB, "%s=%s", argv[0], mbuf);
	iobSetStr(buf);
	return 0;
}

extern int mbox_setArm(void);

static int doSetArm(int argc, char *argv[], struct Buf *buf)
{
	mbox_setArm();
	return 0;
}

extern void acq200_setAbort(void);

static int doSetAbort(int argc, char *argv[], struct Buf *buf)
{
	acq200_setAbort();
	mbox_reportState( ST_STOP );
	return 0;
}

static int doSetSampleTagging(int argc, char *argv[], struct Buf *buf)
{
	/* STUB */
	return 0;
}


extern enum MODE G_mode;

static int doGetMode(int argc, char *argv[], struct Buf *buf)
{
#ifdef TRADITIONAL
	sprintf(BB, "%d %s", G_mode, getKeyFromMode(G_mode));
#else
	sprintf(BB, "%s=%d %s", 
		argv[0], G_mode, getKeyFromMode(G_mode));
#endif
	iobSetStr(buf);
	return 0;
}

extern int mbox_doSetMode(enum MODE mode, int pre, int post);


static int doSetMode(int argc, char *argv[], struct Buf *buf)
{
	unsigned pre = 0;
	unsigned post = 0;
	int valid_count = 0;
	int tc = 0;
	int gc = 0;

	if ((gc = STREQ(MC_SET_MODE_GC, argv[0])) ||
	    (tc = STREQ(MC_SET_MODE_TRC, argv[0]))     ){
		switch(argc){
		case 3:
			if (sscanf(argv[2], "0x%x", &post) == 1 ||
			    sscanf(argv[2], "%d", &post) == 1  ){
				valid_count++;
			}
			if (sscanf(argv[1], "0x%x", &pre) == 1 ||
			    sscanf(argv[1], "%d", &pre) == 1   ){
				valid_count++;
			}
			break;
		}		
		if (valid_count == 2){
			mbox_doSetMode(gc? 
				       M_GATED_CONTINUOUS:
				       M_TRIGGERED_CONTINUOUS,
				       pre, post);
		}else{
			sprintf(buf->buf+buf->len, "ERROR: pre post required");
		}
	}else if (STREQ(MC_SET_MODE, argv[0])){
		
		enum MODE mode;

		if (argc == 3 &&
		   (sscanf(argv[2], "0x%x", &post) == 1 ||
		    sscanf(argv[2], "%d", &post) == 1  )){
			if (STREQ(argv[1], MC_MODE_GATED_TRANSIENT)){
				mode = M_GATED_TRANSIENT;
			}else if (STREQ(argv[1], MC_MODE_SOFT_TRANSIENT)){
				mode = M_SOFT_TRANSIENT;
			}else if (STREQ(argv[1], MC_MODE_SOFT_CONTINUOUS)){
				mode = M_SOFT_CONTINUOUS;
			}else{
				sprintf(BB, "ERROR:BAD MODE %s", argv[1]);
				goto doSetMode99;
			}

			mbox_doSetMode(mode, pre, post);
		}else{
			sprintf(BB, "ERROR:BAD ARG %s", argc==3?argv[2]:"--");
		}
	}else{
		sprintf(buf->buf+buf->len, "INTERNAL ERROR %d", __LINE__ );

	}
 doSetMode99:
	iobSetStr(buf);
	return 0;
}
static int doSetInternalClock(int argc, char *argv[], struct Buf *buf)
{
	unsigned int_clock;
	int master = 0;
	int DO;

	if (argc == 3){
		if (sscanf(argv[2], "DO%d", &DO) == 1 && IN_RANGE(DO, 0, 5)){
			master = 1;
		}else{
			sprintf(BB, "ERROR bad arg \"%s\" must be DO[0-5]",
				argv[2]);
			goto _99;
		}
	}
	if (argc > 1 && sscanf(argv[1], "%u", &int_clock) == 1){
		acq200_setInternalClock(int_clock, master, DO);
	}else{
		sprintf(BB, "ERROR:BAD ARG %s", argc>1?argv[1]:"--");
	}

_99:
	iobSetStr(buf);
	return 0;
}

#define DMAX 0xfffe


static int doSetExternalClock(int argc, char *argv[], struct Buf *buf)
/* setExternalClock DIx [div DOx] */
{
	static int fin_khz;	/* popt quirk means must be static */
	static int fout_khz;
	static struct poptOption opt_table[] = {
		{ "fin",	0, POPT_ARG_INT,	&fin_khz,	'I' },
		{ "fout",	0, POPT_ARG_INT,	&fout_khz,	'O' },
		{ "help",     'h', POPT_ARG_NONE,	0,		'h' },
		POPT_TABLEEND
	};
	poptContext context;
	const char *arg;
	int master = 0;
	int div = 1;
	int DOx = -1;
	int DIx;
	int rc;


	/* options - fin, fout apply to ob_clock */
	if (acq200_isAcq164()){
		fin_khz = EXT_CLK_DEFAULT_KHZ;
		fout_khz = ACQ164_SAMPLE_CLK_DEFAULT_KHZ;
	}else{
		fin_khz = 0;
		fout_khz = 0;
	}

	context = poptGetContext(
		argv[0],  argc, (const char**)argv, opt_table, 0);

	while((rc = poptGetNextOpt(context)) >= 0){
		dbg(1, "poptGetNextOpt returns %d", rc);
		switch(rc){
		case 'h':
			sprintf(BB, 
		     "setExternalClock [--fin fin --fout fout] DIx [div DOx]");
			goto _99;
		default:
			;	
		}
	}

	/* args - MUST have at least DIx */
	if ((arg = poptGetArg(context)) != 0){
		if (!(sscanf(arg, "DI%d", &DIx) == 1 && IN_RANGE(DIx, 0, 5))){
			sprintf(BB, "ERROR bad arg \"%s\" must be DI[0-5]",
				argv[1]);
			goto _99;
		}		
	}else{
		sprintf(BB, "ERROR must provide DIx DI[0-5]");
		goto _99;
	}
	if ((arg = poptGetArg(context)) != 0){
		if (!(sscanf(arg, "%d", &div)==1 && IN_RANGE(div, 1, DMAX))){
			sprintf(BB, "ERROR bad arg \"%s\" div wanted",argv[2]);
			goto _99;
		}
	}
	if ((arg = poptGetArg(context)) != 0){
		if (sscanf(arg, "DO%d", &DOx) == 1 && IN_RANGE(DOx, 0, 5)){
			master = 1;
		}else{
			sprintf(BB, "ERROR bad arg \"%s\" must be DO[0-5]",
				argv[3]);
			goto _99;
		}  
	}

	/* ACTION */
	acq200_setExternalClock(DIx, div, master, DOx);

	if (acq200_has_ics527()){
		if (fout_khz != 0 && fin_khz != 0){
			if (acq200_isAcq132()){
				acq132_setBestDecim(fout_khz, &fout_khz);
				set_acq132_clksel("INTCLK_ICS");
			}
			set_ics527(fin_khz, fout_khz, BB, BB_HEADROOM);
		}else{
			if (acq200_isAcq132()){
				set_acq132_clksel("INTCLK_NOICS");
				set_acq132_decimate(
					ACQ132_DEFAULT_DECIMATE,
					ACQ132_DEFAULT_SHIFT
				);
			}
		}
	}
    _99:
	iobSetStr(buf);
	return 0;
}


static int doSetUserLed(int argc, char *argv[], struct Buf *buf)
/* setUserLed {3|4} {ON|OFF|FLASH} */
{
	if (3 != argc){
		sprintf(BB, "ERROR: setUserLed {3|4} {ON|OFF|FLASH}");
	}else{
		int led = atoi(argv[1]);

		if (!IN_RANGE(led, USER_LED3, USER_LED4)){
			sprintf(BB, "ERROR: setUserLed {3|4} \"%s\" invalid",
				argv[1]);
		}else{
			if (strcmp(MC_SET_USER_LED_ON, argv[2]) == 0){
				acq200_setUserLed(led, USER_LED_ON);
			}else if (strcmp(MC_SET_USER_LED_OFF, argv[2]) == 0){
				acq200_setUserLed(led, USER_LED_OFF);
			}else if (strcmp(MC_SET_USER_LED_FLASH, argv[2]) == 0){
				acq200_setUserLed(led, USER_LED_FLASH);
			}else{
				sprintf(BB, "ERROR: setUseLed \"%s\" invalid", 
					argv[2]);
			}
		}
	}
	iobSetStr(buf);
	return 0;
}
static int doGetInternalClock(int argc, char *argv[], struct Buf *buf)
{
	sprintf(BB,"%s=%d %.3e", argv[0], 
		acq200_getInternalClock(),
		(double)acq200_getInternalClock());
	iobSetStr(buf);
	return 0;
}
static int doGetCalinfo(int argc, char *argv[], struct Buf *buf)
{
	getCalInfo(BB, iobSpace(buf));
	iobSetStr(buf);
	return 0;
}
static int doGetCaptureStats(int argc, char *argv[], struct Buf *buf)
{
	getCaptureStats(BB, iobSpace(buf));
	iobSetStr(buf);
	return 0;
}
static int doGetMaxChannels(int argc, char *argv[], struct Buf *buf)
{
	sprintf(BB, "%s AI=%d AO=%d", argv[0], 
		acq32_getAIcount(), acq32_getAOcount());
	iobSetStr(buf);
	return 0;
}
static int doGetVrange(int argc, char *argv[], struct Buf *buf)
{
	acq200_getRange(BB, BB_HEADROOM);

	iobSetStr(buf);
	return 0;
}


#define VERID \
    "$Id: local-slave.c,v 1.1.2.40 2010/04/15 19:13:28 pgm Exp $\n"

static int doHelp(int argc, char* argv[], struct Buf *buf) 
{
	struct command_def *pcom;
	int print_all = 0;
	int ii;

	for (ii = 0; ii != argc; ++ii){
		if ( STREQNL( argv[ii], "all" ) ){
			print_all = 1;
		}
	}

	iobPrintf( buf, VERID);

	for ( pcom = command_def_tab; pcom->key != 0; ++pcom ){
		iobPrintf( buf, "%-20s%-35s%s\n", 
			   pcom->key, 
			   pcom->prams? pcom->prams: "none",
			   print_all && pcom->help ? pcom->help: "");
		break;
	}

	return 0;
}

#define NOT_RUN(state) ((state) != ST_ARM && (state) != ST_RUN)
#define CALL_OK(state, flags) \
	((state) == ST_STOP || ((flags)&CMD_DEF_FLAGS_OK_WHEN_ARMED) != 0)
static int process_command(int argc, char* argv[], struct Buf *buf)
{
	struct command_def *pcom;
	enum STATE state = acq200_getState();

	for (pcom = command_def_tab; pcom->key != 0; ++pcom){
		if (STREQ(argv[0], pcom->key)){
			if (CALL_OK(state, pcom->flags)){
				return pcom->cmd(argc, argv, buf);
			}else{
				iobPrintf(buf, "ERROR:not during RUN\n");	
				return -1;
			}
		}
	}

	for (pcom = command_def_tab; pcom->key != 0; ++pcom){
		if (STREQNL(argv[0], pcom->key)){
			if (CALL_OK(state, pcom->flags)){
				return pcom->cmd(argc, argv, buf);
			}else{
				iobPrintf(buf, "ERROR:not during RUN\n");	
				return -1;
			}
		}
	}

	iobPrintf(buf, "ERROR:command not found\n");
	return -1;
}


#if 0
#include <ctype.h>             /* causes GLIBC lib problem WORKTODO */
#endif

#define isspace(c) ((c)==' '||(c)=='\t'||(c)=='\n')

static int split(char *str, char *argv[], int maxargs)
{
#define INCR(argc) (argc<maxargs? argc++: argc)
#define DQUOTE '"'

	int argc = 0;
	char *ptok;
	char *pend;
	enum { IN_ARG, IN_QUOTED_ARG, IN_SPACE } state = IN_SPACE;

	for (ptok = pend = str; *pend; ++pend){
		switch(state){
		case IN_ARG:
			if (isspace(*pend)){
				*pend = '\0';
				argv[INCR(argc)] = ptok;
				ptok = pend + 1;
				state = IN_SPACE;
			}
			break;
		case IN_SPACE:
			if (*pend == DQUOTE){
				state = IN_QUOTED_ARG;
				ptok = pend+1;
			}else if (!isspace(*pend)){
				state = IN_ARG;
				ptok = pend;
			}/* else do nothing */
			break;
		case IN_QUOTED_ARG:
			if (*pend == DQUOTE){
				*pend = '\0';
				argv[INCR(argc)] = ptok;
				ptok = pend + 1;
				state = IN_SPACE;
			}
			break;
		default:
			assert(0);
		}
	}

	switch(state){
	case IN_ARG:
	case IN_QUOTED_ARG:
		argv[INCR(argc)] = ptok;
	default:
		;
	}
	return argc;
}
