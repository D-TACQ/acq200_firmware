/*****************************************************************************
 *
 * File: processMessage.c
 *
 * $RCSfile: processMessage.c,v $
 *
 * Copyright (C) 2003 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: processMessage.c,v 1.1.2.29 2009/06/05 20:06:27 pgm Exp $
 * $Log: processMessage.c,v $
 * Revision 1.1.2.29  2009/06/05 20:06:27  pgm
 * avoid opening multiple fd_dma paths for resource leak
 *
 * Revision 1.1.2.28  2007/05/10 21:04:25  pgm
 * *** empty log message ***
 *
 * Revision 1.1.2.27  2007/05/09 15:43:53  pgm
 * read_with_timeout
 *
 * Revision 1.1.2.26  2007/05/07 14:29:26  pgm
 * DMA_XXP, DMA_TBLOCK
 *
 * Revision 1.1.2.25  2007/04/18 19:44:20  pgm
 * handles AB buffers part 1
 *
 * Revision 1.1.2.24  2007/04/15 21:02:32  pgm
 * processFileDMA works again
 *
 * Revision 1.1.2.23  2007/04/13 17:13:20  pgm
 * B2184 hopefully fix hostwin init - both for usual [init] case and for reruns
 *
 * Revision 1.1.2.22  2007/01/02 18:24:19  pgm
 * trim from 106K to 82K
 *
 * Revision 1.1.2.21  2007/01/02 17:46:16  pgm
 * usc call acqcmd works @ 25 cps
 *
 * Revision 1.1.2.20  2007/01/01 20:55:05  pgm
 * text acqcmd works - use minor 137
 *
 * Revision 1.1.2.19  2006/08/19 15:31:29  pgm
 * *** empty log message ***
 *
 * Revision 1.1.2.18  2006/08/19 14:45:32  pgm
 * B2126
 *
 * Revision 1.1.2.17  2006/08/19 12:53:11  pgm
 * dual thread handles mbox calling interp
 *
 * Revision 1.1.2.16  2004/11/01 12:22:27  pgm
 * 2100
 *
 * Revision 1.1.2.15  2004/10/26 11:23:54  pgm
 * aggregating DMA Direct 50M bytes/sec
 *
 * Revision 1.1.2.14  2004/10/25 20:57:15  pgm
 * DMA Direct Works
 *
 * Revision 1.1.2.13  2004/10/25 09:19:18  pgm
 * dma experiments - no work, no crash
 *
 * Revision 1.1.2.12  2004/10/23 14:17:45  pgm
 * rsh single path style
 *
 * Revision 1.1.2.11  2004/03/23 22:45:45  pgm
 * *** empty log message ***
 *
 * Revision 1.1.2.10  2003/12/12 14:19:15  pgm
 * *** empty log message ***
 *
 * Revision 1.1.2.9  2003/11/08 14:48:16  pgm
 * channel mask, pre post sorted
 *
 * Revision 1.1.2.8  2003/11/05 22:29:57  pgm
 * hooks GATED_TRANSIENT, dio
 *
 * Revision 1.1.2.7  2003/11/03 11:29:53  pgm
 * chan def is 0... on wire, 1.. logical
 *
 * Revision 1.1.2.6  2003/10/19 20:44:30  pgm
 * works with dt100rc
 *
 * Revision 1.1.2.5  2003/09/13 13:52:39  pgm
 * DMA IN, PIO OUT works well enough
 *
 * Revision 1.1.2.4  2003/09/11 23:11:53  pgm
 * dma ops incoming good
 *
 * Revision 1.1.2.3  2003/09/10 20:33:19  pgm
 * split File IO prep for DMA, PIO still works
 *
 * Revision 1.1.2.2  2003/09/09 20:56:25  pgm
 * *** empty log message ***
 *
 * Revision 1.1.2.1  2003/09/08 21:41:05  pgm
 * *** empty log message ***
 *
 *
 *
\*****************************************************************************/


#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

#include <assert.h>

#include "local.h"
#include "dt100api.h"
#include "acq32busprot.h"

