/*****************************************************************************
 *
 * File: sinkfactory.cpp
 *
 * $RCSfile: sinkfactory.cpp,v $
 * 
 * Copyright (C) 2001 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: sinkfactory.cpp,v 1.2 2003/06/02 09:43:54 pgm Exp $
 * $Log: sinkfactory.cpp,v $
 * Revision 1.2  2003/06/02 09:43:54  pgm
 * SinkFactory takes responsibility for bad key
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
#include <unistd.h>

#include "sink.h"
#include "sinkfactory.h"

/*
 * ideally we would use an STL vector - but 
 */

#define MAXSB 10

static SinkBuilder* registered_sb_list[MAXSB];

SinkFactory::SinkFactory()
{

}

void SinkFactory::registerSink( SinkBuilder* sinkbuilder )
{
	for ( int ii = 0; ii != MAXSB; ++ii ){
		if ( registered_sb_list[ii] == sinkbuilder ){
			return; // no need to register again
		}else if ( registered_sb_list[ii] == 0 ){
			registered_sb_list[ii] = sinkbuilder;
			return;
		}
	}

	fprintf( stderr, "ERROR:MAXSB exceeded\n" );
	_exit( 1 );
}

void SinkFactory::print()
{
	for ( int ii = 0; ii != MAXSB; ++ii ){
		SinkBuilder* sb = registered_sb_list[ii];
		if ( sb ){
			printf( "%20s : %5s : %s\n",  
				sb->getName(), sb->getKey(), 
				sb->getDescription() );
		}else{
			break;
		}
	}
}
Sink* SinkFactory::instantiateSink( 
				   const char* key, 
				   int nchannels, 
				   unsigned channel_mask )
{
	if ( key == NULL ){
		fprintf( stderr, "SinkFactory:WARNING no key, "
			 "instantiating NullSink\n" );
		return new NullSink;
	}
	for ( int ii = 0; ii != MAXSB; ++ii ){
		SinkBuilder* sb = registered_sb_list[ii];

		if ( sb && 
		     (STREQ(key, sb->getKey()) || STREQ(key, sb->getName()) )){
			return sb->instantiate( nchannels, channel_mask );
		}
	}

	fprintf( stderr, "SinkFactory:WARNING key not found "
		 "instantiating NullSink\n" );
	return new NullSink;
}

SinkFactory& SinkFactory::inst()
{
	static SinkFactory _inst;

	return _inst;
}

