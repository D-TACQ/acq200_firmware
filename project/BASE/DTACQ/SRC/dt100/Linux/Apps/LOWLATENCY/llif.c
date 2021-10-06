/*****************************************************************************
 *
 * File:
 *
 * $RCSfile: llif.c,v $
 * 
 * Copyright (C) 2001 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description: implmentation of device driver interface for LLC
 *
 * $Id: llif.c,v 1.18.4.17 2006/02/22 10:25:47 pgm Exp $
 * $Log: llif.c,v $
 * Revision 1.18.4.17  2006/02/22 10:25:47  pgm
 * fix dbg snafu
 *
 * Revision 1.18.4.16  2006/02/15 10:10:03  pgm
 * *** empty log message ***
 *
 * Revision 1.18.4.15  2006/01/19 20:17:36  pgm
 * *** empty log message ***
 *
 * Revision 1.18.4.14  2006/01/19 20:15:28  pgm
 * *** empty log message ***
 *
 * Revision 1.18.4.13  2006/01/15 11:18:18  pgm
 * SYNC_2V
 *
 * Revision 1.18.4.12  2005/11/04 17:26:18  pgm
 * *** empty log message ***
 *
 * Revision 1.18.4.11  2005/10/22 12:18:56  pgm
 * *** empty log message ***
 *
 * Revision 1.18.4.10  2005/08/22 09:20:40  pgm
 * remove acq32 dependency
 *
 * Revision 1.18.4.9  2005/07/29 20:05:31  pgm
 * can find prams on cmdline
 *
 * Revision 1.18.4.8  2004/12/09 12:58:34  pgm
 * various attempts at repartitioning for speed
 *
 * Revision 1.18.4.7  2004/11/04 07:13:13  pgm
 * merged multicard case
 *
 * Revision 1.18.4.6.2.5  2004/09/28 06:00:06  pgm
 * two card case
 *
 * Revision 1.18.4.6.2.4  2004/09/26 19:50:58  pgm
 * *** empty log message ***
 *
 * Revision 1.18.4.6.2.3  2004/09/26 15:28:14  pgm
 * correct big buf mapping
 *
 * Revision 1.18.4.6.2.2  2004/09/26 11:32:39  pgm
 * multiboard tstats pollstats in
 *
 * Revision 1.18.4.6.2.1  2004/09/25 11:53:03  pgm
 * first pass multi done - check it works for n==1
 *
 * Revision 1.18.4.6  2004/09/15 16:59:27  pgm
 * llif.c
 *
 * Revision 1.18.4.5  2004/09/15 12:48:21  pgm
 * use internal var for llWaitDmaDone, reformat
 *
 * Revision 1.18.4.4  2004/08/10 07:35:46  pgm
 * works with ACQ196
 *
 * Revision 1.18.4.3  2004/07/27 20:49:06  pgm
 * llcontrol 2G with AO - in debug
 *
 * Revision 1.18.4.2  2004/07/25 20:39:38  pgm
 * hbpolling, 96 channels
 *
 * Revision 1.18.4.1  2004/07/25 20:34:59  pgm
 * ACQ196 pass 1
 *
 * Revision 1.18  2003/02/14 09:48:03  pgm
 * bigbuf def now comes from driver
 *
 * Revision 1.17  2002/09/25 09:50:49  pgm
 * opt to use bigbuf
 *
 * Revision 1.16  2002/09/02 16:26:24  pgm
 * USE_IOCTL_DOC
 *
 * Revision 1.15  2002/09/02 15:36:14  pgm
 * hook up int masking
 *
 * Revision 1.14  2002/09/02 11:01:18  pgm
 * revised debug levels
 *
 * Revision 1.13  2002/04/02 10:15:38  pgm
 * emacs indent, untabify
 *
 * Revision 1.12  2001/09/07 15:36:43  pgm
 * ioctl() version works
 *
 * Revision 1.11  2001/09/07 12:17:57  pgm
 * USE_IOCTL - access to regs by deref
 *
 * Revision 1.10  2001/08/23 20:00:50  pgm
 * mod protocol avoids races and works
 *
 * Revision 1.9  2001/05/25 13:12:42  pgm
 * embedded stats
 *
 * Revision 1.8  2001/05/25 12:05:15  pgm
 * its a runner - shippit quick
 *
 * Revision 1.7  2001/05/23 19:16:21  pgm
 * doc
 *
 * Revision 1.6  2001/05/21 17:42:38  pgm
 * reformed
 *
 * Revision 1.5  2001/05/20 21:24:10  pgm
 * ll WIP - almost works
 *
 * Revision 1.4  2001/05/19 19:46:15  pgm
 * enters LL mode with good bufs, mboxes
 *
 * Revision 1.3  2001/05/19 07:03:40  pgm
 * LL done for SOFT CLOCK, compiles
 *
 * Revision 1.2  2001/05/18 20:21:13  pgm
 * compiles. needs algorithms
 *
 * Revision 1.1  2001/05/18 07:09:37  pgm
 * first cut - wont compile :-(
 *
 * Revision 1.2  2000/12/28 11:03:00  pgm
 * *** empty log message ***
 *
 * Revision 1.1  1999/10/22 16:26:49  pgm
 * first entry to cvs
 *
 *
\*****************************************************************************/

