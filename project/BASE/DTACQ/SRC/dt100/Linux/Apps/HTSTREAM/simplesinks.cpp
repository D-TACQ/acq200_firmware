/*****************************************************************************
 *
 * File: simplesinks.cpp
 *
 * $RCSfile: simplesinks.cpp,v $
 * 
 * Copyright (C) 2001 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: simplesinks.cpp,v 1.7 2003/06/03 12:18:45 pgm Exp $
 * $Log: simplesinks.cpp,v $
 * Revision 1.7  2003/06/03 12:18:45  pgm
 * htstream2 - multi thread, interrupt driven, ship it quick
 *
 * Revision 1.6  2003/06/03 09:05:28  pgm
 * fix channel count bug
 *
 * Revision 1.5  2003/06/03 09:04:23  pgm
 * fix channel count bug
 *
 * Revision 1.4  2003/06/02 20:51:26  pgm
 * HTSTREAM V2 single thread good
 *
 * Revision 1.3  2003/05/09 07:37:54  pgm
 * compiles on RH8
 *
 * Revision 1.2  2002/04/14 07:02:55  pgm
 * fix active channel detect bug
 *
 * Revision 1.1  2002/04/12 08:52:00  pgm
 * decoupled factory arch runs
 *
 * Revision 1.2  2000/12/28 11:03:00  pgm
 * *** empty log message ***
 *
 * Revision 1.1  1999/10/22 16:26:49  pgm
 * first entry to cvs
 *
 *
\*****************************************************************************/

#include "local.h"

#include <stdio.h>
#include <math.h>
#include <assert.h>

#include "sink.h"
#include "sinkfactory.h"

extern int verbose;

#define PRINTFIELD( fmt, fld ) fprintf( stderr, "%20s : " fmt "\n", #fld, fld )
#define PRINTFIELD3( fmt, fld, value ) \
        fprintf( stderr, "%20s : " fmt "\n", #fld, value )

Sink::Sink() {
	memset( this, 0, sizeof(Sink) );
}

SinkImpl::SinkImpl( int _nchannels, unsigned _channel_mask  ) {
	nchannels = _nchannels;
	channel_mask = _channel_mask;

	active_channel = -1;

	for ( unsigned ichan = 0, cursor = 0x1; cursor; cursor <<= 1, ichan++){
		if ( (cursor&channel_mask) != 0 ){
			if ( active_channel == -1 ){
				active_channel = ichan;
			}
			++nactive;
		}
	}
}

void SinkImpl::print() {
	printf( "SinkImpl\n" );
	PRINTFIELD( "%d", nchannels );
	PRINTFIELD( "0x%08x", channel_mask );
	PRINTFIELD( "%d", active_channel );
}

class StatsSink : public SinkImpl {
	int nfills;
	long long ndata;
	int maxfill;

	long sum;
	long sumsq;
	short minval;
	short maxval;
	unsigned maxbuffer;

public:
	void init() {
		nfills = ndata = 0;
		sum = sumsq = 0;
		minval = 0x7fff;
		maxval = 0x8000;
		maxbuffer = 0;
		maxfill = 0;
	}
	void sink( unsigned pbuffer, unsigned nbuffer ) {
		++nfills;
		int nsamples = nbuffer/2/nchannels;
		ndata += nsamples;

		short* pdata = (short*)pbuffer+active_channel;

		if ( nsamples > maxfill ){
			maxfill = nsamples;
		}
		if ( nbuffer > maxbuffer ) maxbuffer = nbuffer;

		while( nsamples-- ){
			short the_data = *pdata;

			if ( verbose>1 ){
				printf( "%12d:0x%04x,", 
					the_data, the_data&0x0ffff );
			}
			sum += the_data;
			sumsq += the_data*the_data;
			pdata += nchannels;

			if ( the_data > maxval ){
				maxval = the_data;
			}else if ( the_data < minval ){
				minval = the_data;
			}
		}
	}

	double mean() { return (double)sum/ndata; }

	double variance() { 
		return ((double)sumsq - ndata*mean()*mean())/(ndata-1);
	}
	double stddev() { return sqrt( variance() ); }

	void final() {
		print();
		PRINTFIELD( "%d", nfills );
		PRINTFIELD( "%lld", ndata );
		PRINTFIELD( "%lld", ndata/nfills );
		PRINTFIELD( "%d", maxfill );

		PRINTFIELD( "%6.2f", mean() );
		PRINTFIELD( "%6.2f", stddev() );
		PRINTFIELD3( "0x%04x", maxval, maxval&0x0ffff );
		PRINTFIELD3( "0x%04x", minval, minval&0x0ffff );
		PRINTFIELD( "%d", maxbuffer );
	}
	StatsSink( int _nchannels, unsigned _channel_mask ) :
		SinkImpl( _nchannels, _channel_mask ) 
	{

	}
};

class HistoSink : public SinkImpl {
	static const unsigned NBITS;
	static const unsigned RSHIFT;
	static const char* FNAME;

	unsigned* histogram_buffer;
	unsigned* phisto;              // offset for signed data

public:
	void init() {}
	void sink( unsigned pbuffer, unsigned nbuffer ) {
		unsigned short* pdata = (unsigned short*)pbuffer;
		unsigned nsamples = nbuffer/nchannels/sizeof(short);
	
		while ( nsamples-- ){
			phisto[(*pdata^0x8000)>>RSHIFT]++;   
			// hmmm does >> sign extend?
			pdata += nchannels;
		}
	}
	void final() {
		FILE *fp = fopen( FNAME, "w" );

		assert( fp );

		fprintf( stderr, "writing to file %s\n", fp==0? "XX": "OK" );

		fwrite( histogram_buffer, sizeof(unsigned), 1<<NBITS, fp );
		fclose( fp );

		fprintf( stderr, "histogram written to %s\n", FNAME );
	}

	HistoSink( int _nchannels, unsigned _channel_mask ) : 
		SinkImpl( _nchannels, _channel_mask ) {
		histogram_buffer = new unsigned[1<<NBITS];

		memset( histogram_buffer, 0, (1<<NBITS)*sizeof(unsigned) );
		phisto = histogram_buffer;
	}
};
// pretend it's 12 bit to reduce how much we gotta look at
const unsigned HistoSink::NBITS = 12;
const unsigned HistoSink::RSHIFT = 4;
const char* HistoSink::FNAME = "/tmp/HistoSink.dat";

static ConcreteSinkBuilder<StatsSink>
_stats_sb_inst( "StatsSink", "S", "computes stats on data" );

static ConcreteSinkBuilder<HistoSink>
_hist_sb_inst( "HistoSink", "H", "computes histogram of data" );

