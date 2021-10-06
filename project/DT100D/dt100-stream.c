/*****************************************************************************
 *
 * File: dt100-stream : streaming process for 2G
 *
 * $RCSfile: dt100-stream.c,v $
 *
 * Copyright (C) 2003 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: dt100-stream.c,v 1.23 2012/01/05 08:40:02 pgm Exp $
 * $Log: dt100-stream.c,v $
 * Revision 1.23  2012/01/05 08:40:02  pgm
 * env DT100D_NSUBFRAMES changes samples per frame
 *
 * Revision 1.22  2010/12/02 21:01:58  pgm
 * try avoid 32 bit overflows
 *
 * Revision 1.21  2010/12/02 20:05:46  pgm
 * phase sample start comes to tblock-stream
 *
 * Revision 1.20  2010/11/22 21:34:58  pgm
 * DT100D_TBLOCK_STREAM .. acq132 streaming
 *
 * Revision 1.19  2010/02/16 18:10:33  pgm
 * dt100-stream : MF all the time, prints sensible sample #'s, DIO
 *
 * Revision 1.18  2005/11/16 21:30:19  pgm
 * start timing
 *
 * Revision 1.17  2005/06/24 13:21:23  pgm
 * low latency option
 *
 * Revision 1.16  2005/03/18 16:30:36  pgm
 * runs forever
 *
 * Revision 1.15  2005/02/17 13:12:56  pgm
 * handles ES in continuous mode single frame MF layer
 *
 * Revision 1.14  2005/02/16 15:41:44  pgm
 * streams subset of channels
 *
 * Revision 1.13  2004/12/22 23:08:14  pgm
 * works with const 48 byte ESIG
 *
 * Revision 1.12  2004/12/21 21:12:52  pgm
 * burst streaming works again
 *
 * Revision 1.11  2004/12/21 15:32:35  pgm
 * dynamic EVENT_MAGIC
 *
 * Revision 1.10  2004/12/19 12:53:31  pgm
 * sf tag dets dio, dio32 defs
 *
 * Revision 1.9  2004/10/12 19:36:45  pgm
 * add sys type, make shell generic
 *
 * Revision 1.8  2004/06/12 16:52:07  pgm
 * handles scc correctly
 *
 * Revision 1.7  2004/06/10 22:05:11  pgm
 * MF streaming now works
 *
 * Revision 1.6  2004/06/06 12:59:34  pgm
 * multi-frame streaming works layer1
 *
 * Revision 1.5  2004/05/08 14:08:34  pgm
 * live streaming ship it quick
 *
 * Revision 1.4  2004/05/07 15:03:02  pgm
 * streams live data
 *
 * Revision 1.3  2004/05/04 12:53:10  pgm
 * cosmic ribbon simulation multibuf good
 *
 * Revision 1.2  2004/05/01 14:40:16  pgm
 * simulate as option
 *
 * Revision 1.1  2004/05/01 11:36:27  pgm
 * init test data version
 *
 * Revision 1.2.4.1  2003/09/08 21:12:06  pgm
 * *** empty log message ***
 *
 *
 *
\*****************************************************************************/


#define VERID "dt100-stream $ $Revision: 1.23 $ B1018\n"
                                                
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/signal.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <unistd.h>

#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <sys/time.h>

#include <sched.h>

#include "popt.h"

#include "local.h"                                
#include "acq200_root.h"

#include "acq32busprot.h"
#include "acq200-stream-api.h"



#define ACQ_LIVE_DATA "/dev/acq200/live"
#define ACQ_LIVE "/dev/acq200/live.offsets"
#define MAPLEN   0x1000000

#define RT_PRIORITY 1

struct FramePrams {
	int nchannels;
	int has_event;
	int iframe;

	short *data;
	u32 *event_sig;	
	unsigned short* tags;

	short *cursor;
};

int G_NSUBFRAMES = NSUBFRAMES;

