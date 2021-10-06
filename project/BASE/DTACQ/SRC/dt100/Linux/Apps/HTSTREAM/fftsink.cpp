/*****************************************************************************
 *
 * File: fftsink.cpp
 *
 * $RCSfile: fftsink.cpp,v $
 * 
 * Copyright (C) 2001 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: fftsink.cpp,v 1.6 2002/08/29 18:50:54 pgm Exp $
 * $Log: fftsink.cpp,v $
 * Revision 1.6  2002/08/29 18:50:54  pgm
 * remove FFTW
 *
 * Revision 1.5  2002/04/14 10:58:32  pgm
 * this seems to work (don't use heap after fftw :-))
 *
 * Revision 1.4  2002/04/14 10:22:03  pgm
 * *** empty log message ***
 *
 * Revision 1.3  2002/04/12 10:41:57  pgm
 * *** empty log message ***
 *
 * Revision 1.2  2002/04/12 10:35:47  pgm
 * added fftw code. segfaults :-(
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
#include <assert.h>
#include <unistd.h>

#include "sink.h"
#include "sinkfactory.h"

#ifdef FFTW
#include "rfftw.h"
#else
#include <stdlib.h>
#endif

class FftSink : public SinkImpl {
    // hardcoded for 1024 points, two channels, data in low byte
#define NFFT 1024
#define NFFTB (NFFT*sizeof(unsigned))
    unsigned pprevious;
#ifdef FFTW
    fftw_real* in;
    fftw_real* out;
    // float* window[];
    fftw_plan plan;
#else
    float* in;
    float* out;
#endif    
    int nresults;
    int *results;
    int nsinks;

    void cookData( unsigned buffer_addr ) {
	long* plongs = (long*)buffer_addr;

	for ( int ii = 0; ii != NFFT; ++ii ){
	    in[ii] = plongs[ii]>>16;
	    // in[ii] *= window[ii];
	}
    }

    void dofft() {
#ifdef FFTW
	rfftw( plan, 1, in, 1, 1, out, 1, 1 );
#endif
    }
    void allocateBuffers() {
#ifdef FFTW
	in  = (fftw_real*) fftw_malloc( NFFT * sizeof(fftw_real) );
	out = (fftw_real*) fftw_malloc( NFFT * sizeof(fftw_real) );
#else
	in = (float*) malloc( NFFT * sizeof(float) );
	out =(float*) malloc( NFFT * sizeof(float) );
#endif
    }
    void destroyBuffers() {
#ifdef FFTW
	fftw_free( in );
	fftw_free( out );
#else
	free( in );
	free( out );
#endif
    }
    void initFft() {
#ifdef FFTW
	plan = rfftw_create_plan_specific( 
					  NFFT, 
					  FFTW_FORWARD, 
					  FFTW_FLAGS,
					  in, 1,
					  out, 1 );
	if ( plan == NULL ){
	    fprintf( stderr, "ERROR: cant create plan\n" );
	    _exit( 1 );
	}else{
	    fprintf( stderr, "FftSink::init() done\n" );
	}
#else

#endif
    }
    void destroyFft() {
#ifdef FFTW
	rfftw_destroy_plan( plan );
#endif
    }
protected:
    virtual void processFftOut() {
	if ( nsinks < nresults ){
	    // find largest bin, avoiding DC and NYQUIST
	    float maxval = 0.0f;
	    int maxbin = 0;

	    for ( int ibin = 1; ibin != NFFT-2; ++ibin ){
		if ( out[ibin] > maxval ) {
		    maxval = out[ibin];
		    maxbin = ibin;
		}
		results[nsinks-1] = maxbin;
	    }
	    
	}
    }

public:
#define FFTW_FLAGS FFTW_MEASURE

    void init( int totlen ) {
	nresults = totlen/sizeof(unsigned)/2/NFFT;
	results = new int[nresults];
	memset( results, 0, nresults*sizeof(int) );
	init();
    }

    void init() {
	fprintf( stderr, "FftSink::init()\n" );

	nsinks = 0;
	initFft();
    }
    void sink( unsigned pbuffer, unsigned nbuffer ) {
	if ( pprevious == 0 ) pprevious = pbuffer;
	if ( pprevious > pbuffer ) pprevious = pbuffer;

	if ( (pbuffer-pprevious) + nbuffer >= NFFTB ){
	    ++nsinks;
	    cookData( pprevious );
	    pprevious = pbuffer+NFFTB; // for next time
	    dofft();
	    processFftOut();
	}
    }
    void final() {
	fprintf( stderr, "FftSink::final() nsinks %d \n", nsinks );

	FILE *fp = fopen( "/tmp/FftSinkPeaks", "w" );
	assert( fp );

	fwrite( results, sizeof(int), nresults, fp );
	fclose( fp );

	fprintf( stderr, "results written to /tmp/FftSinkPeaks\n" );
	destroyFft();
    }

    FftSink( int _nchannels, unsigned _channel_mask ) :
	SinkImpl( _nchannels, _channel_mask ) 
    {

	fprintf( stderr, "FftSink::FftSink()n results 0x100000\n" );
	nresults = 0x100000;
	results = new int[nresults];
	memset( results, 0, nresults*sizeof(int) );

	pprevious = 0;
	allocateBuffers();

	if ( in != 0 && out != 0 ){
	    fprintf( stderr, "FftSink() OK\n" );
	}else{
	    perror( "FftSink fftw_malloc() failed" );
	    _exit( 1 );
	}
    }
    ~FftSink() {
	fprintf( stderr, "~FftSink()\n" );
	destroyBuffers();
    }
};



static ConcreteSinkBuilder<FftSink> 
_fft_inst( "FftSink", "F", "computes fft of incoming data" );
