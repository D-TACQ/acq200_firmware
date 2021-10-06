/*****************************************************************************
 *
 * File: llcontrol.c
 *
 * $RCSfile: llcontrol.c,v $
 * 
 * Copyright (C) 2001 D-TACQ Solutions Ltd
 *
 * Description:
 *     application implementing the LOW LATENCY CONTROL feture
 *
 * $Id: llcontrol.c,v 1.30.2.47 2011/12/29 09:46:46 pgm Exp $
 * $Log: llcontrol.c,v $
 * Revision 1.30.2.47  2011/12/29 09:46:46  pgm
 * remove build_prams from Makefile
 *
 * Revision 1.30.2.46  2011/09/29 20:19:51  pgm
 * *** empty log message ***
 *
 * Revision 1.30.2.45  2011/05/18 13:33:20  pgm
 * *** empty log message ***
 *
 * Revision 1.30.2.44  2011/05/18 13:32:07  pgm
 * doxygen changes
 *
 * Revision 1.30.2.43  2010/08/26 16:25:52  pgm
 * more switchable instrumentation
 *
 * Revision 1.30.2.42  2009/09/29 13:09:14  pgm
 * RFM emulation (not complete)
 *
 * Revision 1.30.2.41  2009/08/13 19:15:32  pgm
 * ALT doc
 *
 * Revision 1.30.2.40  2009/08/06 18:31:42  pgm
 * README.RFM
 *
 * Revision 1.30.2.39  2009/04/02 13:19:01  pgm
 * docs away
 *
 * Revision 1.30.2.38  2009/03/28 22:23:05  pgm
 * ao32cpci first cut looks good
 *
 * Revision 1.30.2.37  2009/03/28 18:47:39  pgm
 * sync2VAO32 take 1
 *
 * Revision 1.30.2.36  2007/02/19 15:22:23  pgm
 * *** empty log message ***
 *
 * Revision 1.30.2.35  2006/01/18 14:55:24  pgm
 * *** empty log message ***
 *
 * Revision 1.30.2.34  2006/01/15 11:18:18  pgm
 * SYNC_2V
 *
 * Revision 1.30.2.33  2006/01/09 13:40:31  pgm
 * SYNC_2V mode implemented
 *
 * Revision 1.30.2.32  2005/12/07 10:21:14  pgm
 * *** empty log message ***
 *
 * Revision 1.30.2.31  2005/12/06 21:22:40  pgm
 * ACQ216HS channels=2 pulls in antiphase
 *
 * Revision 1.30.2.30  2005/12/06 18:11:31  pgm
 * ACQ216HS (ChannelMask) option
 *
 * Revision 1.30.2.29  2005/11/30 12:03:36  pgm
 * *** empty log message ***
 *
 * Revision 1.30.2.28  2005/11/30 11:51:09  pgm
 * *** empty log message ***
 *
 * Revision 1.30.2.27  2005/11/30 10:52:53  pgm
 * *** empty log message ***
 *
 * Revision 1.30.2.26  2005/11/04 17:26:18  pgm
 * *** empty log message ***
 *
 * Revision 1.30.2.25  2005/10/22 19:38:36  pgm
 * *** empty log message ***
 *
 * Revision 1.30.2.24  2005/10/20 20:51:00  pgm
 * *** empty log message ***
 *
 * Revision 1.30.2.23  2005/10/03 15:03:48  pgm
 * acq216 llc with prams
 *
 * Revision 1.30.2.22  2005/09/25 20:31:50  pgm
 * samples
 *
 * Revision 1.30.2.21  2005/08/12 20:28:20  pgm
 * *** empty log message ***
 *
 * Revision 1.30.2.20  2005/08/12 09:51:25  pgm
 * *** empty log message ***
 *
 * Revision 1.30.2.19  2005/08/11 10:02:24  pgm
 * SYNC_ECM - init host side slave memory areas
 *
 * Revision 1.30.2.18  2005/08/01 12:39:55  pgm
 * includes bridge test
 *
 * Revision 1.30.2.17  2005/08/01 11:10:24  pgm
 * V2 part 1 running - local status, no hbpoll - ECM 100 acheived
 *
 * Revision 1.30.2.16  2004/12/13 21:39:15  pgm
 * correct DAC base offset
 *
 * Revision 1.30.2.15  2004/12/10 12:01:26  pgm
 * fix timing
 *
 * Revision 1.30.2.14  2004/12/09 22:31:57  pgm
 * microsecond timestamps do the BIZ - ECM20 here we go
 *
 * Revision 1.30.2.13  2004/12/09 12:58:34  pgm
 * various attempts at repartitioning for speed
 *
 * Revision 1.30.2.12  2004/11/04 07:14:02  pgm
 * w
 *
 * Revision 1.30.2.11  2004/11/04 07:13:13  pgm
 * merged multicard case
 *
 * Revision 1.30.2.9  2004/09/25 08:35:55  pgm
 * test mod
 *
 * Revision 1.30.2.8.2.9  2004/09/28 18:15:40  pgm
 * release to pcdaqdpcs
 *
 * Revision 1.30.2.8.2.8  2004/09/28 06:00:06  pgm
 * two card case
 *
 * Revision 1.30.2.8.2.7  2004/09/26 19:50:58  pgm
 * *** empty log message ***
 *
 * Revision 1.30.2.8.2.6  2004/09/26 15:28:40  pgm
 * *** empty log message ***
 *
 * Revision 1.30.2.8.2.5  2004/09/26 11:32:38  pgm
 * multiboard tstats pollstats in
 *
 * Revision 1.30.2.8.2.4  2004/09/25 21:16:28  pgm
 * multiboard case runs
 *
 * Revision 1.30.2.8.2.3  2004/09/25 11:53:03  pgm
 * first pass multi done - check it works for n==1
 *
 * Revision 1.30.2.8.2.2  2004/09/25 09:08:31  pgm
 * first UI/test split
 *
 * Revision 1.30.2.8.2.1  2004/09/25 08:40:57  pgm
 * multicard test commit
 *
 *
 *
\*****************************************************************************/