static inline int sampleSize(struct FramePrams* fp) {
	return fp->nchannels * sizeof(short);
}
static inline int frameSize(struct FramePrams* fp) {
	return sampleSize(fp) * G_NSUBFRAMES;
}

static int S_burst_len;

static int esdump;


static struct timeval tv_start;



/*
 * decode ESIG 
 */


static u8 get8fromES(struct FramePrams* fp, int dst, int src)
{
	u8 xx = fp->event_sig[src];
	fp->tags[dst] = xx&0xff;
	return xx;
}
static u16 get16fromES(struct FramePrams* fp, int dst, int src)
{
	u16 xx = fp->event_sig[src];
	fp->tags[dst+0] = (xx >>8)&0xff;
	fp->tags[dst+1] = (xx    )&0xff;
	return xx;
}
static u32 get24fromES(struct FramePrams* fp, int dst, int src)
{
	u32 xx = fp->event_sig[src];
	fp->tags[dst+0] = (xx >> 16)&0xff;
	fp->tags[dst+1] = (xx >>  8)&0xff;
	fp->tags[dst+2] = (xx      )&0xff;

	return xx;
}
static u32 get32fromES(struct FramePrams* fp, int dst, int src)
{
	u32 xx = fp->event_sig[src];
	fp->tags[dst+0] = (xx >> 24)&0xff;
	fp->tags[dst+1] = (xx >> 16)&0xff;
	fp->tags[dst+2] = (xx >>  8)&0xff;
	fp->tags[dst+3] = (xx      )&0xff;

	return xx;
}
static u32 getBLEN(struct FramePrams* fp) {
	u32 blen = fp->event_sig[ESIG_BURST_LEN];
	return blen;
}
int acq200_debug;

static int help(void)
{
	fprintf(stderr, "help\n" );
	return 1;
}


static int incoming_size(void)
{
	static int _sample_size;

	if (!_sample_size){
		acq200_getRoot(DEV_ROOT,"sample_size",1,"%d",&_sample_size);
		assert(_sample_size);
	}

	return _sample_size;
}


unsigned long long total_scc = 0;

static void updateTotalScc(unsigned pss)
{
	static unsigned pss1;

	total_scc &= ~UINT_MAX;

	if (pss < pss1){
		if (pss1 > UINT_MAX - 10000000){	/* overflow */
			total_scc += (unsigned long long)UINT_MAX + 1;
		}else{
			total_scc = 0;			/* restart */
		}
	}

	total_scc |= pss;
	pss1 = pss;
}

static unsigned short build_tag_sf(int iline, struct FramePrams* not_used);

static unsigned short 
(*build_tag)(int iline, struct FramePrams* fp) = build_tag_sf;

static int get_next_frame_fullsize(int fd, struct FramePrams* fp);
static int get_next_frame_subsize(int fd, struct FramePrams* fp);

static int (*get_next_frame)(int fd, struct FramePrams* fp);

#define NTAGS_CACHE 16 /* max number of frames in flight */

static unsigned short* get_tags(void)
/* hands out next tags array from cache */
{
	static int init_done;
	static unsigned short *tag_cache[NTAGS_CACHE];
	static int icache;
	unsigned short* tags;

	dbg(1, "hello");	

	if (!init_done){
		int ic;

		for (ic = 0; ic != NTAGS_CACHE; ++ic){
			tag_cache[ic] = malloc(G_NSUBFRAMES*sizeof(short));
		}
		init_done = 1;
	}

	tags = tag_cache[icache];
	icache = ++icache < NTAGS_CACHE? icache: 0;
	return tags;
}


static int hasTrigger(int iline, struct FramePrams* fp)
{
	return 0;
}

