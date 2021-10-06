/*****************************************************************************
 *
 * File: htprotocol.c
 *
 * $RCSfile: htprotocol.cpp,v $
 * 
 * Copyright (C) 2001 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: htprotocol.cpp,v 1.21 2003/06/22 12:42:38 pgm Exp $
 * $Log: htprotocol.cpp,v $
 * Revision 1.21  2003/06/22 12:42:38  pgm
 * local status works with correct data
 *
 * Revision 1.20  2003/06/22 09:45:35  pgm
 * status interrupt -l -l
 *
 * Revision 1.19  2003/06/03 12:18:45  pgm
 * htstream2 - multi thread, interrupt driven, ship it quick
 *
 * Revision 1.18  2003/06/02 21:21:55  pgm
 * terminates one shot OK
 *
 * Revision 1.17  2003/06/02 20:51:26  pgm
 * HTSTREAM V2 single thread good
 *
 * Revision 1.16  2003/06/02 09:44:36  pgm
 * new style
 *
 * Revision 1.15  2003/06/02 06:54:23  pgm
 * new format 8 col tab
 *
 * Revision 1.14  2003/05/31 15:41:54  pgm
 * fully local status
 *
 * Revision 1.13  2003/05/07 21:20:55  pgm
 * further refactor to ensure buffer validation for driver
 *
 * Revision 1.12  2003/05/07 20:56:20  pgm
 * fix big_buf ref, channels, doc
 *
 * Revision 1.11  2002/11/02 15:32:30  pgm
 * *** empty log message ***
 *
 * Revision 1.10  2002/08/12 11:56:03  pgm
 * streaming works to 1.3MHz, 41MB/sec
 *
 * Revision 1.9  2002/08/12 10:56:07  pgm
 * streaming works, display from bigdump buf
 *
 * Revision 1.8  2002/04/12 08:52:00  pgm
 * decoupled factory arch runs
 *
 * Revision 1.7  2002/04/11 16:30:20  pgm
 * recycle must work
 *
 * Revision 1.6  2002/04/11 13:14:50  pgm
 * recycle, histo in test
 *
 * Revision 1.5  2002/04/11 10:04:27  pgm
 * local poll and err det works
 *
 * Revision 1.4  2002/04/10 19:53:26  pgm
 * first runner
 *
 * Revision 1.3  2002/04/10 18:23:11  pgm
 * first runner
 *
 * Revision 1.2  2002/04/10 15:00:04  pgm
 * *** empty log message ***
 *
 * Revision 1.1  2002/04/10 14:11:41  pgm
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


#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>


#include "htif.h"
#include "htprotocol.h"
#include "acq32busprot.h"
#include "mbox.h"
#include "sink.h"


extern int verbose;

class HTProtocol {
public:
	int board;

private:
	Mailboxes mbx;
	u32 cmd;
	unsigned enter_cmd;

	int pollAck() {	// polls regular ack from acq32
		int ipoll = 0;
    
		while( (mbx.get( BP_MB_COMMAND )&MASK(BP_CI_ACK_BIT) ) == 0  ){
			if ( (++ipoll&0xfffff) == 0 ){
				fprintf(  stderr, 
				"pollAck %6d looking for 0x%08x got 0x%08x\n",
				ipoll, MASK(BP_CI_ACK_BIT), 
					  mbx.get(BP_MB_COMMAND) );
			}
		}
        
		PRINTF(2)( "pollAck() done 0x%08x\n", mbx.get(BP_MB_COMMAND) );
        
		return 0;
	}

	inline u32 pollSack() {	// return 0 on SACK, exit on SNACK
		u32 status = mbx.pollBits( BP_MB_HTM_CSR, 
					   HTM_CSR_SACK|HTM_CSR_SNACK, 
					   HTM_CSR_SACK ); 
    
		if ( status&HTM_CSR_SNACK ){
			fprintf( stderr, 
				 "ERROR:got a SNACK, bale out 0x%08x\n", 
				 status );
			_exit( 1 );
		}
		return status;
	}


	inline u32 setCmd( u32 cmd ) {
		PRINTF(2)( "llsetCmd( 0x%08x )\n", cmd );
    
		cmd &= ~( HTM_CSR_SACK|HTM_CSR_SNACK);
    
		mbx.set( BP_MB_HTM_CSR, cmd );
		return pollSack();
	}
	inline u32 getCsr()	{
		return mbx.get( BP_MB_HTM_CSR );
	}

protected:
	u32 status_paddr;
	unsigned last_status;


	void setStatusBuf() {
		enter_cmd |= BP_SET_A1( BP_FC_SETMODE_HTM_V2|
					BP_FC_SETMODE_HTM_STATUSBUF );
	}
public:
	void setBuflen( int length ) {
		cmd &= ~HTM_HOSTBUF_MB;
		cmd |= HTM_SET_BUFFLEN_BYTES( length );
	}
	void setRecycle( int enable ) {
		if ( enable ){
			cmd |= HTM_CSR_M_RECYCLE;
		}else{
			cmd &= ~HTM_CSR_M_RECYCLE;
		}
	}
	void setAddr( ContiguousDmaBuffer& cdb ) {
		mbx.set( BP_MB_HTM_DATA_ADDR, cdb.getPaddr() );

		printf( "HTProtocol::setAddr() status_paddr 0x%08x\n", 
			status_paddr );

		mbx.set( BP_MB_HTM_STATUS_ADDR, status_paddr );
		mbx.set( BP_MB_HTM_CURRENT_ADDR, 0 );

		setCmd( HTM_CSR_M_SETADDR|cmd );
	}

	inline void setArm() {
		setCmd(HTM_CSR_M_ARM|cmd); 
	}



	unsigned getLastStatus() const { return last_status; }

	virtual void getStatus() {
		setCmd( cmd );
		last_status = getCsr();
	}
	virtual u32 getCurrentAddr()	{
		unsigned current = mbx.get( BP_MB_HTM_CURRENT_ADDR );

		last_status = current&0x3f;
		return current & ~0x3f;
	}


	virtual int isOverrun() {
		return (last_status&HTM_CSR_S_OVERRUN) != 0;
	}
	virtual int isComplete() {
		return (last_status&HTM_CSR_S_COMPLETE) != 0;
	}




	void enterHT() {
		u32 command = 
			MASK(BP_CI_DONE_BIT)+
			MASK(BP_CI_COMMAND_BIT)+
			BP_SET_FUNCODE(BP_FC_SETMODE_HTM)+
			enter_cmd;

		mbx.set( BP_MB_COMMAND, command );
		pollAck();
		mbx.pollBits( BP_MB_COMMAND, LLC_CSR_READY, LLC_CSR_READY );
	}




	void setHoldoff( int holdoff ) {
		holdoff &= BP_FC_SETMODE_HTM_HOFF;
		enter_cmd |= BP_SET_A1( BP_FC_SETMODE_HTM_V2|holdoff);
		cmd |= HTM_CSR_M_INTERRUPT;
	}
	void leaveHT() {
		setCmd( HTM_CSR_M_ESC );
	}

	void setNumSamplesInBuffer( unsigned nsamples ) {
		mbx.validateBigBuffer( nsamples );
	}

	void setInterruptMask( unsigned mask ) {
		mbx.maskInts( mask );
	}
	void clrInterruptMask( unsigned mask ) {
		mbx.enableInts( mask );
	}

	virtual int atCorner() {
		return 1;        // BASIC HTProtocol knows no better
	}

	HTProtocol( int _board ) : mbx( _board ) { 
		board = _board; 
		cmd = 0; 
		enter_cmd = 0;
		status_paddr = 0; 
	}
	HTProtocol( HTProtocol* clone ) : mbx( clone->board ) {
		board = clone->board;
		cmd = clone->cmd;
		enter_cmd = clone->enter_cmd;
		status_paddr = clone->status_paddr;
	}
};

class HTProtocolLocalStatus : public HTProtocol {

	// pulls status from local memory - saves pci cycles BUT relies on
	// good cache coherency - so willit work ??
protected:
	volatile unsigned* p_loc_stat;
	int first_time;
	unsigned current;

public:
	HTProtocolLocalStatus( HTProtocol* clone, 
			       unsigned _status_paddr, 
			       unsigned status_vaddr   ) : 
		HTProtocol( clone )
	{
#define FN "HTProtocolLocalStatus() "
		first_time = 1;
		p_loc_stat = (unsigned*)status_vaddr;
		*p_loc_stat = 0xdeadbee0;
		status_paddr = _status_paddr;
		printf( FN"status_paddr 0x%08x\n",status_paddr); 
		printf( FN"status_vaddr %p\n",p_loc_stat); 
		getCurrentAddr();
		printf( "and if deadbee0, that was ok\n" );
#undef FN
	}
	virtual u32 getCurrentAddr() {

		if ( verbose && current != *p_loc_stat ){
			printf( "getCurrentAddr() 0x%08x => 0x%08x\n",
				current, *p_loc_stat );
		}
		current = *p_loc_stat;

		if ( current == 0xdeadbee0 ){
			if ( first_time ){
				printf( "deadbeef at %p\n", p_loc_stat );
				first_time = 0;
			}
			return 0;
		}
		return current;
	}
	virtual void getStatus() {}

#warning WORKTODO - hard coded consts
	virtual int isOverrun() {
		return (current&1) != 0;
	}
	virtual int isComplete() {
		return (current&2) != 0;
	}
};


/*
 * WORKTODO : transfers are in 1K chunks.
 * this is a nasty hard code dependency on protocol, remove it
 */