#include "acq32_drv.h"
#include "acq32cpci_drv.h"

#include "dataHandling.h"

#include "acq32_hml.h"

#include "stream.h"
#include "mbox_slave.h"

#include "acq200-mu-app.h"

#define THEFILEFMT "/tmp/acq200tmp_%02d"

#include "processMessage.h"

#include <errno.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "acq200.h"
#include "etcp.h"
#include "usc.h"

static struct timeval dma_timeout = {
	.tv_sec = 10
};

#define DMA_TO	&dma_timeout

extern volatile char *host_win_mapping; /* _really_ want dma WORKTODO */


static unsigned long G_po_usleep;  /* [TMP] delay before posting outbound */

static void dump32(void* data, int ndata)
{
	FILE* fp = popen( "/bin/od -X", "w");

	if (!fp) return;
	fwrite(data, 1, ndata, fp);
	pclose(fp);
}


static void adump( volatile char* buf, int maxbuf )
{
	char aline[33];
	int iline, ibuf;

	aline[32] = '\0';

	for (ibuf = 0; ibuf != maxbuf; ){
		for (iline = 0; 
		     iline != 32 && ibuf != maxbuf; 
		     ++iline, ++ibuf){
			char achar = buf[ibuf];

			aline[iline] = achar>=' '&&achar<='}'? achar: '.';
		}
		
		/* debug 1 means read but don't print */
		dbg(2, ": %s", aline );
	}
}

static void dumpData( char* ident, 
		      volatile char* map, 
		      u32 offset,
		      int nbytes )
{
	if ( acq200_debug ){
		dbg(1, "%20s %p 0x%08x", ident, map, offset );
		adump(map+offset, nbytes );
		dbg(1, "adios" );
	}
}
#define DATAROOTFMT "/dev/acq200/data/%02d"
#define WAVEROOTFMT "/dev/wav232/data/%d/%02d"

static char *chan2fname( int chan, char buf[] )
{
	if (chan&HRD_CHAN_IS_FILE){
		char *fmt = (chan&0x80)? THEFILEFMT: DATAROOTFMT;
		sprintf(buf, fmt, (chan&0x7f)+1);
		return buf;
	}else if (chan&HRD_WAVETRACK){
		sprintf(buf, WAVEROOTFMT, HRD_TRACK(chan), HRD_CHAN(chan));
		return buf;		
	}
	return 0;
}

#define DMAROOT	       "/dev/acq200/data.dma/"
#define DATAROOTFMTDMA DMAROOT "%02d"
#define DMAXXP	       DMAROOT "XXP"
#define DMATBLOCK      DMAROOT "tblock"

static char *chan2fnameDMA( int chan, char buf[] )
{
	if (chan&HRD_CHAN_IS_FILE){
		int my_chan = chan & 0x00ff;
		if (my_chan > 96){
			if ((my_chan&~1) == 124){
				strcpy(buf, DMATBLOCK);
			}else{
				strcpy(buf, DMAXXP);
			}
		}else{
			sprintf(buf, DATAROOTFMTDMA, (my_chan&0x7f)+1);
		}
		return buf;
	}
	return 0;
}



#define FIO_WRITE 1
#define FIO_READ  0



static int doFileIO(
	void *pbuffer,
	int maxlen,
	char *fname,
	int write_mode,
	int first_time,
	int last_time
	)
/** 
 * bi-di file read function
 * of course we don't want to be reading at all - let the DMAC do the lifting
 */
{
	static FILE* file;
	int numio = 0;

	dbg(1, "%s file:%p first:%d maxlen:%d last:%d\n",
	    fname, file, first_time, maxlen, last_time );

	if (first_time && file){
		dbg(2, "first_time && file close()");
		fclose(file);
		file = 0;
	}
	if (!file){
		file = fopen(fname, write_mode? "w": "r");

		if (!file){
			err("FAILED to open file \"%s\"\n", fname);
		}
		dbg(1, "fopen %s returns %p", fname, file);		
	}
	if (maxlen > 0){
		if (write_mode){
			numio = fwrite(pbuffer, 1, maxlen*2, file);
			fflush(file);
		}else{
			numio = fread(pbuffer, 1, maxlen*2, file);
		}
		if (numio&1){
			if (!write_mode){
				((char*)pbuffer)[numio] = '\n';
			}
			numio += 1;
		}
		numio /= 2;
	}
	if (last_time && file){
		dbg(2, "last_time&&file close");
		fclose(file);
		file = 0;
	}
	dbg(1, "numio %d", numio);
	return numio;
}