static unsigned short build_tag_mf(int iline, struct FramePrams* fp)
{
	static unsigned long frame0_sample[2];
	unsigned nX;
	unsigned short * the_tag = &fp->tags[iline];
	unsigned short extra = 0xfeed;

	switch(iline){
	case MFX_SF0:
		*the_tag = SF0;
		frame0_sample[0] = total_scc & 0xffffffffUL;
		frame0_sample[1] = total_scc >> 32;
		total_scc += G_NSUBFRAMES;                         
		break;
	case MFX_SF1:
		*the_tag = SF1;
		break;
	case MFX_MF2:
		*the_tag = MF_MF2;
		break;
	case MFX_MF3:
		*the_tag = MF_MF3;
		break;
	case MFX_FNa:
		*the_tag = fp->iframe>>8;
		break;
	case MFX_FNb:
		*the_tag = fp->iframe&0x00ff;
		break;
	case MFX_Ja:
		get32fromES(fp, MFX_Ja, ESIG_JIFFIES); 
	case MFX_Jb:
	case MFX_Jc:
	case MFX_Jd:
		break;
	case MFX_ESa:
		get32fromES(fp, MFX_ESa, ESIG_ESIG);
	case MFX_ESb:
	case MFX_ESc:
	case MFX_ESd:
		break;
	case MFX_ESOFa:
		get24fromES(fp, MFX_ESOFa, ESIG_E2_OFFSET);
	case MFX_ESOFb:
	case MFX_ESOFc:
		break;
	case MFX_DIO:
		get8fromES(fp, MFX_DIO, ESIG_DIO);
		break;
	case MFX_EDIOa:
		get32fromES(fp, MFX_EDIOa, ESIG_EDIO);
	case MFX_EDIOb:
	case MFX_EDIOc:
	case MFX_EDIOd:
		break;
	case MFX_BLENa:
		get16fromES(fp, MFX_BLENa, ESIG_BURST_LEN);
	case MFX_BLENb:
		break;
	case MFX_BDELa:
		get16fromES(fp, MFX_BDELa, ESIG_BURST_DLY);
	case MFX_BDELb:
		break;
	case MFX_OVER: {
		u32 over = fp->event_sig[ESIG_FLAGS];
		if (over > 0xff) over = 0xff;
		*the_tag = (over)&0xff;
		break;
	}
	case MFX_MFNa:
		get32fromES(fp, MFX_MFNa, ESIG_MFN);
	case MFX_MFNb:
	case MFX_MFNc:
	case MFX_MFNd:
		break;
	case MFX_TVSa:
		get32fromES(fp, MFX_TVSa, ESIG_TVS);
	case MFX_TVSb:
	case MFX_TVSc:
	case MFX_TVSd:
		break;
	case MFX_TVUSa:
		get32fromES(fp, MFX_TVUSa, ESIG_TVUS);
	case MFX_TVUSb:
	case MFX_TVUSc:
	case MFX_TVUSd:
		break;
	default:
		*the_tag = (iline&0x1)? MF_FILLa: MF_FILLb;
	}

	*the_tag &= 0x00ff; 

	*the_tag |= (iline << s0_bit) | (hasTrigger(iline, fp) << T_bit);

	if (iline < 32){
		nX = (frame0_sample[0] >> iline) & 0x1U;
	}else if (iline < NID_BITS){
		nX = (frame0_sample[1] >> (iline-32)) & 0x1U;
	}else{
		nX = (extra >> (iline-NID_BITS))&0x1;
	}
	*the_tag |= nX << nX_bit;

	return *the_tag;
}


static void handle_es(struct FramePrams* fp, char *data)
{
#if 0
	unsigned scc = fp->event_sig[ESIG_SCC];
	/* beware 32 bit rollover - 29h @ 50kSPS */
	total_scc = scc;
#endif	
	memcpy(fp->event_sig, data, ESIG_LEN);
	build_tag = build_tag_mf;
	dbg(1, "Event 0x%08x scc %Lu", *(u32*)fp->event_sig, total_scc);
	fp->iframe = 0;

	if (esdump){
		fwrite(fp->event_sig, 2, 64, stderr);
	}
}