/** @file llcontrol.c <b>MAIN</b> llcontrol top level and "UI".
 * llcontrol -
 * - mmapMboxes() - get app mapping for mailboxes
 * - mmapBuffer() - get dmabuff and phys addresses
 * - runSCM() - capture using soft trigger
 * - runECM() - capture using external clock
 */

/** 

@mainpage llcontrol $Revision: 1.30.2.47 $

@author Peter Milne <peter.milne@d-tacq.com>

- Low Latency control operation:
 - Target push data transfer to slave memory on host card.
 - Data is delivered with minimum latency after the sample clock.

- For overview, please refer to 
 - http://www.d-tacq.com/resources/D-Tacq_2G_UserGuide.pdf

- For operating instructions, please refer to 
 - <a href="../README.ACQ196">README.ACQ196</a> readme file for ACQ196 operation
(deprecated)
 - <a href="../README.ACQ196.SYNC_2V">README.ACQ196.SYNC_2V</a> readme file for ACQ196
 operation SYNC_2V (preferred)
 - <a href="../README.ACQ216">README.ACQ216</a> readme file for ACQ216 operation
 - <a href="../README.AO32">README.AO32</a> readme file for AO32 operation
  - <a href="ao-ramps.png">ao-ramps.png</a>
  - <a href="walking-bit.png">walking-bit.png</a>
 - <a href="../README.RFM">README.RFM</a>   readme file for RFM/"ALTERNATE TARGET" operation

 - as a consequence of evolution, llcontrol has many operating modes
  - SOFT -> ECM -> syncECM -> SYNC2V
  - SYNC2V is the preferred mode.
   - SYNC2V_AO32 : SYNC2V with output to AO32 cards
   - SYNC2V+ALT : alternate VI destination eg RFM. 
	<b>NB<a> ALT does NOT affect llcontrol AT ALL.

 - llcontrol-core.c implements the core of the llcontrol loop, and ECM mode
 - llcontrol-sync2v-core.c implements <b>SYNC_2V</b> mode
 - llcontrol-sync2VAO32-core.c detail of <b>AO32CPCI</b> control

 - llcontrol-syncECM-core.c implements "SYNC_ECM mode" 
 - llcontrol-acq216-core.c detail of ACQ216 control
*
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

#ifdef __ACQ196__
#define FLAVOR "ACQ196"
#else
#define FLAVOR "ACQ32"
#endif

#define VERID "llcontrol $Revision: 1.30.2.47 $ B1051 " FLAVOR "\n"
extern const char* core_ident;
#define HELP \
"llcontrol [opts] mode \n"\
"mode:\n"\
"    A : Soft Clock Master - alt SCM [interval-usecs]\n"\
"    Bi: Ext Clock Slave   - alt ECS\n"\
"    Bii: Ext Clock Master - alt ECM <divisor>\n\n"\
"    SYNC_ECM <divisor> - Syncronous update ECM (LLCV2)\n"\
"    SYNC_2V <divisor> - Syncronous update 2V mode\n"\
"    SYNC_2VAO32 <divisor> AO32ID [AO32ID...]\n"\
"    N : null enter lowlatency mode and escape again\n"\
"    MBL : Measure Bridge Latency\n" \
"\n"\
"opts:\n"\
"    -b board [default:1]\n"\
"    -o file   output data (- is stdout)\n"\
"    -v verbose  verbose output\n"\
"    -d decimation {0..15} [default:1]\n"\
"    -t log clock times (embeds status info in data)\n" \
"    --tlogbin <file> log clock times binary out (better with octave)\n" \
"    -l use internal loopback\n"\
"    -n iterations [default:1]\n"\
"    -m <millisec> - monitor at this rate\n"\
"\n"\
"    -O overwrite data\n"\
"    -A append data\n"\
"    -T {+,-} trigger edge [default:-]\n"\
"    -C {+,-} clock edge   [default:-]\n"\
"\n"\
"    -W do \"work\" - copy data to user buffer in real time\n"\
"    -M <mask> use this interrupt mask\n"\
"    -F --feedback <channel> feed this data back to outputs\n"\
"    -c <channels> channel count [32]\n"\
"    --samples <len> #samples per cycle [1]\n"\
"    --hbpoll host buffer polling\n"\
"    --bigbuf   use bigbuf definition from driver\n"\
"    --dacs <def-file> update dacs: def-file has dac data definition\n" \
"    --mlt 1 : minimum latency timing - drops tinst tproc time query\n" \
"    --V2 1 : set V2 mode\n" \
"    --HWGO 1 : set hardware gate off (don't poll for counter stopped)\n" \
"    --prams <pram-file> configure prams each shot (ACQ216 only)\n" \
"    --cliplimit N - stats format control default: 100000\n"\
""


/*
 * globals (keep to minimum)
 */