#define TLEN 1024

class HTProtocolLocalStatusBuffer : public HTProtocolLocalStatus 
/*
 * Status pointer increments. 
 * Keep the buffer cleared.
 * If next != 0 => new data available, clear current, increment
 * If previous != 0 => We have been overrun
 */
{
	static const char* const CN;
	unsigned bmask;

	volatile unsigned* p_current;
	volatile unsigned* overran_at;

	volatile unsigned* incr( volatile unsigned* p ) { 
		unsigned index = p - p_loc_stat;

		return p_loc_stat + (++index&bmask);
	}

	volatile unsigned* decr( volatile unsigned* p ) {
		unsigned index = p - p_loc_stat;

		return p_loc_stat + (--index&bmask);
	}
	
public:
	HTProtocolLocalStatusBuffer( 
				    HTProtocol* clone, 
				    unsigned _status_paddr, 
				    unsigned _status_vaddr,
				    int len ) :
		HTProtocolLocalStatus( clone, _status_paddr, _status_vaddr )
	{
		bmask = len - 1;
		memset( (void*)p_loc_stat, 0, len*sizeof(unsigned*) );
		setStatusBuf();
		no_buffer_count = 0;

		if ( verbose ){
			fprintf( stderr, 
				 "%s p_loc_stat %p phys 0x%08x mask 0x%08x\n", 
				 CN, p_loc_stat, _status_paddr, bmask );
		}
		p_current = p_loc_stat;
	}
	virtual u32 getCurrentAddr() {
		volatile unsigned* next;

		if ( *decr( p_current ) != 0 ){
			onOverrun();
		}
		next = incr( p_current );

		if ( *next != 0 ){
			*p_current = 0;
			p_current = next;
			current = *p_current;
		}else{
			if ( current && current != *p_current ){
				printNoBufferWarning();
			}
			current = *p_current;
		}

		return current;
	}

	virtual void onOverrun() {
		if ( decr( p_current ) != overran_at ){
			fprintf( stderr, 
				 "%s:ERROR: STATUS OVERRUN %p 0x%08x\n", CN,
				 overran_at = decr( p_current ), *overran_at);
		// WORKTODO consider more drastic action ...
		}
	}

	int no_buffer_count;

	void printNoBufferWarning() {
		if ( !(++no_buffer_count&0xfff) ){
			fprintf( stderr, "%s:WARNING:"
			 "STATUS BUFFER static, consider firmware upgrade\n", 
			 CN );
		}
	}

	virtual int atCorner() {
		return incr( p_current ) == p_loc_stat;
	}
};