static void tag_gather_and_send(
	short *data, int nchannels, 
	struct FramePrams* fp,
	int discard)
{
	static struct iovec *iovec;

	unsigned short* tags = get_tags();	
	int iline = 0;	


	if (!iovec){
		iovec = malloc(2*G_NSUBFRAMES*sizeof(struct iovec));
		assert(iovec);
	}
	
	fp->tags = tags;
		
	for (iline = 0; iline != G_NSUBFRAMES; ++iline){
		iovec[iline*2].iov_base   = &tags[iline];
		iovec[iline*2].iov_len    = sizeof(short);
		tags[iline] = build_tag(iline, fp);
		iovec[iline*2+1].iov_base = data+(iline*nchannels);
		iovec[iline*2+1].iov_len  = sizeof(short)*nchannels;
	}


	dbg(1, "writev %d discard %d", 2*G_NSUBFRAMES, discard);

	if (!discard){

		int nwrite = writev(1, iovec, 2*G_NSUBFRAMES);

		if (acq200_debug){
			struct timeval tv_now;

			gettimeofday(&tv_now, 0);

			dbg(1, "writev:time:%6ld\n", 
			    (tv_now.tv_sec - tv_start.tv_sec) );
		}
		if (nwrite <= 0){
			err("ERROR in write\n");
		}	
	}
}

static void tag_gather_and_send_frame(struct FramePrams *fp, int discard) 
/*
 * WORKTODO: just call existing func for now, add extra structure later
 */
{
	u32 blen = getBLEN(fp);

	dbg(1, "hello");

	tag_gather_and_send(fp->data, fp->nchannels, fp, discard);

	/* increment frame number and reset on end of multiframe */

	dbg(1, "frame %d out of %d", fp->iframe, blen);

	if (++fp->iframe >= blen){
		fp->iframe = 0;
	}
}


static u32 _event_magic;
static u32 _event_magic_mask;


#define EVENT_MAGIC      _event_magic
#define EVENT_MAGIC_MASK _event_magic_mask

#define IS_EVENT_MAGIC(x) (((x)&~EVENT_MAGIC_MASK) == EVENT_MAGIC)


/*
 * don't need to check > 2 samples for sig, it has already been pre qualified
 * by the driver.
 */

#define IS_EVENT_SIGNATURE(data) \
       (IS_EVENT_MAGIC(((u32*)data)[0]) && \
        IS_EVENT_MAGIC(((u32*)data)[1]))


int continuous = 0;

static int get_next_frame_fullsize(int fd, struct FramePrams* fp)
/* get next frame. Only returns on frame complete (with ES) or error */
{
	int totread = 0;
	int nread;
	char* data = (char*)fp->data;
	int event_received = continuous != 0; /* no events if continuous */

	dbg(2, "starts...");
	for (totread = 0; totread < frameSize(fp); ){

		dbg(2,"calling read(%d %d %d)", 
		    fd, totread, frameSize(fp) - totread);

		nread = read(fd, data + totread, frameSize(fp) - totread);

		dbg(2,"read returns %d", nread);

		if (nread == ESIG_LEN){
			dbg(2, "ES? MAGIC 0x%08x EMM 0x%08x "
			       "[0x%08x 0x%08x] %s",
			    EVENT_MAGIC, EVENT_MAGIC_MASK,
			    ((u32*)data)[0], ((u32*)data)[1],
			    IS_EVENT_SIGNATURE(data)? "YES": "NO");
                }

		if (nread <= 0){
			return nread;     /* finished or error */
		}else if (nread == ESIG_LEN && IS_EVENT_SIGNATURE(data)){
			handle_es(fp, data);
			event_received = 1;
			totread = 0;
		}else{
			totread += nread;
		}
		dbg(2, "totread now %d", totread);
	}

	dbg(2,"returns 1");
	return 1;
}