static volatile void* mapping;

static int __processFileIO(
	Message *message, int write_mode, char* fname, void* pbuffer)
{
	HostRequestDataRecord *hrdr = &message->payload.hrdr[0];
	int nsamples;
	int eof = (hrdr->chan&HRD_CHAN_EOF) != 0;
	int last_time = eof;
	int first_time = hrdr->start==0 && !eof;

	dbg(2, "LINE %d nsamples %d %s %p", __LINE__, hrdr->nsamples, 
	    (hrdr->chan&HRD_CHAN_EOF)!=0? "EOF":"---", pbuffer );

	nsamples = hrdr->nsamples = doFileIO(
		(void*)pbuffer,
		hrdr->nsamples,
		fname,
	        write_mode,
		first_time,
		last_time );


	dbg(2, "LINE %d nsamples %d %s", __LINE__, hrdr->nsamples, 
	    (hrdr->chan&HRD_CHAN_EOF)!=0? "EOF":"---" );
	
	return nsamples;
}

static int _processFileIO(Message *message, int write_mode)
{
	HostRequestDataRecord *hrdr = &message->payload.hrdr[0];
	volatile void *pbuffer = mapping + hrdr->pci;
	char fname[80];

	dbg(2, "mapping %p offset 0x%08x pbuffer %p",
	    mapping, hrdr->pci, pbuffer);

	return __processFileIO(message, write_mode, 
			       chan2fname(hrdr->chan, fname), (void*)pbuffer);
}

static int _processFileIOName(Message *message, int write_mode, char *fname)
{
	HostRequestDataRecord *hrdr = &message->payload.hrdr[0];
	volatile void *pbuffer = mapping + hrdr->pci;

	return __processFileIO(message, write_mode, fname, (void*)pbuffer);
}

static void processFilePIO(Message *message, int write_mode)
{
	_processFileIO(message, write_mode);

	i2o_postOutboundMessage(message);
}


static void processCommandIn(Message *message, int write_mode)
{
	HostRequestDataRecord *hrdr = &message->payload.hrdr[0];

	processFilePIO(message, write_mode);

	if ((hrdr->chan&HRD_CHAN_EOF) != 0){
		char job[80];
		char jobout[80];
		char cmd[128];
		int rc;

		chan2fname(hrdr->chan, job);
		chan2fname(hrdr->chan+1, jobout);
		
		sprintf(cmd, "chmod u+x %s;%s 2>&1 > %s", job, job, jobout);
		
		rc = system(cmd);
		dbg(1, "system(%s) returned %d\n", cmd, rc);
	}
}



static void processShortCommandWrite(
	char *command,
	char *jobout,
	int cnum)
{
	char buf[80];
	int rc;
	FILE* fpout = fopen(jobout, "w");
	FILE* fpin = popen(command, "r");


	dbg(1, "command \"%s\" len %d fpin %p", 
		command, strlen(command), fpin);
	buf[79] = '\0';

	if (!fpout){
		err("[%d] FAILED to open %s", __LINE__, jobout);
		return;
	}

	if (!fpin){
		err("[%d] FAILED popen %s", __LINE__, command);
		return;
	}

	while(fgets(buf, 79, fpin)){
		fputs(buf, fpout);
	}

	rc = pclose(fpin);

	fprintf(fpout, "EOF %d %d\n", cnum, rc);


	fclose(fpout);
}