const char* const 
HTProtocolLocalStatusBuffer::CN = "HTProtocolLocalStatusBuffer";


class HTimpl : public HTController {
	HTProtocol* htp;
	HTProtocol* htp_basic;

	Sink* the_sink;

	unsigned host_not_keeping_up;


	void considerLocalHTProtocol() {
/* HACK ALERT: status buffer follows data buffer in memory. 
 * So cdb.getLenth() must be >= datalength+statuslength
 */
 
		if ( run_length < cdb->getLength()-1024 ){

			unsigned status_physaddr = cdb->getPaddr()+run_length;
			unsigned status_vaddr    = cdb->getVaddr()+run_length;

			printf ( "cdb available %d requested %d\n",
				 cdb->getLength(), run_length );
			htp = new HTProtocolLocalStatus( 
				htp_basic, status_physaddr, status_vaddr );

			PRINTF(1) ( 
				 "WARNING: board %d using local status\n",
				 getBoard());

		}else{
			htp = htp_basic;
		}
	}

	void considerLocalBufferHTProtocol() {
		int status_buffer_len = run_length/TLEN;

		if ( run_length < cdb->getLength()-status_buffer_len ){

			unsigned status_physaddr = cdb->getPaddr()+run_length;
			unsigned status_vaddr    = cdb->getVaddr()+run_length;

			printf ( "cdb available %d requested %d\n",
				 cdb->getLength(), run_length );
			htp = new HTProtocolLocalStatusBuffer( 
				htp_basic, status_physaddr, status_vaddr,
				status_buffer_len );

			PRINTF(1)(  
				 "WARNING: board %d using local status "
				 "BUFFER 0x%08x\n",
				 getBoard(), status_physaddr );

		}else{
			htp = htp_basic;
		}
	}
	int psleep() { usleep( 1000 ); return 1; }