static int get_next_frame_subsize(int fd, struct FramePrams* fp)
/* get next frame. Only returns on frame complete (with ES) or error */
{
	int totread = 0;
	int nread;
	char* data = (char*)fp->data;
	int event_received = continuous != 0; /* no events if continuous */

	dbg(2, "starts...");
	for (totread = 0; totread < frameSize(fp); ){

		dbg(2,"calling read(%d %d %d)", fd, totread, incoming_size());

		nread = read(fd, data + totread, incoming_size());

		dbg(2,"read returns %d", nread);

		if (nread == ESIG_LEN){
			dbg(2, "ES? MAGIC 0x%08x EMM 0x%08x "
			       "[0x%08x 0x%08x] %s",
			    EVENT_MAGIC, EVENT_MAGIC_MASK,
			    ((u32*)data)[0], ((u32*)data)[1],
			    IS_EVENT_SIGNATURE(data)? "YES": "NO");
                }

		if (nread <= 0){
			return nread;     /* finished or error */
		}else if (nread == ESIG_LEN && IS_EVENT_SIGNATURE(data)){
			handle_es(fp, data);
			event_received = 1;
			totread = 0;
		}else if (nread >= sampleSize(fp)){
			totread += sampleSize(fp);
		}else{
			return nread;    /* error */
		}
		dbg(2, "totread now %d", totread);
	}

	dbg(2,"returns 1");
	return 1;
}

#if 1
/* attemp congestion monitor - NFG */
#include <sys/types.h>
#include <sys/socket.h>
#include <linux/tcp.h>
//#include <linux/socket.h>
#include <netinet/in.h>
//#include <netinet/tcp.h>
#include <time.h>

#define SOL_TCP         6

#define MAX_UNACKED 2
#define MAX_CWND    5

static int lowlatency_mode = 0;
static int max_unacked = MAX_UNACKED;
static int max_cwnd = MAX_CWND;

static void lowLatencyDiscardReport(struct tcp_info* info, int discard) {
	static time_t last_time;
	static int stats[2];
	time_t now;

	stats[discard]++;

	now = time(0);

	if ((now - last_time) >= 60){
		info("sent %d discard %d th:%d %d",
		     stats[0], stats[1], max_unacked, max_cwnd);
		stats[0] = stats[1] = 0;
		last_time = now;
	}
}

static int _lowLatencyDiscard() {
	struct tcp_info info;
	socklen_t info_size = sizeof(info);
	int rc;
	
	info.tcpi_unacked = -1;
	info.tcpi_snd_cwnd = -1;

	rc = getsockopt(1, SOL_TCP, TCP_INFO, &info, &info_size);

	if (rc != 0){
		perror("getsockopt TCP_INFO failed");
		return 0;
	}else{
		int discard = info.tcpi_unacked > max_unacked && 
			      info.tcpi_snd_cwnd > max_cwnd;
		
		lowLatencyDiscardReport(&info, discard);
		return discard;
	}
}

static int lowLatencyDiscard() {
	if (lowlatency_mode){
		return _lowLatencyDiscard();
	}else{
		return 0;
	}
}
#else
#define lowLatencyDiscard() 0
#endif

static int bigbuf_stream_read(int stride, int flags, int nchannels)
{
	int fd = open(ACQ_LIVE_DATA, O_RDONLY);

	unsigned frame_sz = incoming_size()*G_NSUBFRAMES;
	short *data_buffers[NTAGS_CACHE];
	u32 *event_sig = malloc(incoming_size());
	int icache;
	struct FramePrams fp = { .nchannels = nchannels };

	assert(fd >= 0);
	assert(event_sig);

	
	for (icache = 0; icache != NTAGS_CACHE; ++icache){
		data_buffers[icache] = malloc(frame_sz);
		assert(data_buffers[icache]);
	}


	info("stride %d flags %d nchannels %d buffers %d", 
	     stride, flags, nchannels, NTAGS_CACHE);

	fp.event_sig = event_sig;

	while(1){
		dbg(2, "new frame");
		fp.data = data_buffers[icache = (icache+1)&(NTAGS_CACHE-1)];
		fp.has_event = 0;

		if (get_next_frame(fd, &fp) > 0){
			dbg(2, "got frame, now tagging");
			tag_gather_and_send_frame(&fp, lowLatencyDiscard());
		}else{
			break;
		}
	}

	dbg(2, "all done");

	free(event_sig);
	for (icache = 0; icache != NTAGS_CACHE; ++icache){
		free(data_buffers[icache]);
	}
	close(fd);

	return errno;
}