int verbose = 0;


/*
 * class Testdescription 
 */
 


int G_quit;


static void setDefaults( struct TestDescription* td )
{
	td->mode = M_SCM;
	td->arg.interval = 0;
	td->iterations = 1;
	td->decimation = 1;
	td->overwrite = 0;
	td->clkpos = 1;
	td->trpos = 0;
	td->internal_loopback = 0;

	td->mask_ints = 0;
	td->mask_ints_mask = 0;

	td->update_dacs = 0;
	td->samples = 1;      /* samples > 1 is an acq216 thing */
#ifdef __ACQ196__
	td->channels = 64;     /* most popular llc variant */
#else
	td->channels = 32;
#endif

	if (isAcq216()){
		td->overwrite = 1;  
		td->hb_polling = 1;
	}
}

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


static void loadPramfile(struct TestDescription *td, const char* pram_file)
/** ACQ216 ONLY: pram_file contains binary array of struct LLC200_INIT.
 */
{

	if (!isAcq216()){
		return;
	}

	struct stat stats;

	if (stat(pram_file, &stats) == 0){
		int records = stats.st_size/LLC200_INIT_SZ;
			
		if (records*sizeof(struct LLC200_INIT) != stats.st_size){
			fprintf(stderr,
				"ERROR: pram file size mismatch "
				"%d * %lu != %d\n",
				records, LLC200_INIT_SZ, (int)stats.st_size);
			exit(1);
		}else{
			FILE *fp = fopen(pram_file, "r");

			assert(fp);

			td->llc200_init = calloc(records, LLC200_INIT_SZ);
			assert(td->llc200_init);
				
			fread(td->llc200_init, LLC200_INIT_SZ, records, fp);
			fclose(fp);
			td->llc200_init_count = records;
			fprintf(stderr, "prams: %d records loaded\n",
				records);
		}
	}else{
		fprintf(stderr, 
			"ERROR failed to open pram file \"%s\"\n",
			pram_file);
	}
}


