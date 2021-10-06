/*
 * stubs for missing link opts in firmware port
 */
/* pgmwashere, ARMSIDE */

#define REVID "$Revision: 1.1.2.20 $"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <setjmp.h>

#include "local.h"

#include "acq200.h"
#include "ctype.h"

#include "dt100api.h"

#include "acq32busprot.h"

#include "acq32_drv.h"
#include "acq32_hml.h"
#include "acq32cpci_drv.h"

#include "dataHandling.h"

#include "dmac.h"
#include "mbox_slave.h"
#include "stream.h"

#include "dlog.h"

#include "acq200_hal.h"
//#include "hal.h"

#include "../cal-image/cal-image.h"

#include "REGDefs.h"

unsigned mu_base;

#define IOP321_REG_ADDR(reg) (mu_base|(reg))

#include "iop321.h"

unsigned csr_buffer[0x400];   // dummy memory for CSR 
unsigned scratch_pad_buf[SCRATCH_PAD_LEN/4];
char diags_buf[0x10000];

#include <fcntl.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

int kb_hit(void)
/* STUBBED: just generates a delay */
{
	fd_set watchset;
	struct timeval tv = { 0, 1000 };

	FD_ZERO(&watchset);
	select( 0, &watchset, 0, 0, &tv );
#ifdef PGMCOMOUT
	return FD_ISSET(0, &watchset);
#else
	return 0;
#endif
}
extern int fifo_sink(
    unsigned** ppdest,    // R0
    unsigned** ppfifo,    // R1
    int nsamples,         // R2
    unsigned mask         // R3
    )
{
	/* WORKTODO !! */
	return 0;
}


volatile unsigned* mbox( int imbox )
/*
 * IOP has two incoming, two outging mailboxes
 * nowrries, we treat the all as equivalent, having masked the outgoing
 * interrupt.
 */
{
    ARM_ASSERT( IN_RANGE( imbox, 0, 3 ) );

    switch( imbox ){
    case 0: return IOP321_IMR0;
    case 1: return IOP321_IMR1;
    case 2: return IOP321_OMR0;
    case 3: return IOP321_OMR1;
    default:
	    err( "imbox out of range %d\n", imbox );
	    return 0;
    }
}

int i2o_message_waiting(void)
{
	static int iter;
	u32 head = *IOP321_IPHPR;
	u32 tail = *IOP321_IPTPR;
#define IS_MSG (head != tail)
  

	if ( (iter++&0x1ffff) == 0 || IS_MSG){
		dbg(1, " H [0x%08x] T [0x%08x] %s",
		      head, tail, IS_MSG? "MESSAGE": "no message" );
	}

	return IS_MSG;
#undef IS_MSG
}

void acq32_assertPciInterrupt( unsigned short bits )
{
    PRINTF( "IRQ %x\n", bits );
    *IOP321_ODR |= bits;
}

void* memcpy64(void* dst, const void* src)
{
	return 0;
}
void memcpy4(unsigned* dst, unsigned* src, int nbytes)
{
	;
}
int fifo_sink_notests(
    unsigned** ppdest,    // R0
    unsigned** ppfifo,    // R1
    int nsamples          // R2
    )
{
	return -1;
}


static R16 the_regs[21];

R16* regs[21];


void init_regs(void)
{
	int ireg;

	for (ireg = 0; ireg != 21; ++ireg){
		regs[ireg] = &the_regs[ireg];
	}
}

#define MAPDEV      "/dev/acq200.pmmr"

void init_mbox_mapping(void)
{
	int fd = open( MAPDEV, O_RDWR );
	void* mapping;

	if ( fd < 0 ){
		perror( "mbox mapping failed, use dummy" );
		return;
	}
	mapping = mmap( 0, 0x1000, PROT_READ|PROT_WRITE,
				MAP_SHARED, fd, 0 );
	if ( mapping == 0 ){
		perror( "mapping failed, use dummy" );
		return;
	}
	
	mu_base = (unsigned)mapping;
}

void acq200_init(void)
{
	*IOP321_OIMR = 3; /* mask outbound message ints (used inbound) */
}

#undef MAPDEV
#define MAPDEV      "/dev/mu_hostbuffer"