#define BURST_LEN	"burst_len"
#define _EVENT_MAGIC     "event_magic"
#define _EVENT_MAGIC_MASK "event_magic_mask"


static void getGlobalPrams(void)
{
	int len = 0;

	acq200_getRoot(DEV_ROOT, BURST_LEN, 1, "%d", &S_burst_len);

	continuous = len == 0;
	dbg(1, "%s mode selected %d", 
	    continuous? "continuous": "burst",
	    len);

	acq200_getRoot(DEV_ROOT, _EVENT_MAGIC, 1, 
		       "0x%x", &_event_magic);
	acq200_getRoot(DEV_ROOT, _EVENT_MAGIC_MASK, 1, 
		       "0x%x", &_event_magic_mask);

	/** optional - low latency parameter file */
	{
		FILE *fp = fopen("/tmp/dt100stream-lowlat", "r");
		if (fp){
			fclose(fp);
			acq200_getRoot(
				"/tmp", "dt100stream-lowlat", 3, "%d %d %d",
				&lowlatency_mode, &max_unacked, &max_cwnd);
		}
	}
}

#define TBSTAT_EV "/dev/acq200/data.dma/tbstat2"	
#define TBLOCK_LEN	0x600000


/* ACQ132 is grouped in DQUADS */

#define DQUAD		8
#define DQUAD_SZ	(DQUAD*sizeof(short))
#define DQUAD_LEN	128
#define DQUAD_STRIDE	(DQUAD * DQUAD_LEN)



struct TBLOCK_CURSOR {
	short *base;
	short *cursor;
};

static inline int frameFull(struct FramePrams *fp)
{
	int rc = (fp->cursor - fp->data) * sizeof(short) >= frameSize(fp);
	dbg(2, "%d", rc);
	return rc;
}
static inline int tblockCursorFull(struct TBLOCK_CURSOR *tbc)
{
	int ns = tbc->cursor - tbc->base;
	int rc = (ns + 4*DQUAD_STRIDE) > TBLOCK_LEN/sizeof(short);
	dbg(2, "%d", rc);
	return rc;
}

int big_stride;

static int get_next_tblock_frame(
	struct TBLOCK_CURSOR *tbc, struct FramePrams* fp)
/* get frameSize() samples from the block beginning pdata */
{
	int nblocks = fp->nchannels/8;
	int stride = DQUAD_STRIDE * nblocks * big_stride;

	dbg(1, "stride %d DQUAD_STRIDE %d", stride, DQUAD_STRIDE);

	while (!tblockCursorFull(tbc)){
		short *sample = tbc->cursor;
		int iblock;
		for (iblock = 0; iblock < nblocks; ++iblock){
			while(IS_EVENT_SIGNATURE(sample)){
				sample += DQUAD;
			}
			memcpy(fp->cursor, sample, DQUAD_SZ);
			sample += DQUAD_STRIDE;
			fp->cursor += DQUAD;
		}

		tbc->cursor += stride;

		dbg(2, "80: pos in tblock: %d tblockCursor %d frameFull %d",
		    tbc->cursor - tbc->base, 
			tblockCursorFull(tbc), frameFull(fp));
		
		if (frameFull(fp)){
			return 1;
		}
	}
	return 0;
}

