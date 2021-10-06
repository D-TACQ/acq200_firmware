/*****************************************************************************
 *
 * File:
 *
 * $RCSfile: acq200_i2o.c,v $
 * 
 * Copyright (C) 2003 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: acq200_i2o.c,v 1.1.2.10 2007/04/10 17:28:36 pgm Exp $
 * $Log: acq200_i2o.c,v $
 * Revision 1.1.2.10  2007/04/10 17:28:36  pgm
 * B2180 allows restart on PCI
 *
 * Revision 1.1.2.9  2007/04/03 21:58:34  pgm
 * handles mbox interrupts
 *
 * Revision 1.1.2.8  2007/01/03 12:31:38  pgm
 * MU interrupts rock! - .7s for 100 commands.
 *
 * Revision 1.1.2.7  2007/01/02 18:24:18  pgm
 * trim from 106K to 82K
 *
 * Revision 1.1.2.6  2004/04/03 19:20:36  pgm
 * mu mapping moved for acq196
 *
 * Revision 1.1.2.5  2003/10/25 15:12:00  pgm
 * gutted
 *
 * Revision 1.1.2.4  2003/10/19 20:44:29  pgm
 * works with dt100rc
 *
 * Revision 1.1.2.3  2003/09/13 13:52:38  pgm
 * DMA IN, PIO OUT works well enough
 *
 * Revision 1.1.2.2  2003/09/07 16:30:39  pgm
 * basic i2o
 *
 * Revision 1.1.2.1  2003/09/04 21:08:42  pgm
 * getFwrev works (x156)
 *
 *
 *
\*****************************************************************************/


#include "local.h"
#include "stream.h"
#include "acq200_hal.h"

#include <assert.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define INF  "/dev/mu_inbound"
#define OUTF "/dev/mu_outbound"
#define RMAF "/dev/mu_rma"
#define MBOX "/dev/mu_mbox"


int acq200_debug=0;

static struct I2O_STATE {
	int fd_in;
	int fd_out;
	int fd_rma;
	int fd_mbox;
} i2o_state;

#define MLEN(mb) (mb->header.length)

static Message __msg_buf;	  /* pool of 1 WORKTODO */ 


static inline Message* getMessageBuf(void) 
{
	return &__msg_buf;
}
static inline void putMessageBuf(Message *message)
{

}


#define MAXMESSAGE sizeof(Message)

void i2o_initOutboundHostPull( int nfifo )
{
	dbg(1,  "%d", nfifo );
}
void i2o_initOutboundAcq32Push( 
	int nfifo, unsigned pci_addr, unsigned buf_len )
{
	dbg(1,  "%d %x %d",  nfifo, pci_addr, buf_len );
}

Message* i2o_getFreeOutboundMessage( void )
{
	return getMessageBuf();
}

void i2o_postOutboundMessage( Message* message )
{
	int rc;
	dbg(1,  "before write() to file %d len %d", 
	      i2o_state.fd_out, MLEN(message));
	rc = write( i2o_state.fd_out, message, MLEN(message) );
	if ( rc != MLEN(message) ){
		err( "write() returned %d\n", rc );
	}
	dbg(1,  "" );
	
}

void i2o_showOutboundStatus( void )
{
	dbg(1,  "" );
}
int i2o_numberOutboundMessagesPending( void )
{
	dbg(1,  "" );
	return 0;
}

void i2o_initInboundHostPush( int nfifo )
{
	dbg(1,  "" );
}

#define NO_MESSAGE     ((Message*)0xffffffff)

extern int i2o_message_waiting(void);

Message* i2o_getInboundMessage( void )
{
	Message *message = getMessageBuf();
	int rc;

/*
 * EAGAIN mechanism locks up ??? Fix later. Hack with mmaped IO
 */
	if ( !i2o_message_waiting() ){
		return NO_MESSAGE;
	}

	rc = read(i2o_state.fd_in, message, MAXMESSAGE);

	if (rc <= 0 && errno == EAGAIN){
		errno = 0;
		return NO_MESSAGE;
	}else if ( rc > 0 ){
		return message;
	}else if ( rc == 0 ){
		return NO_MESSAGE;
	}else{
		char buf[80];
		sprintf( buf, "%s failed %d (EAGAIN %d) rc %d", 
			 FN, errno, EAGAIN, rc );
		perror( buf );
		_exit(errno);
	}
	return NO_MESSAGE;
}


void i2o_returnInboundMessage( Message* message )
{
	dbg(1,  "" );
}

void i2o_showInboundStatus( void )
{

}

#define OPEN( fd, fn, flags ) \
        fd = open( fn, flags ); assert( fd > 0 );
 
void i2o_static_init( void )
{
#ifdef NOJOY_NOBLOCK
	OPEN( i2o_state.fd_in,  INF,  O_RDONLY|O_NONBLOCK );
#else
	OPEN( i2o_state.fd_in,  INF,  O_RDONLY);
#endif
	OPEN( i2o_state.fd_out, OUTF, O_WRONLY);
#if 0
	OPEN( i2o_state.fd_rma, RMAF, O_RDWR);
#endif
	OPEN(i2o_state.fd_mbox, MBOX, O_RDONLY);
}

void init_host_window_mapping(int len);


void i2o_initBuffers( u32 base, u32 len )
{
	dbg(1,  "%x %d", base, len );

	acq200_init_downstream_window(base, len);
}

int i2o_getFd(void) {
	return i2o_state.fd_in;
}

int i2o_getMbox(void) {
	return i2o_state.fd_mbox;
}