static void processCommandIO(Message *message, int write_mode)
{
	HostRequestDataRecord *hrdr = &message->payload.hrdr[0];

	char job[80];
	char jobout[80];
	static int cnum;


	chan2fname(hrdr->chan, job);
	chan2fname(hrdr->chan+1, jobout);


	dbg(1, "%s job %s joubout %s nchars %d", 
	    write_mode == FIO_WRITE? "FIO_WRITE": "FIO_READ",
	    job, jobout, hrdr->nsamples * 2
        );
	dbg(2, "hrdr: 0x%08x %d %d %d %d", 
	    hrdr->pci, hrdr->start, hrdr->nsamples, hrdr->chan, hrdr->stride);

	if (write_mode == FIO_WRITE){
		char cmd[256];
		int rc;
		int nchars = hrdr->nsamples * 2;

		hrdr->chan |= HRD_CHAN_EOF;

		if (nchars < 80){
			char first_line[80];
			memcpy(first_line, (char*)mapping + hrdr->pci, nchars);
		
			first_line[nchars] = '\0';
			processShortCommandWrite(first_line, jobout, ++cnum);
			i2o_postOutboundMessage(message);
		}else{
			processFilePIO(message, write_mode);

			rc = sprintf(cmd, 
			     "chmod u+x %s;%s 2>&1 > %s;echo EOF %d >> %s", 
			     job, job, jobout, ++cnum, jobout);
	
			dbg(1, "call system \"%s\" len =%d", cmd, rc);
			rc = system(cmd);
			dbg(1, "system(%s) returned %d\n", cmd, rc);
		}
	}else{
		_processFileIOName(message, write_mode, jobout);
		i2o_postOutboundMessage(message);
	}
}

#define MAXBUF MESSAGE_DATA_LEN

static void processAcqcmd(Message *message, int write_mode)
{
	HostRequestDataRecord *hrdr = &message->payload.hrdr[0];

	static int sock;
	static enum State {
		ACQ_IDLE,
		ACQ_WRITE,
		ACQ_READ1,
		ACQ_READ_COMPLETE
	} state;
	int nchars = 0;

	dbg(1, "%s sock %d",		
	    write_mode == FIO_WRITE? "FIO_WRITE": "FIO_READ",
	    sock);

	hrdr->chan |= HRD_CHAN_EOF;

	if (write_mode == FIO_WRITE){
		if (state != ACQ_IDLE){
			err("unexpected state %d", state);
		}
		state = ACQ_WRITE;
	}

	if (sock == 0){
		sock = getUnixSocket(COMMAND_SOCK);
	}

	switch(state){
	case ACQ_WRITE:
		nchars = hrdr->nsamples * 2;
		write(sock, (char*)mapping + hrdr->pci, nchars);
		state = ACQ_READ1;
		break;

	case ACQ_READ1 : {
		volatile char* buf = mapping + hrdr->pci;
/** gcc is _very_ picky about volatile - use union to fool it */
		union {
			volatile char* v;
			char *c;
		} pread;

		pread.v = buf;

#define BFULL (pread.v - buf)
#define BSPACE (MAXBUF - BFULL)

		while(1){
			int rc = readline(sock, pread.c, BSPACE);

			dbg(3, "readline returns %d", rc);

			if (rc <= 0){
				if (rc == 0){
					err("readline EOF");
					exit(1);
				}else{
					err("readline err %d", rc);
					exit(-errno);
				}
			}else{
				nchars += rc;
				if (strncmp(pread.c, "EOF", 3) == 0){
					pread.c += rc;
					break;
				}else{
					pread.c += rc;
				}					      
			}
		}		      

		if ((nchars&1) != 0){
			pread.c[nchars] = '\n';
			pread.c += 1;
			nchars += 1;
		}
		hrdr->nsamples = nchars/2;
/*
  dbg(2, "close %d", sock);
  close(sock);
  sock = 0;
*/
		state = ACQ_READ_COMPLETE;
	}	break;
	default:
		state = ACQ_IDLE;
	}
	i2o_postOutboundMessage(message);
#undef BSPACE
#undef BFULL
}




/*
 * @@WORKTODO - interactive comes later
 */
#define processCommandRsh processCommandIn

