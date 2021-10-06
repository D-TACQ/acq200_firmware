/*****************************************************************************
 *
 * File: htprotocol.h
 *
 * $RCSfile: htprotocol.h,v $
 * 
 * Copyright (C) 2001 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: htprotocol.h,v 1.16 2003/06/22 09:45:35 pgm Exp $
 * $Log: htprotocol.h,v $
 * Revision 1.16  2003/06/22 09:45:35  pgm
 * status interrupt -l -l
 *
 * Revision 1.15  2003/06/03 12:18:45  pgm
 * htstream2 - multi thread, interrupt driven, ship it quick
 *
 * Revision 1.14  2003/06/02 20:51:26  pgm
 * HTSTREAM V2 single thread good
 *
 * Revision 1.13  2003/06/02 09:44:36  pgm
 * new style
 *
 * Revision 1.12  2003/05/07 21:20:55  pgm
 * further refactor to ensure buffer validation for driver
 *
 * Revision 1.11  2002/08/12 11:56:03  pgm
 * streaming works to 1.3MHz, 41MB/sec
 *
 * Revision 1.10  2002/08/12 10:56:07  pgm
 * streaming works, display from bigdump buf
 *
 * Revision 1.9  2002/04/12 08:52:00  pgm
 * decoupled factory arch runs
 *
 * Revision 1.8  2002/04/11 16:30:20  pgm
 * recycle must work
 *
 * Revision 1.7  2002/04/11 16:05:01  pgm
 * sensible histogram, fixes seg fault
 *
 * Revision 1.6  2002/04/11 13:14:50  pgm
 * recycle, histo in test
 *
 * Revision 1.5  2002/04/11 10:04:27  pgm
 * local poll and err det works
 *
 * Revision 1.4  2002/04/10 18:23:11  pgm
 * first runner
 *
 * Revision 1.3  2002/04/10 15:00:04  pgm
 * *** empty log message ***
 *
 * Revision 1.2  2002/04/10 14:11:29  pgm
 * *** empty log message ***
 *
 * Revision 1.1  2002/04/10 12:36:47  pgm
 * *** empty log message ***
 *
 * Revision 1.1  2002/04/02 10:41:19  pgm
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


class Sink;
class ContiguousDmaBuffer; // defined elsewhere

class HTController {
protected:
	unsigned run_length;
	ContiguousDmaBuffer* cdb;
	int local_status_requested;
	int recycle;
	int bytes_per_sample;  
	const int board;

	HTController( int _board ) :
		board( _board ) 
		{}
public:
	int getBoard() { return board; }
	void setRunLength( unsigned _run_length ) {
		run_length = _run_length;
	}
	void setBuffer( ContiguousDmaBuffer* _cdb ) {
		cdb = _cdb;
	}
	void setBytesPerSample( int _bytes_per_sample ) {
		bytes_per_sample = _bytes_per_sample;
	}
	void requestLocalStatus( int _local_status ) { 
		local_status_requested = _local_status; 

	}
	void setRecycle( int enable ) {
		recycle = enable;
	}
	virtual void setHoldoff( int holdoff ) {}
	// set holdoff in usecs before interrupt (can be zero)

	virtual void start() = 0;
	virtual int processAvailableData() = 0;
	// return #data sets processed
	virtual void finish() = 0;
	
	virtual void runToCompletion() = 0;

	virtual void registerSink( Sink* sink ) = 0;

	virtual void abort() = 0;

	virtual void setNumSamplesInBuffer( unsigned nsamples ) = 0;
	virtual void setInterruptMask( unsigned mask ) = 0;
	virtual void clrInterruptMask( unsigned mask ) = 0;

	virtual unsigned getCurrentAddr() = 0;
// deprecated:
	virtual void run( unsigned length, ContiguousDmaBuffer& cdb,
 		      int bytes_per_sample ) = 0;
// factory
	static HTController& getHTController( int _board );
};