/** @file llif.c  implementation of device driver interface for LLC.
 * define USE_IOCTL to use ioctls to access mbox. 
 *  this is slow, but guaranteed portable
 * 
 * if undefined, memory mapped accesss is used - much faster, but x86 only
 */

//#define USE_IOCTL


#include "local.h"


#include <assert.h>
#include "llif.h"

#include <stdio.h>
#include <stdlib.h>

#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>


#include "acq32ioctl.h"



#ifdef USE_IOCTL
#define USE_IOCTL_DOC	       "USE_IOCTL - portable but slow"
#else
#define USE_IOCTL_DOC	       "NOT USE_IOCTL memory mapped - x86 only FAST"
#endif


#ifndef dbg
#define dbg(level, fmt, arg...)  fprintf(stderr, fmt, ## arg)
#endif

typedef volatile u32 r32;

struct MU {
	int fd;                         // alternate - use ioctl to access
	r32* mailboxes;                 // or mapping (x86 only)
	u32 mailboxes_shadow[4];        // up to dat cache of write values
	u32 mailboxes_lastwrite[4];     // what did we write last ??
	r32 *inbound_q_port;            /* inbound on target */
	r32 *outbound_q_port;

	int poll_count;
};

#define FNAME_FMT        "/dev/acq32/acq32.%d."
#define FNAME_FMT_MBOX   FNAME_FMT "raw"
#define FNAME_FMT_DMABUF FNAME_FMT "01"    // don't ask - this is a convenience

#define FNAME_FMT_HOST   FNAME_FMT "host"

int setMbox( struct MU* m, int ibox, u32 value )      
// set a mail mbox register: ibox {0..3}. return 0 on success
{
#ifdef USE_IOCTL
	int rc;
#endif
	assert( IN_RANGE( ibox, 0, 3 ) );

    
	PRINTF(3)( "setMbox( %d, 0x%08x )\n", ibox, value );
#ifndef USE_IOCTL           
	m->mailboxes[ibox] = 
#endif
		m->mailboxes_shadow[ibox] = 
		m->mailboxes_lastwrite[ibox] = value;    

#ifdef USE_IOCTL
	rc = ioctl( m->fd, ACQ32_IOSMBX(ibox), value );
    
	PRINTF(3)( "ioctl( %d, %d, %08x ) returns %d\n",
		   m->fd, ACQ32_IOSMBX(ibox), value, rc );
                        
	assert( rc >= 0 );
#endif
	return 0;   
}    

void showLastWrites( struct MU* m )
{
	fprintf( stderr, "lastWrites()\n" );
	fprintf( stderr, "0x%08x  0x%08x  0x%08x  0x%08x\n",
		 m->mailboxes_lastwrite[0],
		 m->mailboxes_lastwrite[1],
		 m->mailboxes_lastwrite[2],
		 m->mailboxes_lastwrite[3] );
}
int setMboxBits( struct MU* m, int ibox, u32 bits_to_set )
{
	assert( IN_RANGE( ibox, 0, 3 ) );
    
	m->mailboxes[ibox] = m->mailboxes_shadow[ibox] |= bits_to_set;
	return 0;
}