static inline u32 makeChannelMask(struct TestDescription* td){
	int channels = td->channels;

	if (isAcq216HS()){
		switch(channels){
		case 2:
			return 0x000f;
		case 4:
			if (td->acq216hs_ch12ch34_bonding){
				return 0x000f;      
			}else{
				return 0x00f0;
			}
		case 8: 
			return 0x0ff0;
		default:
			return 0xfff0;
		}
	}else{
		switch(channels){
		case 4:
			return 0x000f;
		case 8:
			return 0x00ff;
		case 12:
			return 0x0fff;
		default:
			return 0xffff;
		}
	}
}

static void initPrams(struct TestDescription *td) 
/** force channel mask if not already set in prams data. */
{
	if (td->channels != 0 &&
	    (td->llc200_init->mask&LLC200_INIT_MASK_CHANNEL) == 0){
		td->llc200_init->channel_mask = makeChannelMask(td);
		if (isAcq216HS()){
			td->llc200_init->channel_mask |= 
				LLC200_INIT_CHANNEL_MASK_ANTIPHASE;
		}
		td->llc200_init->mask |= LLC200_INIT_MASK_CHANNEL;
		fprintf(stderr,
			"setChannelMask 0x%08x", 
			td->llc200_init->channel_mask);
	}
}

static int make_output_file(
	struct TestDescription* td, 
	int slot,
	const char* class)
{
	if (td->outfname == 0){
		return 0;                // no file 
	}else if ( td->outfname[0] == '-' ){
		return 1;                // use stdout
	}else{
		char outfname[128];
		int fd_out;

		sprintf(outfname, "%s.%d.%s", td->outfname, slot, class);
		fd_out = open( outfname, O_WRONLY|O_CREAT|O_TRUNC, 0666 );

		if (fd_out <= 0){
			fprintf(stderr, "ERROR: failed to open file %s\n",
				outfname);
			exit( -1 );
		}
		return fd_out;
	}
}

void updateTimingStats(
	struct TimingStats* buffer, int iter, struct TimingStats* tstats)
{
	struct TimingStats* cursor = &buffer[iter];

	memcpy(cursor, tstats, sizeof(struct TimingStats));
	cursor->iter = iter;
}


static int CLIP_LIMIT = 100000;

/** CLIP bogus times - not to try hide anything - big number spoils display */
#define CLIP(t) ((t) < 0? -1: (t) > CLIP_LIMIT? (-1): (t))

static inline int extractTprocess(struct TimingStats *ts)
/** if TP is available, USE IT, else calculate from tlatch, tinst
 *  this is valid because either it is accurate IOP calc, or 
 *  worst case Host calc
 */
{
	if (!isAcq216() && CLIP(ts->tprocess)){
		return CLIP(ts->tprocess);
	}else{
		return CLIP(ts->tinst) - CLIP(ts->tlatch);
	}
}