	unsigned old_current;
	int npoll;
	int quit;

public:
	int  processAvailableData() {
		unsigned new_current;
		int nprocessed = 0;
		int newbytes = 0;
		unsigned current_va = 0;

		for ( nprocessed = 0;
		      (new_current = htp->getCurrentAddr()) != old_current;
		      ++nprocessed, newbytes += 1024 ){

			old_current = new_current;

			if ( nprocessed == 0 ){
				current_va = cdb->getVaddr() + 
					(new_current-cdb->getPaddr());
			}else if ( newbytes > 1024 ){
				++host_not_keeping_up;
			}

			htp->getStatus();

			if ( htp->isOverrun() ){
				fprintf( stderr, 
					 "ERROR: overrun at 0x%08x\n", 
					 new_current );
				// return?? throw more likely
				quit = 1;
				break;
			}
			if ( htp->isComplete() ){
				fprintf( stderr, "COMPLETE\n" );
				// return?? throw more likely
				quit = 1;
				break;
			}

			if ( htp->atCorner() ){
				break;
			}
		}

		the_sink->sink( current_va, newbytes );

		return nprocessed;
	}

	void setHoldoff( int holdoff ) {
		htp->setHoldoff( holdoff );
	}
private:
	void doRun() {

		quit = 0;

		do {
			for ( int npoll = 1;
			      processAvailableData() == 0 && !quit;
			      psleep(), ++ npoll ){

				if ( (npoll&0xffff) == 0 ){
					htp->getStatus();
					if ( htp->isComplete() || 
					     htp->isOverrun() ){
						break;
					}
					fprintf( stderr, 
						 "poll 0x%08x 0x%08x\n",
						 htp->getLastStatus(), 
						 htp->getCurrentAddr() );
				}
			}

			if ( htp->isOverrun() ){
			       break;
			}
	       } while( !htp->isComplete() );
	}

	void final() {
		if ( host_not_keeping_up ){
			fprintf( stderr, "WARNING:host_not_keeping_up %u\n",
				 host_not_keeping_up );
		}
	}


public:
	void start() {
		host_not_keeping_up = 0;
		old_current = 0;

		PRINTF(1)( "HTController::start() board %d "
			   "local_status_requested %d\n",
			   getBoard(), local_status_requested );

		switch( local_status_requested ) {
		case 1:
			considerLocalHTProtocol();	
			break;
		case 2:
			considerLocalBufferHTProtocol();
			break;
		case 0:
			break;
		default:
			assert( 0 );
		}

		htp->enterHT();    
		htp->setBuflen( run_length );
		htp->setRecycle( recycle );
		htp->setAddr( *cdb );

		htp->setArm();
	}
	void finish() {
		htp->leaveHT(); 
		final();
		the_sink->final();
		setNumSamplesInBuffer( run_length/bytes_per_sample );	    
	}

	void runToCompletion() {
		start();
		doRun();
		finish();
	}

	void run( unsigned _length, 
		  ContiguousDmaBuffer& _cdb,
		  int _bytes_per_sample ) {
		setRunLength( _length );
		setBuffer( &_cdb );
		setBytesPerSample( _bytes_per_sample );
		runToCompletion();
	}
	void registerSink( Sink* sink ) { the_sink = sink; }

	void abort() { 
		finish();
	}

	void setNumSamplesInBuffer( unsigned nsamples ) {
		htp->setNumSamplesInBuffer( nsamples );
	}
	virtual void setInterruptMask( unsigned mask ) {
		htp->setInterruptMask( mask );
	}
	virtual void clrInterruptMask( unsigned mask ) {
		htp->clrInterruptMask( mask );
	}
	HTimpl( int _board, HTProtocol* _htp ) :
		HTController( _board )
	{
		htp_basic = htp = _htp;
		local_status_requested = 0;
		recycle = 0;
	}

	unsigned getCurrentAddr() {
		return htp->getCurrentAddr();
	}
};


HTController& 
HTController::getHTController( int _board )
{
	return * new HTimpl( _board, new HTProtocol( _board ) );
}