static int acq132_tblock_stream_read(int stride, int flags, int nchannels)
{
	char tbnum[32];
	unsigned frame_sz = incoming_size()*G_NSUBFRAMES;
	short *data_buffers[NTAGS_CACHE];
	int icache;
	struct FramePrams fp = { .nchannels = nchannels };
	FILE *fd = fopen(TBSTAT_EV, "r");
	int len = 0x600000;		
	struct TBLOCK_CURSOR tbc = {};	
	int rc = 0;

	if (fd == 0){
		perror("failed to open " TBSTAT_EV);
		exit(errno);
	}

	for (icache = 0; icache != NTAGS_CACHE; ++icache){
		data_buffers[icache] = malloc(frame_sz);
		assert(data_buffers[icache]);
	}

	fp.data = data_buffers[0];
	fp.cursor = fp.data;
	fp.event_sig = malloc(incoming_size());

	while(fgets(tbnum, sizeof(tbnum), fd)){
		char tblock_id[8];
		char tbname[80];
		long pss = 0;		/* phase sample start */
		
		switch(sscanf(tbnum, "%3s %ld", tblock_id, &pss)){
		case 2:
			if (total_scc > pss){
				err("total scc fallback int:%lld ext:%ld\n",
				    total_scc, pss);
			}
			
			/* fall through */
		case 1:
			break;
		default:
			err("bad return from %s: %s", TBSTAT_EV, tbnum);
			rc = -1;
			goto cleanup;
		}

		chomp(tbnum);
		
		sprintf(tbname, "/dev/acq200/tblocks/%s", tblock_id);
		int tb_fd = open(tbname, O_RDONLY);
		short *pdata;

		if (tb_fd < 0){
			perror("failed to open tblock");
			exit(errno);
		}
		pdata = mmap(0, len, PROT_READ, MAP_PRIVATE, tb_fd, 0);
		tbc.base = tbc.cursor = pdata;

		dbg(1, "*******************************************");
		dbg(1, "TBLOCK:fname %s len %d pdata %p", tbname, len, pdata);

		if (pdata == MAP_FAILED) {
			
			perror("MAP_FAILED");
			exit(-1);
		}

		while(!tblockCursorFull(&tbc)){
			if (get_next_tblock_frame(&tbc, &fp) > 0){

				dbg(1, "let's go bokke");
				updateTotalScc(pss);
				tag_gather_and_send_frame(&fp, 0);
				dbg(1, "back from tag_gather_and_send");
				fp.data = data_buffers[
					icache=(icache+1)&(NTAGS_CACHE-1)];
				fp.cursor = fp.data;
			}
		}
		dbg(1, "01: %s", tbnum);

		munmap(pdata, len);
		close(tb_fd);
	}

cleanup:
	for (icache = 0; icache != NTAGS_CACHE; ++icache){
		free(data_buffers[icache]);
	}
	fclose(fd);
	
	return rc;
}

static void set_schedule_rt(void)
{
	struct sched_param sp = {
		.sched_priority = RT_PRIORITY,
	};
	if (sched_setscheduler(0, SCHED_RR, &sp)) {
		perror("sched_setscheduler");
	}
}