static volatile void* _setHostWinMapping(int incoming)
{
	return host_win_mapping;
}

/**
 * we use a despatch table allow substitution of other methods later
 * eg DMA out, PIO in etc,
 */
typedef void (*MessageProcessor)(Message *message, int write);

static MessageProcessor processFileIn = processFilePIO;
static MessageProcessor processFileOut = processFilePIO;
static volatile void* (*setMappingIn)(int incoming) = _setHostWinMapping;
static volatile void* (*setMappingOut)(int incoming) = _setHostWinMapping;


void acq200_processHostbound(Message *message)
/**
 * processes outbound message
 * @param message - the message
 */
{
	mapping = setMappingOut(0);

	if (message->payload.hrdr[0].chan&HRD_CHAN_IS_FILE){
		switch(HRD_SPCLID(&message->payload.hrdr[0])){
		case HRD_SPCLID_ACQCMD:
			processAcqcmd(message, FIO_READ);
			break;
		case HRD_SPCLID_BATCHIO:
			processCommandIO(message, FIO_READ);
			break;	
		default:

			processFileOut(message, FIO_READ);
		}
		return;
	}else if (message->payload.hrdr[0].chan&HRD_WAVETRACK){
		processFileOut(message, FIO_READ);
	}
	dbg( 1, "STUB" );

}
void acq200_processAcqbound(Message *message)
/**
 * processes inbound message
 * @param message - the message
 */
{

	mapping = setMappingIn(1);

	if (message->payload.hrdr[0].chan&HRD_CHAN_IS_FILE){

		MessageProcessor stash = processFileOut;

		switch(HRD_SPCLID(&message->payload.hrdr[0])){
		case HRD_SPCLID_ACQCMD:
			processFileOut = processFilePIO;
			processAcqcmd(message, FIO_WRITE);
			break;
		case HRD_SPCLID_BATCHTO:
			processFileOut = processFilePIO;
			processCommandIn(message, FIO_WRITE);
			break;
		case HRD_SPCLID_BATCHIO:
			processFileOut = processFilePIO;
			processCommandIO(message, FIO_WRITE);
			break;
		case HRD_SPCLID_BATCHRSH:
			processFileOut = processFilePIO;
			processCommandRsh(message, FIO_WRITE);
			break;
		default:
			processFileIn(message, FIO_WRITE);
		}
		processFileOut = stash;
		return;
	}else if (message->payload.hrdr[0].chan&HRD_WAVETRACK){
		processFileIn(message, FIO_WRITE);
	}
	dbg( 1,"STUB");	
}


static int mu_fd;

static int dma_write_rma( int write_mode, u32 offset, u32 length )
{
	struct mu_rma rma = { 
		.magic = MU_MAGIC,
		.buffer_offset = offset,
		.length = length
	};
	int rc;

	if (write_mode == FIO_WRITE){
		rma.magic |= MU_ACQBOUND;
	}else{
		rma.magic |= MU_HOSTBOUND;
	}

	dbg( 1, "%s writing rma %08x %08x %08x",
	     write_mode == FIO_WRITE?
	     "MU_ACQBOUND": "MU_HOSTBOUND",
	     rma.magic, rma.buffer_offset, rma.length );

	rc = write( mu_fd, &rma, sizeof(rma));
		     
	if (rc != sizeof(rma)){
		err( "write to /dev/mu_rma %d failed  %d", mu_fd, rc);
	}

	if (getenv("DMA_SLEEP_DEBUG")) {
		sleep( atoi(getenv("DMA_SLEEP_DEBUG")) );
	}
	return rc==sizeof(rma)? 0: -1;
}


static void processFileDMA(Message *message, int write_mode)
{
	HostRequestDataRecord *hrdr = &message->payload.hrdr[0];
	int nsamples;

	if (write_mode == FIO_WRITE && hrdr->nsamples > 0){
/*
 * INCOMING: first dma the data, then write the file
 */
		dma_write_rma(write_mode, hrdr->pci, hrdr->nsamples*2);


		dumpData( "window",  host_win_mapping, hrdr->pci, 80 );

	}

	nsamples = _processFileIO(message, write_mode);

	if (write_mode == FIO_READ &&  nsamples > 0){
/*
 * OUTBOUND: first read the file, then DMA it to host
 */
		dma_write_rma(write_mode, hrdr->pci, nsamples*2);

		dumpData( "HB window",  host_win_mapping, hrdr->pci, 80 );
	}

/*
 * FINALLY: acknowledge the message
 */
	i2o_postOutboundMessage(message);
}


