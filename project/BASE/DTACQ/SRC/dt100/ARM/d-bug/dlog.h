/*****************************************************************************
 *
 * File:
 *
 * $RCSfile: dlog.h,v $
 * 
 * Copyright (C) 2001 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: dlog.h,v 1.3.4.2 2004/04/25 12:57:47 pgm Exp $
 * $Log: dlog.h,v $
 * Revision 1.3.4.2  2004/04/25 12:57:47  pgm
 * concurrent queries work
 *
 * Revision 1.3.4.1  2004/04/25 11:40:53  pgm
 * concurrency 1 with logging
 *
 * Revision 1.3  2002/03/21 11:54:51  pgm
 * untabify
 *
 * Revision 1.2  2001/03/20 21:51:04  pgm
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



// Dlog.h - interface to Dlog

#ifndef _DLOG_H_
#define _DLOG_H_

int dlogDump( void );
int dlogEnable( int enable );
int dlogPrintf( const char* format, ... );
int dlogIsEnabled( void );

#define DLOGPRINTF      if ( dlogIsEnabled() ) dlogPrintf

#endif