int clrMboxBits( struct MU* m, int ibox, u32 bits_to_clr )
{
	assert( IN_RANGE( ibox, 0, 3 ) );
    
	m->mailboxes[ibox] = m->mailboxes_shadow[ibox] &= ~bits_to_clr;
	return 0;
}

int setMboxField( struct MU* m, 
		  int ibox, u32 field_mask, u32 field_value )
{
	assert( IN_RANGE( ibox, 0, 3 ) );
    
	m->mailboxes_shadow[ibox] &= ~field_mask;
	m->mailboxes_shadow[ibox] |= field_value;
	m->mailboxes[ibox] = m->mailboxes_shadow[ibox];
	return 0;
}

u32 getMbox( struct MU* m, int ibox )      
// get contents of mailbox register: ibox {0..3}
{
	u32 newval;
#ifdef USE_IOCTL
	int rc;
#endif
    
	assert( IN_RANGE( ibox, 0, 3 ) );

#ifdef USE_IOCTL
	rc =  ioctl( m->fd, ACQ32_IOGMBX(ibox), &newval );
	assert( rc >= 0 );

	PRINTF(4)( "ioctl( %d, %d, %p ) output 0x%08x\n",
		   m->fd, ACQ32_IOGMBX(ibox), &newval, newval );
#else
	newval = m->mailboxes[ibox];
#endif
    
	if ( newval != m->mailboxes_shadow[ibox] ){
		PRINTF(4)( "getMbox( %d ) = 0x%08x\n", 
			   ibox, m->mailboxes_shadow[ibox] );
	}
    
	m->mailboxes_shadow[ibox] = newval;
	return m->mailboxes_shadow[ibox];
}

u32 getMboxShadow(struct MU* m, int ibox)
{
	assert( IN_RANGE( ibox, 0, 3 ) );
	return m->mailboxes_shadow[ibox];
}

u32 pollMboxBits( struct MU* m, int ibox, u32 mask, u32 goal )
{
	int ipoll = 0;
	u32 mbtemp;
    
	while( ( (mbtemp = getMbox( m, ibox ))&mask ) == 0 ){
		if ( (++ipoll&0x3ffff) == 0 ){
			fprintf( stderr, 
				 "pollMboxBits() mask:0x%08x goal:0x%08x got:0x%08x\n",
				 mask, goal, mbtemp       );
		}
	}
    
	PRINTF(4)( "pollMboxBits() returning 0x%08x\n", mbtemp );
	return mbtemp;
}


void setMboxPollcount(struct MU* m, int poll_count)
{
	m->poll_count = poll_count;
}
int getMboxPollcount(struct MU* m)
{
	return m->poll_count;
}


struct MU* mmapMbox( int iboard )   
// iboard {1..3}. return 0 on success
{
	char fname[80];

	void* region;
	struct MU* m = malloc( sizeof(struct MU) );
	unsigned offset;

	assert( m != 0 );    
	assert( IN_RANGE( iboard, 1, 8 ) );
    
	sprintf( fname, FNAME_FMT_MBOX, iboard );

	if ( (m->fd = open( fname, O_RDWR )) < 0 ){
		fprintf( stderr, "mmap: failed to open device \"%s\" - ", fname );
		perror( "" );
		exit( 1 );
	}
#ifndef USE_IOCTL
	region = mmap( NULL, 0x400, PROT_READ|PROT_WRITE, MAP_SHARED, m->fd, 0 );

	if ( region == (void*)-1 ){
		char errbuf[256];
		sprintf(errbuf, "%s %s", "mmap", fname);
		perror(errbuf);
		exit( 1 );
	}
    
	// IO not on page boundary - get the offset to adjust
    
	if ( ioctl( m->fd, ACQ32_IOG_PCIREGS_OFFSET, &offset ) != 0 ){
		perror( "ioctl ACQ32_IOG_PCIREGS_OFFSET" );
		exit( 1 );
	}

	PRINTF(1)("mmap %p offset %d\n", region, offset);
	PRINTF(2)("data: %08x %08x %08x %08x\n",
		  ((unsigned*)(region+offset))[0],
		  ((unsigned*)(region+offset))[1],
		  ((unsigned*)(region+offset))[2],
		  ((unsigned*)(region+offset))[3] );

	dbg(1, "mbox access mode %s\n", USE_IOCTL_DOC );

#ifndef __ACQ196__
	m->mailboxes = (r32*)((char*)region+offset+MAILBOX_0);
#else
	m->mailboxes = (r32*)((char*)region+offset);
	m->inbound_q_port = (r32*)((char*)region+offset+0x30);
	m->outbound_q_port = (r32*)((char*)region+offset+0x34);
#endif
	getMbox( m, 0 );
	getMbox( m, 1 );
	getMbox( m, 2 );
	getMbox( m, 3 );
#endif
	dbg( 1, "mbox access mode %s\n", USE_IOCTL_DOC );    
	return m;
}