#define MAX_RMA		(MAX_RMA_GROUP-1)


static int read_with_timeout(
	int fd, void* buf, int maxbuf, struct timeval *timeout)
{
	fd_set watchset;
	int rc;
	struct timeval _timeout;

	_timeout = *timeout;

	FD_ZERO(&watchset);
	FD_SET(fd, &watchset);

	rc = select(fd+1, &watchset, 0, 0, &_timeout);

	if (rc < 0){
		return rc;
	}else{
		return read(fd, buf, maxbuf);
	}
}

static void processFileDMADirect(Message *message, int write_mode)
{
	static struct mu_rma rma_group[MAX_RMA_GROUP];
	struct mu_rma* rma;
#define RMA_LEN ((void*)rma - (void*)rma_group)
#define RMA_GROUP_HEADROOM (sizeof(rma_group) - RMA_LEN)
	static int fd_dma;
	HostRequestDataRecord *hrdr = &message->payload.hrdr[0];
	int last_time = (hrdr->chan&HRD_CHAN_EOF) != 0;
	int first_time = first_time = hrdr->start==0 && !last_time;

	int rc = 0;
	int wc = 0;
	int nleft = hrdr->nsamples*2;
	int nfin  = nleft/8;			/* avoid short residue dma */
	int nread = 0;
	unsigned magic_flags = (hrdr->chan&HRD_ABS_PCI)? MU_PCI_ABS: 0;
	int irma = 0;
	int mal = MESSAGE_ACTUAL_LEN(message);


	if (write_mode == FIO_WRITE && hrdr->nsamples > 0){
		return;   /** @@todo INBOUND not supported */
	}

	if (first_time && fd_dma == 0){
		char fname[80];

		fd_dma = open(chan2fnameDMA(hrdr->chan, fname), O_RDONLY);

		dbg(1,"chan: %d opened %s fd_dma %d", 
		    HRD_CHANNEL(hrdr), fname, fd_dma);
		assert(fd_dma>0);
	}
	/**
         *    HACK ALERT: read on this device ONLY returns rma, not data
         *    but it does report #bytes AS IF it were.
         *    so reported bytes is variable and large, actual data == rma
         */

	for (rma = rma_group; irma < MAX_RMA && nleft > nfin && 
		     (rc = read_with_timeout(fd_dma, rma, nleft, DMA_TO)) > 0;
	     nread += rc, nleft -= rc, ++irma, ++rma){


		dbg(1, "read %d, hrdr->pci 0x%08x", rc, hrdr->pci);

		assert((nread&1) == 0);
		assert(MU_RMA_MAGIC(rma) == MU_MAGIC_BB);

		rma->bb_remote_pci_offset = hrdr->pci + nread;
		rma->magic |= magic_flags;

		dbg(3, "maybe call dump32 %s", acq200_debug? "YES": "NO");

		if (acq200_debug > 1){
			dump32(rma, sizeof(struct mu_rma));
		}
	}

	hrdr->nsamples = nread/2;     /** @@todo ASSUMES size 2 */


	if (nread && RMA_GROUP_HEADROOM >= mal + sizeof(u32)){
		/** append message to end of group to be posted after dma */
		rma->magic = MU_MAGIC_OB;
		memcpy(MU_RMA_PAYLOAD(rma), message, mal);
		if (acq200_debug > 1){
			dump32(rma, mal+4);
		}
		dbg(1, "write(mu_fd, %p, %d", rma_group, RMA_LEN+mal);

		wc = write(mu_fd, rma_group, RMA_LEN+mal);
	}else{
		if (nread){
		/* just write the first record. this is an error */
		        dbg(1, "writing one record ... must be an error ?");
			wc = write(mu_fd, rma_group, RMA_LEN);
		}
		/* nread == 0 => closedown */

		dbg(2, "finally postOut rc was %d wc %d message %p len %d", 
		    rc, wc,	message, mal);

		usleep(G_po_usleep);
		i2o_postOutboundMessage(message);
	}


	dbg(1, "99 postOutbound rc was %d wc %d loops %d %s", rc, wc,
	    irma-1,
	    last_time? "LAST_TIME":"continue");

	if (last_time){
		close(fd_dma);
		fd_dma = 0;
	}
}




