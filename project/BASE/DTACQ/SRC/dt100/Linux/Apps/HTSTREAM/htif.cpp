/*****************************************************************************
 *
 * File: htif.c
 *
 * $RCSfile: htif.cpp,v $
 * 
 * Copyright (C) 2001 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: htif.cpp,v 1.10 2003/06/22 12:42:38 pgm Exp $
 * $Log: htif.cpp,v $
 * Revision 1.10  2003/06/22 12:42:38  pgm
 * local status works with correct data
 *
 * Revision 1.9  2003/06/02 09:43:18  pgm
 * computes nboards from driver
 *
 * Revision 1.8  2003/05/07 20:56:20  pgm
 * fix big_buf ref, channels, doc
 *
 * Revision 1.7  2002/11/02 15:32:30  pgm
 * *** empty log message ***
 *
 * Revision 1.6  2002/08/12 10:56:07  pgm
 * streaming works, display from bigdump buf
 *
 * Revision 1.5  2002/04/10 19:53:26  pgm
 * first runner
 *
 * Revision 1.4  2002/04/10 18:23:11  pgm
 * first runner
 *
 * Revision 1.3  2002/04/10 14:11:29  pgm
 * *** empty log message ***
 *
 * Revision 1.2  2002/04/10 13:14:32  pgm
 * compiles
 *
 * Revision 1.1  2002/04/10 12:37:46  pgm
 * *** empty log message ***
 *
 * Revision 1.1  2002/04/10 12:36:47  pgm
 * *** empty log message ***
 *
 * Revision 1.1  2002/04/02 10:25:14  pgm
 * *** empty log message ***
 *
 *
 * Revision 1.1  1999/10/22 16:26:49  pgm
 * first entry to cvs
 *
 *
\*****************************************************************************/
#include "local.h"

#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

#include "htif.h"

#include "mbox.h"  // PRINTF()
/*
 * WARNING: these prams MUST match kernel boot command AND
 * acq32_big_buf driver pram settings
 *
 * for this test, we assume a 512MB system, with a MEM=128M statement
 * we leave the second 128M free (7.3 likes a gap)
 */

/* shameful dirty cut and paste from llcontrol */

static int acq32_big_buf_base;
static int acq32_big_buf_len;
static int acq32_big_buf_nboards;

static void getBigBufPrams()
{
	FILE* fp = fopen( "/proc/.acq32.globs", "r" );
	char buf[128];

	assert( fp != 0 );

	while( fgets( buf, sizeof(buf), fp ) ){
		if ( sscanf( buf, 
			     " acq32_big_buf_base:0x%x", 
			     &acq32_big_buf_base )){
			printf( "acq32_big_buf_base set %x\n", 
				acq32_big_buf_base );
		}
		if ( sscanf( buf, 
			     " acq32_big_buf_len:0x%x", 
			     &acq32_big_buf_len ) ){
			printf( "acq32_big_buf_len set %x\n", 
				acq32_big_buf_len );
		}
	}

	fclose( fp );

	fp = fopen( "/proc/acq32", "r" );
	assert( fp != 0 );

	while( fgets( buf, sizeof(buf), fp ) ){
		if ( strncmp( buf, "ACQ", 3 ) == 0 ){
			acq32_big_buf_nboards++;
		}
	}

	fclose( fp );
}


/*
 * in this implementation, we divide up the single big buf ourselves
 * ideally the driver would do this for us.
 * to implement: make fd, region non static, remove one time checks on 
 * fd, region instantiation
 */
#define SEG_LEN (acq32_big_buf_len/acq32_big_buf_nboards)   

class ContiguousDmaBufferImpl: public ContiguousDmaBuffer {

	static /* !!**?? */ int fd;
	int board;
	static /* !!**?? */ void *region;

	const char* devName( int board ) const {
		static char buf[80];

		sprintf( buf, "/dev/acq32/acq32.%d.host", 1 /*board*/ );

		return buf;
	}

public:
	ContiguousDmaBufferImpl( int _board ) {
		board = _board;

		if ( fd == 0 /* !!**?? */ ){
			PRINTF(1)( "ONE SHOT big buffer mapping\n" );
			getBigBufPrams();
			fd = open( devName( _board ), O_RDWR );
			if ( fd <= 0 ){
				char errb[80];
				sprintf( errb, "failed to open device %s\n", 
					 devName( _board ) );
				perror( errb );
				_exit(1);
			}
		}
	}

	int makeMapping( unsigned _length ) {
		if ( region == NULL /* !!**?? */ ){
			region = mmap( NULL, acq32_big_buf_len, 
				     PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0 );
		}
		if ( region == (void*)-1 ){
			return -1;
		}else{
			paddr = acq32_big_buf_base+(board-1)*SEG_LEN; 

			vaddr = (unsigned)region + (board-1)*SEG_LEN;
			length = SEG_LEN;
			return 0;
		}
	}
	~ContiguousDmaBufferImpl() {
		close( fd );
	}
};

int ContiguousDmaBufferImpl::fd;
void* ContiguousDmaBufferImpl::region;

ContiguousDmaBuffer&
ContiguousDmaBuffer::getBuffer( int board, unsigned length )
{
	ContiguousDmaBufferImpl* pbuffer = new ContiguousDmaBufferImpl( board );

	if ( pbuffer->makeMapping( length ) == 0 ){
		return *pbuffer;
	}else{
		perror( "ERROR: makeMapping failed" );
		_exit( 1 );
		return *pbuffer;// not reached
	}
}
