/*****************************************************************************
 *
 * File: mbox_slave.h
 *
 * $RCSfile: mbox_slave.h,v $
 * 
 * Copyright (C) 2001 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description: interface for mbox_slave module
 *
 * $Id: mbox_slave.h,v 1.6.4.4 2007/01/05 12:56:49 pgm Exp $
 * $Log: mbox_slave.h,v $
 * Revision 1.6.4.4  2007/01/05 12:56:49  pgm
 * restore setDIO to old mbox, also abort
 *
 * Revision 1.6.4.3  2007/01/03 23:16:16  pgm
 * double fork() is good
 *
 * Revision 1.6.4.2  2007/01/03 12:31:38  pgm
 * MU interrupts rock! - .7s for 100 commands.
 *
 * Revision 1.6.4.1  2004/08/29 11:48:18  pgm
 * improve number enabled channels handling
 * also ext clk
 *
 * Revision 1.6  2001/09/08 13:54:36  pgm
 * Build 1333 - fixes E1 overrun, opt i2o incoming
 *
 * Revision 1.5  2001/03/25 19:20:28  pgm
 * first cut i2o command handling
 *
 * Revision 1.4  2001/03/20 21:51:05  pgm
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


#include "dt100api.h"


extern void mbox_reportState(enum STATE state);

enum MBOXI2OIN { MBOXI2OIN_POLL, MBOXI2OIN_INIT, MBOXI2OIN_ENABLE };

extern void mboxProcessI2OIncoming( int init );

#define MBOX_SLAVE_ONCE_BLOCK	1

void mboxSlaveOnce(int block);

enum STATE mbox_getState(void);

/** returns local version of state */