char *host_win_mapping; /* _really_ want dma WORKTODO */
int host_win_maplen;

void init_host_window_mapping(int len)
{
	static int fd = -1;
	void* mapping;

	if (fd < 0){
		fd = open( MAPDEV, O_RDWR );
		if ( fd < 0 ){
			perror( "mbox mapping failed, use dummy" );
			return;
		}
	}else{
		munmap(host_win_mapping, host_win_maplen);
	}

	if (len == HW_DEFLEN){
		acq200_getRoot("/sys/module/acq200_mu/parameters",
			       "HBLEN", 1, "%d", &len);
	}
	mapping = mmap( 0, len, PROT_READ|PROT_WRITE,
				MAP_SHARED, fd, 0 );
	if (mapping == MAP_FAILED){
		perror( "mapping failed, use dummy" );
		return;
	}
	
	host_win_mapping = (char*)mapping;
	host_win_maplen = len;

	dbg(1, "mapping %p len %d", mapping, len);
}

short *BIG_BUF;
int BIG_BUF_LEN;
void* PCI_BUF;

/*
 * WORKTODO: get length from system
 * this code is appalling - rewrite it please
 */

#define BIGBUF_DEV "/dev/fifo_ro"
#define BIGBUF_LEN 0x08000000

void init_bigbuf_mapping(void)
{
	int fd = open(BIGBUF_DEV, O_RDWR);

	if (fd < 0){
		perror("init_bigbuf_mapping() failed");
		exit(1);
	}
	
	BIG_BUF = mmap( 0, BIGBUF_LEN, PROT_READ|PROT_WRITE,
				MAP_SHARED, fd, 0 );

	if (BIG_BUF == MAP_FAILED){
		perror("init_bigbuf_mapping() failed");
		printf("but it's not fatal, continue\n");
	}
	BIG_BUF_LEN = BIGBUF_LEN;
}



extern void configure_abort_code(void);


void init_stubs(void)
{
	info( "%s Built %s", REVID, __DATE__ " "__TIME__ );

	PCI_BUF = malloc(PCI_BUF_LEN);
	info("PCI_BUF %p\n", PCI_BUF);
	init_regs();
	init_mbox_mapping();
	/* pickup default - it may be correct from a previous run .. */
	init_host_window_mapping(HW_DEFLEN);
#if 0
	init_bigbuf_mapping();  /* mapping is not used anyhow */
#endif
	if (acq200_isAcq() || acq200_isWav232()){
		configure_abort_code();
	}
	acq200_init();
}


static void _writePci(unsigned* pci, unsigned local, int nw)
{
	unsigned *src = (unsigned *)local;

	while (nw--) {
		*pci++ = *src++;
	}
}

void dmacWritePci( unsigned pci_addr, unsigned local_addr, int nbytes )
{
	dbg(1, "PIO: pci_addr 0x%08x local 0x%08x nbytes %d",
	    pci_addr, local_addr, nbytes );

	_writePci((unsigned*)(pci_addr+host_win_mapping), 
		  local_addr, (nbytes+nbytes%4)/4);
}


const char* getBigbufLen( char client_buf[], int maxbuf )
{
	FILE *fp = fopen("/dev/dtacq_drv/bb_len", "r");
	char buf[80] = {};

/*
 * 'Bits:        	'
 *  12345678901234567890
 */
	if (!fp){
		sprintf(client_buf, "WARNING: buf len not available\n");
	}else{
		fread(buf, 1, 80, fp);
		sprintf(client_buf, "\n%s:\t%s\n", "Buffer Length", buf );
		fclose(fp);
	}
	return client_buf;
}

#define FPGA_HEADER "/tmp/fpga_header"
extern const char* getConfig( char client_buf[], int maxbuf )
{
	FILE *fp = fopen(FPGA_HEADER, "r" );
	int nread;
	

	if (!fp){
		nread = sprintf(client_buf, 
				"ERROR:failed to open %s\n",
				FPGA_HEADER);
	}else{
		nread = fread(client_buf, 1, maxbuf, fp);
		fclose(fp);

	}

	getBigbufLen(client_buf+nread, maxbuf - nread);
		
	return client_buf;
}
