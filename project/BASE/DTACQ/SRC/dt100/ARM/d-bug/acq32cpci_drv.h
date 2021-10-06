/*****************************************************************************
 *
 * File: acq32cpci_drv.h - API to interface specific funcs
 *
 * $RCSfile: acq32cpci_drv.h,v $
 * 
 * Copyright (C) 2001 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: acq32cpci_drv.h,v 1.2 2001/04/14 20:37:50 pgm Exp $
 * $Log: acq32cpci_drv.h,v $
 * Revision 1.2  2001/04/14 20:37:50  pgm
 * CPCI routing coded, needs testing
 *
 * Revision 1.1  2001/04/14 19:05:40  pgm
 * stubs
 *
 * Revision 1.2  2000/12/28 11:03:00  pgm
 * *** empty log message ***
 *
 * Revision 1.1  1999/10/22 16:26:49  pgm
 * first entry to cvs
 *
 *
\*****************************************************************************/


#ifndef __ACQ32CPCI_DRV_H__
#define __ACQ32CPCI_DRV_H__


int acq32cpci_setRoute( enum Q32C_SR_FUNC func, enum Q32C_SR_DEST dest_mask );
// returns 1 on success

void  acq32cpci_clearAllRoutes( void );

unsigned acq32cpci_getCurrentFuncState( void );
// returns current inputs state











#endif // __ACQ32CPCI_DRV_H__