static void dumpTimingStats( struct TestDescription* td, FILE* fp )
{
#define HFMT "%8s, %10s, %10s, %8s, %8s, %6s, %4s, "
#define DFMT "%8d, %10d, %10d, %8d, %8d, %6d, %4d, "

	int iter;

	u32 old_tlatch[MAXCARDS] = {};
	EACHCARD_INIT;

	if (isAcq216()){
		CLIP_LIMIT = 10000000;
	}
	FOREACHCARD{
		fprintf(fp,  "%60s: %d", "CARD", EACHSLOT(td));
	}
	fprintf(fp, "\n");
	FOREACHCARD{
		int it;
		fprintf(fp,  HFMT, 
		       "iter", "tinst", "tlatch",  "tprocess", "tclock",
		       "hpol","tpol");
		for (it = 0; it != MAXTEST; ++it){
			fprintf(fp,  "%6d,", it);
		}
	}
	fprintf(fp, "\n");
    
	for (iter = 0; iter != td->iterations; ++iter){
		
		if ( iter && td->stats_buf[0][iter].iter == 0 ){
			fprintf(fp, "\n");
			break;
		}else       

		FOREACHCARD{
			struct TimingStats *ts = &td->stats_buf[icard][iter];
			int tclock = ts->tlatch - old_tlatch[icard];
			int it;
			int tp1 = 0;
			int tp2 = 0;

			fprintf(fp,  DFMT, 
				ts->iter,
				CLIP(ts->tinst),
				CLIP(ts->tlatch),
				extractTprocess(ts),
				CLIP(tclock),
				ts->hb_poll,
				ts->target_poll);
			old_tlatch[icard] = ts->tlatch;

			for (it = 0; it != MAXTEST; ++it){
				int tp = ts->test_points[it];
				if (tp){
					if (tp1 == 0){
						tp1 = tp;
					}else if (tp > tp2){
						tp2 = tp;
					}
				}
				fprintf(fp,  "%6d,", tp);
			}
			fprintf(fp, " T:%3d, ", tp2 - tp1);
		}

		fprintf(fp, "\n");
	}
#undef HFMT
#undef DFMT
#undef CLIP
}


static void dumpTimingStatsBin(struct TestDescription* td, const char *bfile)
{
	FILE* fp = fopen(bfile, "w");

	u32 tclock;
	int iter;
	EACHCARD_INIT;
	u32 old_tinst[MAXCARDS] = {};	

	assert(fp);

	for ( iter = 0; iter != td->iterations; ++iter ){	
		FOREACHCARD{
			struct TimingStats *ts = &td->stats_buf[icard][iter];

			fwrite(&ts, sizeof(struct TimingStats), 1, fp);
			tclock = ts->tinst - old_tinst[icard];
			fwrite(&tclock, sizeof(u32), 1, fp);
			old_tinst[icard] = ts->tinst;
		}
	}
	fclose(fp);
}

static void td_allocate_dac_data(struct TestDescription* td, int nsamples)
{
	td->dac_data = calloc(nsamples, DAC_SAMPLE_SIZE);
	assert(td->dac_data);
	td->dac_data_samples = nsamples;
	td->dac_cursor = 0;
}

static void td_load_dac_data_impulse(struct TestDescription* td)
/* load an impulse pattern to the dac data */
{
	td_allocate_dac_data(td, 128);
	((short*)td->dac_data)[0] = 0x7fff;  /* single impulse, channel 0 */

	((u32 *)(td->dac_data+128*DAC_SAMPLE_SIZE-4))[0] = 0xcafebabe;
}

static int get_file_size(const char* fname)
{
	struct stat stat_buf;

	if (lstat(fname, &stat_buf) == 0){
		return (int)stat_buf.st_size;
	}else{
		return 0;
	}
}

static void td_load_dac_data_file(
	struct TestDescription* td, const char* fname)
{
	FILE* fp = fopen(fname, "r");
	int fsize = get_file_size(fname);

	if (!fp || !fsize){
		char buf[128];
		sprintf(buf, "ERROR: failed to open file \"%s\"\n", fname);
		perror(buf);
		exit(errno);
	}
	
	if (fsize%DAC_SAMPLE_SIZE){
		fsize = (fsize%DAC_SAMPLE_SIZE) + DAC_SAMPLE_SIZE;
	}
	td_allocate_dac_data(td, fsize/DAC_SAMPLE_SIZE);
	fread(td->dac_data, sizeof(short), fsize/sizeof(short), fp);
	fclose(fp);
}

static void td_load_dac_data(struct TestDescription* td)
{
	if (strcmp(td->dac_data_file, "i") == 0){
		td_load_dac_data_impulse(td);
	}else{
		td_load_dac_data_file(td, td->dac_data_file);
	}
	td->update_dacs = 1;
	printf("PGMWASHERE: td->update_dacs set 1\n");
}