/*
 * extern: setDmaMapping(direction)
 * if mapping already cached => no change
 * else close existing mapping
 * open in correct mode, make mapping
 * down in the driver, this sets the pci_dma_mapping directions correctly
 */
static volatile void* _setDmaMapping(int incoming)
{
	static void* dma_mapping;
	static int dma_mapping_incoming;
	static int mu_mapping_fd;


/*
 * mapping file is DIRECTIONAL
 */
	if (!dma_mapping || incoming != dma_mapping_incoming){
		if (mu_mapping_fd){
			munmap(dma_mapping, 0xf00000); 
			close(mu_mapping_fd);
		}
		mu_mapping_fd = open( "/dev/mu_rma", O_RDWR );
//				      incoming? O_WRONLY: O_RDONLY );
		
		if (mu_mapping_fd <= 0){
			perror("failed to open mu mapping");
			exit(1);
		}
		dbg( 1, "calling mmap on fd %d direction %s", 
		      mu_mapping_fd, incoming? "in (READ)": "out (WRITE)" );

/* 
 * INCOMING: DMA writes the physmem, We READ the MAPPING
 * OUTGOING: We WRITE the mapping, DMA reads the physmem
 */
		dma_mapping = mmap( 0, 0xf00000, 
				    incoming?           PROT_READ: 
				                        PROT_WRITE,
				    MAP_SHARED, mu_mapping_fd, 0 );

		if (dma_mapping == (void*)0xffffffff){
			perror("mapping failed" );
			exit(1);
		}else{

			dma_mapping_incoming = incoming;
		}
	}
	return dma_mapping;
}

void acq200_setTransferModeDma(int dma)
/**
 * sets bulk backplane pci transfer mode.
 * @param dma : mask:
 *         - 0x1 incoming dma 
 *         - 0x2 outbound dma
 */

{
	char mbuf[80];

	sprintf( mbuf, "%d ", dma );

	if (dma&1){
		strcat( mbuf, "IN=DMA ");
		processFileIn = processFileDMA;
		setMappingIn = _setDmaMapping;
	}else{
		strcat( mbuf, "IN=PIO " );
		processFileIn = processFilePIO;
		setMappingIn = _setHostWinMapping;
	}
	if (dma&2){
		strcat( mbuf, "OUT=DMA " );
		processFileOut = processFileDMA;
		setMappingOut = _setDmaMapping;
	}else{
		strcat( mbuf, "OUT=PIO " );
		processFileOut = processFilePIO;
		setMappingOut = _setHostWinMapping;
	}
	if (dma&4){
		strcat(mbuf, "OUT=DMADIRECT");
		processFileOut = processFileDMADirect;
#ifdef PGMCOMOUT
		setMappingOut = _setDmaMapping;
#else
		/* DMADIRECT doesn't need any mapping .. */
		setMappingOut = _setHostWinMapping;
#endif

	}

	if (getenv("ACQ200_PO_USLEEP")){
		G_po_usleep = strtoul(getenv("ACQ200_PO_USLEEP"),0,0);
	}

/*
 * message exchange file opens and stays open
 */
	if (dma && !mu_fd ){
		mu_fd = open( "/dev/mu_rma", O_RDWR);

		if (mu_fd <=0 ){
			perror( "failed to open /dev/mu_rma" );
			exit(1);
		}

		info( "mu_fd set %d", mu_fd );
	}

	info( "%s", mbuf );
}
