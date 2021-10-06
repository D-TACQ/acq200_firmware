/*****************************************************************************
 *
 * File: sinkfactory.h
 *
 * $RCSfile: sinkfactory.h,v $
 * 
 * Copyright (C) 2001 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: sinkfactory.h,v 1.2 2002/04/12 08:52:00 pgm Exp $
 * $Log: sinkfactory.h,v $
 * Revision 1.2  2002/04/12 08:52:00  pgm
 * decoupled factory arch runs
 *
 * Revision 1.1  2002/04/12 07:14:26  pgm
 * *** empty log message ***
 *
 * Revision 1.2  2000/12/28 11:03:00  pgm
 * *** empty log message ***
 *
 * Revision 1.1  1999/10/22 16:26:49  pgm
 * first entry to cvs
 *
 *
\*****************************************************************************/

/*
 * prospective Sinks register themselves with SinkFactory using a SinkBuilder
 *
 * clients get their Sinks by calling instatiateSink() with key
 */

class Sink;                 // we don't need to know

class SinkBuilder {
 public:
  virtual const char* getName() = 0; // name can also build
  virtual const char* getKey() = 0; // the key that builds the sink
  virtual const char* getDescription() = 0;
  virtual Sink* instantiate( int nchannels, unsigned channel_mask ) = 0;
};




class SinkFactory {
// singleton, factory pattern - makes sinks

    SinkFactory();
 public:
    void registerSink( SinkBuilder* sinkbuilder );

    void print(); // prints key/description info WORKTODO - use iterator
    
    Sink* instantiateSink( 
	const char* key, int nchannels, unsigned channel_mask );

    static SinkFactory& inst();  // returns the singleton item
};

template<class S> 
class ConcreteSinkBuilder : public SinkBuilder {
    const char* const N;
    const char* const K;
    const char* const D;

 public:
  const char* getName() { return N; }
  const char* getKey() { return K; }
  const char* getDescription() { return D; }
  Sink* instantiate( int nchannels, unsigned channel_mask ){
    return new S( nchannels, channel_mask );
  }
  ConcreteSinkBuilder( const char* _N, const char* _K, const char* _D ) :
	N( _N ), K( _K ), D( _D )
  {
      SinkFactory::inst().registerSink( this );
  }
};