/*
 * mmapDmaBuffer obtains a user space buffer
 * in virtual memory, this is mapped linearly
 * in physical memory, this is a series of 128K blocks
 *
 * if the allocated length < 128K, no problem, it's just linear memory
 *
 * clients should use getBusAddr to get the appropriate bus address for offset
 */
 
#define MAX_MAPPING 0x00100000        // 1MB is a handy # to play with
#define MAX_SEG     0x00020000        // Linux maps 128k segments

#define MAX_SEGMENTS    (MAX_MAPPING/MAX_SEG)
 
struct SegmentMap {
	int offset;
	int length;
	u32 physaddr;
	int pad;
};


struct DmaBuffer {
	u32* vaddr;
	int nbytes;
	struct SegmentMap seg_map[MAX_SEGMENTS];
};

int getDmaBufferLen(struct DmaBuffer* buffer)
{
	return buffer->nbytes;
}

struct DmaBuffer* mmapDmaBuffer( int iboard, unsigned nbytes )
// maps dma buffer, nbytes long. ret 0 on success
// iboard {1..4}
{
	char fname[80];
	int fd_in;
	caddr_t* region;
	u32* phys_defs;
	int idef;
	u32 offset;
	int rc;
	struct DmaBuffer* d = malloc( sizeof( struct DmaBuffer ) );
    
	assert( d != 0 );
    
	sprintf( fname, FNAME_FMT_DMABUF, iboard );
/*
 * (1)open() a path to the device
 */
	if ( (fd_in = open( fname, O_RDWR )) < 0 ){
		fprintf( stderr, "mmap: failed to open device \"%s\" - ", fname );
		perror( "" );
		exit( 1 );
	}

/*
 * (2) mmap() a buffer
 */
 
	region = mmap( NULL, nbytes, PROT_READ|PROT_WRITE, MAP_SHARED, fd_in, 0 );

	if ( region == (caddr_t*)-1 ){
		perror( "mmap" );
		exit( 1 );
	}

/*
 * (3) ioctl() - fetch the phys defs
 */
 
	PRINTF(3)( "region is %p\n", region );
     
	rc = ioctl( fd_in, ACQ32_IOREAD_GETPHYSICAL, region );

	if ( rc < 0 ){
		perror( "ioctl ACQ32_IOREAD_GETPHYSICAL failed" );
		exit( 1 );
	}
    
/*
 * now decode the physical mem defs in buffer.
 */
	phys_defs = (u32*)region;
     
	for ( idef = 0, offset = 0; idef != MAX_SEGMENTS; idef += 1 ){
		if ( phys_defs[idef*2] == 0 ){
			PRINTF(3)( "quitting after %d segments\n", idef );
			break;
		}else{
			PRINTF(3)( "adding segment %d at offset 0x%08x phys 0x%08x"
				   " len 0x%08x\n",
				   idef, offset, phys_defs[idef*2], phys_defs[idef*2+1] );
			d->seg_map[idef].offset = offset;
			d->seg_map[idef].physaddr = phys_defs[idef*2];
			d->seg_map[idef].length = phys_defs[idef*2+1];                   
			offset += phys_defs[idef*2+1];
		}
	}
    
	d->vaddr = (u32*)region;
	d->nbytes = nbytes;
	return d;
}

u32 getBusAddr( struct DmaBuffer* d, u32 offset ) 
// returns mapped bus address for offset (bytes) 
// warning: client is assumed not to go over the end of a segment
// BUT client has no way of knowng where the end is ...
{
	int idef;
    
	for ( idef = 0; idef != MAX_SEGMENTS; ++idef ){
		if ( offset >= d->seg_map[idef].offset && 
		     offset < d->seg_map[idef].offset + d->seg_map[idef].length ){
			return d->seg_map[idef].physaddr + offset - 
				d->seg_map[idef].offset;
		}
	}
    
	return 0;
}

