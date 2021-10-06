/*****************************************************************************
 *
 * File:
 *
 * $RCSfile: llif26.c,v $
 * 
 * Copyright (C) 2001 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description: implmentation of device driver interface for LLC
 *
 * $Id: llif26.c,v 1.4 2010/08/26 16:25:52 pgm Exp $
 * $Log: llif26.c,v $
 * Revision 1.4  2010/08/26 16:25:52  pgm
 * more switchable instrumentation
 *
 * Revision 1.3  2007/05/01 19:22:04  pgm
 * linux26 device index from 0
 *
 * Revision 1.2  2007/02/19 15:22:23  pgm
 * *** empty log message ***
 *
 * Revision 1.1  2006/06/19 13:13:30  pgm
 * *** empty log message ***
 *
 * Revision 1.18.4.17  2006/02/22 10:25:47  pgm
 * fix dbg snafu
 *
 *
 *
\*****************************************************************************/

/** @file llif26.c  implementation of device driver interface 
 * for LLC for kernel .2.6x.
 */



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

#define FMT_SYS_PARAMS	"/sys/module/acq200_hostdrv/parameters/"
#define FMT_SYS_SLOTS		FMT_SYS_PARAMS "slots"

#define FMT_SYS_DEV "/sys/class/acqX00/acq200.%d/device/"
#define FMT_SYS_DEV_HOST_PA	FMT_SYS_DEV "host_pa"
#define FMT_SYS_DEV_HOST_LEN	FMT_SYS_DEV "host_len"


static int lookup_device_index(int slot)
{
	static int mapping[16];
	static int valid;
	int rc;
	
	if (!valid){
		char slotdef[128];		
		int ii;
		FILE *fp;		
		
		fp = fopen(FMT_SYS_SLOTS, "r");
		if (!fp){
			fprintf(stderr, "Failed to open slot mapping:\"%s\"", 
				FMT_SYS_SLOTS);	
			exit(errno);
		}
				
		for (ii = 0; ii < 16; ++ii){
			mapping[ii] = -1;	
		}
		if (fgets(slotdef, 128, fp)){
			char *cp;
			for (cp = slotdef, ii = 0; 
				ii < 16 && (cp = strtok(cp, ",")) != 0; 
				cp = 0, ++ii){
					
				int slot_num = atoi(cp);
				assert(IN_RANGE(slot_num, 0, 15));
				if (slot_num != 0){
					mapping[slot_num] = ii; 	
				}	
			}
		}				
		valid = 1;	
	}
	
	rc = mapping[slot];
	if (rc == -1){
		fprintf(stderr, "No device found for slot %d", slot);
		exit(-1);	
	}
	return rc;
}
static unsigned host_len(int iboard) {
	char fname[80];
	char line[80];
	char *my_line;
	unsigned len = 0;
	int rc;
	
	sprintf(fname, FMT_SYS_DEV_HOST_LEN, lookup_device_index(iboard));
	FILE *fp = fopen(fname, "r");
	assert(fp);

	my_line = fgets(line, sizeof(line), fp);
	assert(my_line);
	
	rc = sscanf(my_line, "%u", &len);
	assert(rc);
	return len;
}
static unsigned host_pa(int iboard) {
	char fname[80];
	char line[80];
	char *my_line;
	unsigned pa = 0;
	int rc;

	sprintf(fname, FMT_SYS_DEV_HOST_PA, lookup_device_index(iboard));
	FILE *fp = fopen(fname, "r");
	assert(fp);

	my_line = fgets(line, sizeof(line), fp);
	assert(my_line);
	
	rc = sscanf(my_line, "0x%x", &pa);
	assert(rc);
	return pa;
}

int setMbox( struct MU* m, int ibox, u32 value )      
// set a mail mbox register: ibox {0..3}. return 0 on success
{
	assert( IN_RANGE( ibox, 0, 3 ) );

    
	PRINTF(3)( "setMbox( %d, 0x%08x )\n", ibox, value );
	m->mailboxes[ibox] = 
		m->mailboxes_shadow[ibox] = 
		m->mailboxes_lastwrite[ibox] = value;    

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
    
	assert( IN_RANGE( ibox, 0, 3 ) );

	newval = m->mailboxes[ibox];
    
	if ( newval != m->mailboxes_shadow[ibox] ){
		PRINTF(4)( "getMbox( %d ) was:0x%08x now:0x%08x\n", 
			   ibox, m->mailboxes_shadow[ibox],
			   newval);
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

	region = mmap( NULL, 0x400, PROT_READ|PROT_WRITE, MAP_SHARED, m->fd, 0 );

	if ( region == (void*)-1 ){
		char errbuf[256];
		sprintf(errbuf, "%s %s", "mmap", fname);
		perror(errbuf);
		exit( 1 );
	}
    
#if 0
	// IO not on page boundary - get the offset to adjust
    
	if ( ioctl( m->fd, ACQ32_IOG_PCIREGS_OFFSET, &offset ) != 0 ){
		perror( "ioctl ACQ32_IOG_PCIREGS_OFFSET" );
		exit( 1 );
	}
#else
	offset = 16;
#endif

	PRINTF(1)("mmap %p offset %d\n", region, offset);
	PRINTF(2)("data: %08x %08x %08x %08x\n",
		  ((unsigned*)(region+offset))[0],
		  ((unsigned*)(region+offset))[1],
		  ((unsigned*)(region+offset))[2],
		  ((unsigned*)(region+offset))[3] );

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
    
	PRINTF(1)("mmapDmaBuffer 01\n");

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



	sprintf(fname, FNAME_FMT_HOST, iboard );

	PRINTF(1)(FN"01: open %s\n", fname);
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
	nbytes = host_len(iboard);
	PRINTF(1)(FN"ask for full bigbuf portion %x\n", nbytes);
#endif
 
	PRINTF(1)( FN"map at most %d bytes\n", nbytes );

	region = mmap( NULL, nbytes, PROT_READ|PROT_WRITE, MAP_SHARED, fd_in, 0 );

	if ( region == (caddr_t*)-1 ){
		perror( "mmap" );
		exit( 1 );
	}

	/* fill in a single segment .. 
	 * @todo remove over complex segmented design hang over
	 */
	d->seg_map[0].physaddr = host_pa(iboard);
	d->seg_map[0].offset = 0;
	d->seg_map[0].length = nbytes;
	d->vaddr = (u32*)region;
	d->nbytes = nbytes;

	PRINTF(1)( FN "physaddr 0x%08x vaddr 0x%08x\n", 
		   d->seg_map[0].physaddr, (u32)d->vaddr );

	return d;
#undef FN    
}


void mmapValidateDmaBuffer( struct MU* m, int nsamples )
{

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
	int len = buf->nbytes - 0x100000;

	assert(len > 0);

	memset32(buf->vaddr, INITBUF_MARKER, len/sizeof(u32));
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

