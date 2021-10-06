/*****************************************************************************
 *
 * File:
 *
 * $RCSfile: stream.h,v $
 * 
 * Copyright (C) 2001 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: stream.h,v 1.10.4.1 2003/09/01 08:17:37 pgm Exp $
 * $Log: stream.h,v $
 * Revision 1.10.4.1  2003/09/01 08:17:37  pgm
 * i2o_init_buffes() hook
 *
 * Revision 1.10  2001/09/30 14:08:20  pgm
 * i2o boot time - two commands
 *
 * Revision 1.9  2001/03/31 20:45:13  pgm
 * DUMDMA works with debugs on ...
 *
 * Revision 1.8  2001/03/25 10:24:45  pgm
 * incoming i2o works with cache disabled. outgoing trahsed :-(
 *
 * Revision 1.7  2001/03/24 20:47:17  pgm
 * rejig I2O, add incoming hooks (outgoing still works)
 *
 * Revision 1.6  2001/03/20 21:51:05  pgm
 * added headers
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
 * stream.h ... defs for streaming data
 */

#include "acq32busprot.h"

// API

void i2o_initOutboundHostPull( int nfifo );

Message* i2o_getFreeOutboundMessage( void );
void i2o_postOutboundMessage( Message* message );

void i2o_showOutboundStatus( void );
int i2o_numberOutboundMessagesPending( void );

void i2o_initInboundHostPush( int nfifo );

#define NO_MESSAGE     ((Message*)0xffffffff)

Message* i2o_getInboundMessage( void );
void i2o_returnInboundMessage( Message* message );

void i2o_showInboundStatus( void );

void i2o_static_init( void );


/*
 * new for acq200 ...
 */
void i2o_initBuffers( u32 base, u32 len );