u32* getVaddr( struct DmaBuffer* db, u32 offset )
{
	if ( offset < db->nbytes ){
		PRINTF(4)( "getVaddr %p %d %p\n", db->vaddr, offset, 
			   &db->vaddr[offset/sizeof(u32)] );
        
		return &db->vaddr[offset/sizeof(u32)];
	}else{
		return 0;
	}
}

void acq32_enableInts( struct MU* mbx, unsigned mask )
{
	if ( ioctl( mbx->fd, ACQ32_IOS_INTS_ENABLE, mask ) != 0 ){
		perror( "ioctl ACQ32_IOS_INTS_ENABLE" );
		exit( 1 );
	}
}
void acq32_maskInts( struct MU* mbx, unsigned mask )
{
	if ( ioctl( mbx->fd, ACQ32_IOS_INTS_DISABLE, mask ) != 0 ){
		perror( "ioctl ACQ32_IOS_INTS_DISABLE" );
		exit( 1 );
	}
}



/*
 * mapping from bigbuf
 * work to do - should get consts from /proc/.acq32.globs ... meanwhile ...
 */
//            acq32_big_buf_base:0x08000000
//            acq32_big_buf_len:0x08000000

#ifdef PGMCOMOUT
#define ACQ32_BIG_BUF_BASE 0x08000000    /* physaddr of buffer base */
#define ACQ32_BIG_BUF_LEN  0x08000000    /* buffer length in bytes  */
#else
#define ACQ32_BIG_BUF_BASE acq32_big_buf_base
#define ACQ32_BIG_BUF_LEN  acq32_big_buf_len

static int acq32_big_buf_base;
static int acq32_big_buf_len;

static void _getBigBufPrams() 
{
	FILE* fp = fopen( "/proc/.acq200.globs", "r" );
	char buf[80];

	assert( fp != 0 );

	while( fgets( buf, sizeof(buf), fp ) ){
		if (sscanf(buf, 
			   " acq32_big_buf_base:0x%x", &acq32_big_buf_base)){
			PRINTF(1)("acq32_big_buf_base set %x\n", 
				  acq32_big_buf_base);
		}
		if (sscanf(buf, 
			   " acq32_big_buf_len:0x%x", &acq32_big_buf_len)){
			PRINTF(1)("acq32_big_buf_len set %x\n", 
				  acq32_big_buf_len);
		}
	}
}

static void getBigBufPrams() 
{
	const char* base = getenv("acq32_big_buf_base");
	const char* len  = getenv("acq32_big_buf_len");

	if (len != 0 && base != 0){
		acq32_big_buf_base = strtoul(base, 0, 0);
		acq32_big_buf_len = strtoul(base, 0, 0);
	}else{
		_getBigBufPrams();
	}
}
#endif

/*
 * 2G driver - returns bigbuf portion details on read of .host device

[dt100@cp605 LOWLATENCY]$ cat /dev/acq32/acq32.7.host
BIGBUF: pa=0x18000000 len=0x01000000

 */
static void acq196_get_physical(int fd, struct DmaBuffer* d)
{
	char myline[80];
	FILE* fp = fdopen(fd, "r");

	if (fp == 0){
		perror("FAILED fdopen");
		exit(errno);
	}

	if (fgets(myline, sizeof(myline)-1, fp) &&
	    sscanf(myline, 
		   "BIGBUF: pa=0x%x len=0x%x",
		   &d->seg_map[0].physaddr,
		   &d->seg_map[0].length) == 2 ){
			   ;
	}else{
		perror("FAILED to read data from file.\n"
		       "Please check driver is up to date\n");
	}
	fclose(fp);
}