static void setEdge( int* flag, const char* arg )
{
	*flag = arg[0] == '+';
}



static void selectBoards(struct TestDescription *td, const char* board_def)
/* slots in comma sep list, max 8 */
{
	EACHCARD_INIT;

	td->ncards = sscanf(board_def, "%d,%d,%d,%d,%d,%d,%d,%d",
			    &td->cards[0].slot,
			    &td->cards[1].slot,
			    &td->cards[2].slot,
			    &td->cards[3].slot,
			    &td->cards[4].slot,
			    &td->cards[5].slot,
			    &td->cards[6].slot,
			    &td->cards[7].slot);

	FOREACHCARD{
		fprintf(stderr, "card %d\n", EACHSLOT(td));
	}
}


static void doWorkBufDataOutput(struct TestDescription *td)
{
	EACHCARD_INIT;

	FOREACHCARD{
		int iter;
		int offset = 0;
		int fd_out = make_output_file(td, EACHSLOT(td), "userbuf");

		for (iter = 0; iter != td->iterations; ++iter){
			write(fd_out, td->work_buf[icard]+offset,
			      td_sample_size(td)*td->samples);
			offset += td_sample_size(td)*td->samples;
		}
		close(fd_out);
	}
}
static void doDmaBufDataOutput(struct TestDescription *td)
{
	EACHCARD_INIT;

	FOREACHCARD{
		int iter;
		int offset = 0;
		int fd_out = make_output_file(td, EACHSLOT(td), "dmabuf");

		for (iter = 0; iter != td->iterations; ++iter){
			write(fd_out, getVaddr(EACHBUF(td), offset), 
			      td_sample_size(td)*td->samples);
			if (td->overwrite){
				break;
			}
			offset += td->sample_offset;

		}
		close(fd_out);
	}
}
static void doPostShotAnalysis(struct TestDescription *td)
{
	if ( td->tlog ){
		if (td->tlog_binfile != 0){
			dumpTimingStatsBin(td, td->tlog_binfile);
		}else{
			FILE *fp = fopen("/tmp/llcontrol.tstats", "w");
			dumpTimingStats(td, fp);
			fclose(fp);
			printf("timing stats stored in /tmp/llcontrol.tstats\n");
//			dumpTimingStats(td, stdout);
		}
	}    
	if ( td->outfname != 0 ){
		doDmaBufDataOutput(td);
		if (td->do_work){
			doWorkBufDataOutput(td);
		}
	}
}


static void initWorkBuf(struct TestDescription *td)
{
	EACHCARD_INIT;
	int len = td->samples * td_sample_size(td)*td->iterations;

	FOREACHCARD{
		td->work_buf[icard] = malloc(len);
		if(td->work_buf[icard] == 0){
			perror("malloc failed");
			exit(errno);
		}
	}
}

extern void initV2Stats(struct TestDescription* td);

static void initV2(struct TestDescription *td)
/** LLC V2 initialization - pick up prams from config file. */
{
	fprintf(stderr, "initV2\n");
	td->V2_BUFS[LLCV2_INIT_MARKER] = LLCV2_INIT_MAGIC_MARKER;
/** WORKTODO V2 */
        td->V2_BUFS[LLCV2_INIT_AI_HSBT] = V2_NOBUF;
	td->V2_BUFS[LLCV2_INIT_AO_HSBS] = V2_NOBUF;
	td->V2_BUFS[LLCV2_INIT_DO_HSBS] = V2_NOBUF;
	td->V2_BUFS[ LLCV2_INIT_STATUS_HSBT] = LLCV2_OFFSET_STATUS_HSBT;

	td->hb_polling = 0;
	initV2Stats(td);
}

static int is_v2 = 0;

