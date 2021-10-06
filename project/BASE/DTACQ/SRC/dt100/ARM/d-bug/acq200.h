/*****************************************************************************
 *
 * File: acq200.h
 *
 * $RCSfile: acq200.h,v $
 * 
 * Copyright (C) 2003 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: acq200.h,v 1.1.2.7 2009/09/02 20:31:46 pgm Exp $
 * $Log: acq200.h,v $
 * Revision 1.1.2.7  2009/09/02 20:31:46  pgm
 * B2217 supports ACQ164
 *
 * Revision 1.1.2.6  2008/03/25 15:51:59  pgm
 * acq132
 *
 * Revision 1.1.2.5  2007/04/13 17:13:20  pgm
 * B2184 hopefully fix hostwin init - both for usual [init] case and for reruns
 *
 * Revision 1.1.2.4  2007/01/03 23:16:16  pgm
 * double fork() is good
 *
 * Revision 1.1.2.3  2007/01/02 17:46:16  pgm
 * usc call acqcmd works @ 25 cps
 *
 * Revision 1.1.2.2  2004/04/25 11:40:52  pgm
 * concurrency 1 with logging
 *
 * Revision 1.1.2.1  2004/02/10 07:35:57  pgm
 * *** empty log message ***
 *
 * Revision 1.1.2.11  2003/12/12 14:19:15  pgm
 */


#ifndef __ACQ200_H__
#define __ACQ200_H__

#define BT_NONE    0
#define BT_ACQ     0x80000000
#define BT_ACQ216  1
#define BT_WAV232  2
#define BT_ACQ196  4
#define BT_ACQ232  8
#define BT_ACQ132  0x10
#define BT_ACQ164  0x20



#define DEBUG_SOCK   "/tmp/acq200control_debug"
#define COMMAND_SOCK "/tmp/acq200control_command"


#ifdef __cplusplus
extern "C" {
#endif
	void acq200_setBoardType(int bt);       
#ifdef __cplusplus
};


#endif

int acq200_getRoot(
	char * ROOT, char *fname, int nval, char *fmt, ... );

#endif /* __ACQ200_H__ */