struct DmaBuffer* mmapBigBuffer( int iboard, unsigned nbytes )
// maps dma buffer, nbytes long. ret 0 on success
// iboard {1..4}
{
#define FN "mmapDmaBuffer() "
	char fname[80];
	int fd_in;
	caddr_t* region;
	struct DmaBuffer* d = malloc( sizeof( struct DmaBuffer ) );
    
	assert( d != 0 );

	getBigBufPrams();    

	sprintf( fname, FNAME_FMT_HOST, iboard );
/*
 * (1)open() a path to the device
 */
	if ( (fd_in = open( fname, O_RDWR )) < 0 ){
		fprintf(stderr, "mmap:failed to open device \"%s\" - ", fname);
		perror( "" );
		exit( 1 );
	}

/*
 * (2) mmap() a buffer
 */
#ifdef __ACQ196__
	nbytes = 0x01000000;
	PRINTF(1)(FN"ask for full bigbuf portion %x\n", nbytes);
#endif
	nbytes = MIN( nbytes, ACQ32_BIG_BUF_LEN );
 
	PRINTF(1)( FN"map at most %d bytes\n", nbytes );

	region = mmap( NULL, nbytes, PROT_READ|PROT_WRITE, MAP_SHARED, fd_in, 0 );

	if ( region == (caddr_t*)-1 ){
		perror( "mmap" );
		exit( 1 );
	}

#ifdef __ACQ196__
	acq196_get_physical(fd_in, d);
#else
/*
 * now decode the physical mem defs in buffer.
 */
	d->seg_map[0].physaddr = ACQ32_BIG_BUF_BASE;
	d->seg_map[0].length   = ACQ32_BIG_BUF_LEN;
#endif
	d->seg_map[0].offset = 0;
	d->vaddr = (u32*)region;
	d->nbytes = nbytes;

	PRINTF(1)( FN "physaddr 0x%08x vaddr 0x%08x\n", 
		   d->seg_map[0].physaddr, (u32)d->vaddr );

	return d;
#undef FN    
}


void mmapValidateDmaBuffer( struct MU* m, int nsamples )
{
	if ( ioctl( m->fd, ACQ32_IOS_VALIDATE_BIGBUF, nsamples ) != 0 ){
		perror( "ioctl ACQ32_IOS_VALIDATE_BIGBUF" );
		exit( 1 );
	}
}

#define INITBUF_MARKER 0x1100c0de   /* chances of 4 channels all having this val => 0*/



static void udelay(int usecs)
/* a wild guess at a microsecond delay. aim is to "rest" memory to let 
 * external DMA have accesss. We assume this code runs from cache ...
 */
{
	volatile int ii;

	while(usecs--){
		for(ii = 0x10; --ii;){
			;
		}
	}
}

void hbPrimeBuffer(struct DmaBuffer *buf)
/* one shot buffer priming pre run */
{
#ifdef __ACQ196__
	memset32(buf->vaddr, INITBUF_MARKER, 0x00f00000/sizeof(u32));
#else
#error WORKTODO - __ACQ196__ not defined
#endif
}
void hbPrimePoll(struct DmaBuffer *buf, int offset, int sample_len)
{
	int end32 = (offset + sample_len)/sizeof(u32);
	volatile u32* marker = &buf->vaddr[end32-2];

	marker[0] = INITBUF_MARKER;
	marker[1] = INITBUF_MARKER;
}
int hbPoll(
	struct DmaBuffer *buf, int offset, int sample_len,
	volatile int *user_abort)
{
	int end32 = (offset + sample_len)/sizeof(u32);
	volatile u32* marker = &buf->vaddr[end32-2];
	int ipoll = 1;
	int reported = 0;

	while (marker[0] == INITBUF_MARKER && marker[1] == INITBUF_MARKER){
		udelay(1);
		++ipoll;
		if (ipoll > 10000 && !reported){
			PRINTF(1)("hbPoll looking at %p\n", marker);
			reported = 1;
		}
		if (*user_abort){
			break;
		}
	}

	return ipoll;
}


MFA mu_reserveOutbound(struct MU* mu)
{
	return *mu->inbound_q_port;
}
int mu_putOutbound(struct MU* mu, MFA mfa)
/* return 0 on success */
{
	*mu->inbound_q_port = mfa;
	return 0;
}


MFA mu_getInbound(struct MU* mu)
{
	return *mu->outbound_q_port;
}
int mu_returnInbound(struct MU* mu, MFA mfa)
{
	*mu->outbound_q_port = mfa;
	return 0;
}