int main( int argc, const char* argv[] )
{
	struct TestDescription *td = calloc(1,sizeof(struct TestDescription)); 
	const char* arg;
	int rc;
	char* board_def = "1";
	int monitor = 0;
	/* WARNING: really should use bigbuf ALL THE TIME */
	int use_bigbuf = 1;
	char *pram_file;
	EACHCARD_INIT;
	
    
	struct poptOption opt_table[] = {
		{ "help",       'h', POPT_ARG_NONE,   0,             'h'  },
		{ "version",    'V', POPT_ARG_NONE,   0,             'V'  },
		{ "verbose",    'v', POPT_ARG_INT,    &verbose,       0   },
		{ "output",     'o', POPT_ARG_STRING, &td->outfname,  0   },
		{ "decimation", 'd', POPT_ARG_INT,    &td->decimation, 0  },
		{ "histogram",  't', POPT_ARG_NONE,   0,             't'  },
		{ "iterations", 'n', POPT_ARG_INT,    &td->iterations, 0  },
		{ "board",      'b', POPT_ARG_STRING, &board_def,    'X'  },
		{ "internal",   'l', POPT_ARG_NONE,   0,             'l'  },
		{ "monitor",    'm', POPT_ARG_INT,    &monitor,       0   },
		{ "overwrite",  'O', POPT_ARG_NONE,   0,             'O'  },
		{ "append",     'A', POPT_ARG_NONE,   0,             'A'  },
		{ "trgedge",    'T', POPT_ARG_STRING, &arg,          'T'  },
		{ "clkedge",    'C', POPT_ARG_STRING, &arg,          'C'  },
		{ "int_mask",   'M', POPT_ARG_STRING, &arg,          'M'  },
		{ "bigbuf",     'B', POPT_ARG_NONE,   0,             'B'  },
		{ "samples",   'L', POPT_ARG_INT,    &td->samples       },
		{ "dacs",       'D', POPT_ARG_STRING, &td->dac_data_file,'D'},
		{ "hbpoll",     ' ', POPT_ARG_NONE,   0,              'H' },
		{ "channels",   'c', POPT_ARG_INT,    &td->channels,   'c'  },
		{ "feedback",   'F', POPT_ARG_INT,  &td->feedback_channel,'F'},
		{ "tlogbin",    ' ', POPT_ARG_STRING, &arg,           'b' },
		{ "work",       'W', POPT_ARG_NONE,   0,   'W' },
		{ "mlt",        0,  POPT_ARG_INT,    &td->min_latency_test,0},
		{ "V2",         0,  POPT_ARG_INT,     &is_v2, 0 },
		{ "HWGO",       0,  POPT_ARG_INT,   &td->hardware_gate_off, 0},
		{ "prams",      0,  POPT_ARG_STRING, &pram_file, 'P' },
		{ "cliplimit",  0,  POPT_ARG_INT,    &CLIP_LIMIT, 0 },
		{ }
	};

	poptContext opt_context;
	setDefaults( td );


	opt_context = poptGetContext(argv[0], argc, argv, opt_table, 0);

	while ( (rc = poptGetNextOpt( opt_context )) > 0 ){
		switch( rc ){
		case 'X':
			selectBoards(td, board_def);
			break;
		case 'V':
			fprintf(stderr, VERID);
			fprintf(stderr, core_ident);
			break;
		case 'h':
			fprintf(stderr, VERID);
			fprintf(stderr, core_ident);
			fprintf( stderr, HELP );
			return 1;
		case 'b':
			td->tlog_binfile = arg;  /* fall thru */
		case 't':
			td->tlog = 1;
			break;
		case 'W':               
			td->do_work = 1;
			break;
		case 'O':
			td->overwrite = 1;
			break;
		case 'A':
			td->overwrite = 0;
			break;    
		case 'T':
			setEdge(&td->trpos, arg);
			break;
		case 'C':
			setEdge(&td->clkpos, arg);
			break;
		case 'l':
			td->internal_loopback = 1;
			break;    
		case 'M':
			td->mask_ints = 1;
			td->mask_ints_mask = strtoul( arg, 0, 0 );
		case 'B':
			use_bigbuf = 1;
			break;
		case 'H':
			td->hb_polling = 1;
			break;
		case 'D':
			td_load_dac_data(td);
			break;
		case 'F':
			td->feedback = 1;
			break;
		case 'P':
			loadPramfile(td, pram_file);
			break;
		case 'c':
			if (td->channels == 2 && isAcq216HS()){
				fprintf(stderr, "ANTIPHASE\n");
				td->acq216hs_ch12ch34_bonding = 1;
				td->channels = 4;           /* interleaved */
			}
		}
	}  // processes all other opts via arg pointers

	if ( (arg = poptGetArg( opt_context )) == NULL ){
		fprintf( stderr, "please specify mode\n" );
		return 1;
	}else{
		const char* arg2 = poptGetArg( opt_context );
            
		if ( STREQ( arg, "A" ) || STREQ( arg, "SCM" ) ){
			td->mode = M_SCM;
			if ( arg2 ){
				td->arg.interval = atoi( arg2 );
			}
		}else if ( STREQ( arg, "Bi" ) || STREQ( arg, "ECS" ) ){
			td->mode = M_ECS;
		}else if ( STREQ( arg, "Bii" ) || STREQ( arg, "ECM" ) ){
			td->mode = M_ECM;
			if ( arg2 == 0 ){
				fprintf( stderr, "please specify divisor\n" );
				return 1;
			}else{
				td->arg.divisor = atoi( arg2 );
			}
		}else if (STREQ(arg, "SYNC_ECM")){
			td->mode = M_SYNC_ECM;
			if ( arg2 == 0 ){
				fprintf( stderr, "please specify divisor\n" );
				return 1;
			}else{
				td->arg.divisor = atoi( arg2 );
			}
		}else if (STREQ(arg, "SYNC_2VAO32")){
			td->mode = M_SYNC_2VAO32;
			if ( arg2 == 0 ){
				fprintf( stderr, "please specify divisor\n" );
				return 1;
			}else{
				td->arg.divisor = atoi( arg2 );
				while((arg = poptGetArg(opt_context)) != NULL){
					if (td->ao32_count < MAXAO32){
						td->ao32_ids[td->ao32_count++] 
								    = atoi(arg);
					}else{
						fprintf(stderr, 
						 "max ao32count=%d\n", MAXAO32);
					}
				}
			}			
		}else if (STREQ(arg, "SYNC_2V")){
			td->mode = M_SYNC_2V;
			if ( arg2 == 0 ){
				fprintf( stderr, "please specify divisor\n" );
				return 1;
			}else{
				td->arg.divisor = atoi( arg2 );
			}
		}else if (STREQ(arg, "SYNC_2VRFM")){
			td->mode = M_SYNC_2VRFM;
			if ( arg2 == 0 ){
				fprintf( stderr, "please specify divisor\n" );
				return 1;
			}else{
				td->arg.divisor = atoi( arg2 );
			}			
		}else if ( STREQ( arg, "N" ) || STREQ( arg, "NULL" ) ){
			td->mode = M_NULL;
		}else if (STREQ(arg, "MBL")){
			td->mode = M_MEASURE_BRIDGE;
		}else{
			fprintf( stderr, "sorry, don't understand mode \"%s\"\n", arg );
			return 1;
		}
	}


#ifdef __ACQ196__    
	FOREACHCARD{
		initCardResource(EACHCARD(td));
	}
#else
	if ( use_bigbuf ){
		td->buf = mmapBigBuffer(board, td->overwrite?0x10000:td_dss(td));
		mmapValidateDmaBuffer( td->mbx, td->iterations );
	}else{

		td->buf = mmapDmaBuffer( 
			board, td->overwrite? 0x10000: MAX(td_dss(td), 0x200000 ) 
			);

	}
#endif

	td->sample_offset = 256;  /* operate on 256 boundaries - easy debug */

	if (td->llc200_init_count){
		initPrams(td);
	}
	if (is_v2){
		initV2(td);
	}

	if (IS_SYNC_2V(td->mode)){
		td->sample_size = (td->channels + 32) * sizeof(short);
		fprintf(stderr, "M_SYNC_2V sample size %d\n",
			td_sample_size(td));
	}else{
		td->sample_size = td->channels * sizeof(short);
	}
	if (td->do_work){
		initWorkBuf(td);
	}

	setupAbortHandler(td);
	if ( monitor ){
		setupMonitor(monitor);
	}
	rc = runTest(td);    

	doPostShotAnalysis(td);

	return rc;
}






unsigned  llcv2_hb_offset = LLCV2_HB_OFFSET;