int main( int argc, const char **argv )
{
        struct poptOption opt_table[] = {
                { "help",       'h', POPT_ARG_NONE,   0,           'h' },
		{ "debug",      'd', POPT_ARG_INT,    &acq200_debug, 0 },
		{ "esdump",     'e', POPT_ARG_INT,    &esdump, 0},
		{}
	};
	
	const char *arg;
	int rc;
	int iarg;
	int stride = 1, flags = 0, nchannels = 96;
	int (*action)(int stride, int flags, int nchannels) = 
		bigbuf_stream_read;
	char *ve;

	if ((ve = getenv("DT100D_TBLOCK_STREAM"))){
		if (getenv("acq100") && strcmp(getenv("acq100"),"acq132") == 0){
			action = acq132_tblock_stream_read;	
			big_stride = atoi(ve);
		}
	}
	if ((ve = getenv("DT100D_NSUBFRAMES"))){
		G_NSUBFRAMES = strtoul(ve, 0, 0);
	}
	const char* env;

	info(VERID);
	poptContext context = 
		poptGetContext(argv[0], argc, argv, opt_table, 0);

	fprintf(stderr, VERID);

	build_tag = build_tag_sf;

        while((rc =poptGetNextOpt(context)) > 0){
                switch(rc) {
                case 'h':
                        return help();
                }
        }

	dbg(1, "debug level set %d", acq200_debug);


	for( iarg = 0; ((arg = poptGetArg(context)) != 0) && iarg < 4; ++iarg){
		switch(iarg){
		case 0:
			stride = atoi(arg); break;
		case 1:
			flags = atoi(arg);  break;
		case 2:
			nchannels = 2 * atoi(arg); break;
		}
	}

	dbg(1, "stride: %d flags %x nchannels %d",
	    stride, flags, nchannels);


	getGlobalPrams();

	if (nchannels*sizeof(short) == incoming_size()){
		get_next_frame = get_next_frame_fullsize;
	}else{
		get_next_frame = get_next_frame_subsize;
	}

	if ((env = getenv("DT100STREAM")) ){
		if (strstr(env, "sched-rr") != 0){
			set_schedule_rt();
		}else if (sscanf(env, "acq200_debug=%d", &acq200_debug) == 1){
			;
		}else{
			fprintf(stderr, "ERROR: unknown option %s\n", env);
			return -1;
		}
	}

	gettimeofday(&tv_start, 0);
/*
 * ride over reported driver problems. 
 * Usually errno=4 (EINTR) this is a generic internal fault code unfortunately
 */
	for (;;){
		int rc = action(stride, flags, nchannels);

		if (rc){
			err("action %d %d %d returned %d", 
			    stride, flags, nchannels, rc);
		}
	}
	
	return action(stride, flags, nchannels);
}




static inline int getTrigger(void)
{
	static int trigger;
	
	trigger = (trigger+1) & 0x7;

	return trigger == 0;
}

static unsigned short _dio = 0xbad1;

static inline void setDio(unsigned short dio)
{
	_dio = dio;
}


#define DIO6 "/dev/dtacq/dio_raw"

static u8 getDio(void)
{
	int fd = open(DIO6, O_RDONLY);
	u8 buf = 0;

	if (fd >= 0){
		read(fd, &buf, 1);
		close(fd);
	}
	return buf;
}

#define DIO32 "/sys/bus/platform/drivers/rtm/rtm0/dio32_raw"

static u32 getDio32(void)
{
	int fd = open(DIO32, O_RDONLY);
	u32 buf = 0;

	if (fd >= 0){
		read(fd, &buf, sizeof(buf));
		close(fd);
	}
	return buf;
}

//unsigned long long sample;
#define sample total_scc

unsigned short frame;

static unsigned short build_tag_sf(int iline, struct FramePrams* not_used)
{
	static unsigned long frame0_sample[2];
	static u32 dio32;
	static unsigned short extra;
	unsigned short the_tag;
	unsigned nX;
#define GET_BYTE(d, n) (((d)>>(n)) & 0x000000ff)

	switch(iline){
	case 0:
		the_tag = 0xfe;
		frame0_sample[0] = sample & 0xffffffffUL;
		frame0_sample[1] = sample >> 32;
		dio32 = getDio32();
		extra = frame++;
		break;
	case 1:
		the_tag = 0xed;
		break;
	default:
		if (iline&1){
			the_tag = GET_BYTE(dio32, (iline >> 1) & 0x3);
		}else{
			the_tag = getDio();			
		}
	}

	the_tag |= (iline << s0_bit) | (getTrigger() << T_bit);

	if (iline < 32){
		nX = (frame0_sample[0] >> iline) & 0x1U;
	}else if (iline < NID_BITS){
		nX = (frame0_sample[1] >> (iline-32)) & 0x1U;
	}else{
		nX = (extra >> (iline-NID_BITS))&0x1;
	}
	the_tag |= nX << nX_bit;

	sample++;
	return the_tag;
}

