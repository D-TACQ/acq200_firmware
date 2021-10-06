/*****************************************************************************
 *
 * File:
 *
 * $RCSfile: acq32cpci_drv.c,v $
 * 
 * Copyright (C) 2001 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: acq32cpci_drv.c,v 1.9.4.1 2003/10/27 20:38:36 pgm Exp $
 * $Log: acq32cpci_drv.c,v $
 * Revision 1.9.4.1  2003/10/27 20:38:36  pgm
 * reads status from driver
 *
 * Revision 1.9  2002/03/21 11:17:09  pgm
 * untabify
 *
 * Revision 1.8  2002/03/13 21:43:09  pgm
 * correct formatting with emacs (Stroustrup)
 *
 * Revision 1.7  2001/10/07 10:19:59  pgm
 * GPME looking good build 1370 checkin
 *
 * Revision 1.6  2001/06/27 22:51:44  pgm
 * nodos
 *
 * Revision 1.5  2001/06/23 08:32:16  pgm
 * remove extra fixups, printf from acq32_run()
 *
 * Revision 1.4  2001/05/09 07:46:59  pgm
 * more adds to Wavegen, EmbTrig
 *
 * Revision 1.3  2001/05/01 20:59:22  pgm
 * first pass em trig runs - runs away in fact
 *
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

#include <stdio.h>

#include "local.h"

#include "REGDefs.h"
//#include "dt100api.h"
#include "acq32busprot.h"

#include "acq32cpci_drv.h"

#define FIRST_DEST     Q32C_SR_DEST_MIx 
#define LAST_DEST      Q32C_SR_DEST_PXI_CLK10

#define NREGS 5
static unsigned mirrors[NREGS]; // ARM workaround lack static init ...

int acq32cpci_setRoute( enum Q32C_SR_FUNC func, enum Q32C_SR_DEST dest_mask )
// set or clear route selects for <func> according to <dest_mask>
// returns 1 on success

{
#ifdef ACQ200COMOUT
    unsigned dest_bit = FIRST_DEST;
    int idest = 0;
    unsigned func_mask;

    // KLUDGE for wiring - CPCI matches PCI where DI1, DI2 swap
 
    switch( func ){
    case Q32C_SR_FUNC_DI1:
        func = Q32C_SR_FUNC_DI2; break;
    case Q32C_SR_FUNC_DI2:
        func = Q32C_SR_FUNC_DI1; break;
    default:
        break;
    }   
    
    func_mask = 1<<func;        
    
    for ( ; dest_bit <= LAST_DEST; ++idest, dest_bit <<= 1 ){
    
        ARM_ASSERT( idest < NREGS );
        
        if ( dest_mask&dest_bit ){
            mirrors[idest] |= func_mask;
        }else{
            mirrors[idest] &= ~func_mask;
        }
                
        PRINTF( "acq32cpci_setRoute() %p = 0x%02x\n", 
                &Q32C_ROUTESEL[idest], mirrors[idest] );
                                
        Q32C_ROUTESEL[idest] = mirrors[idest];
    }
#else
    dbg(1, "STUB");
#endif
    return 1;
}

void  acq32cpci_clearAllRoutes( void )
{
    int func;
    
    for ( func = Q32C_SR_FUNC_DI0; func != Q32C_SR_FUNC_DI5; ++func ){
        mirrors[func] = 0;  // not needed, but looks better in debugger
        acq32cpci_setRoute( (enum Q32C_SR_FUNC)func, Q32C_SR_DEST_NONE );
    }     
}
unsigned acq32cpci_getCurrentFuncState( void )
// returns current inputs state
{
#ifdef ACQ200COMOUT
    return *Q32C_ROUTEDEBUG;
#else
    dbg(1, "STUB");
    return 0xdeadbeef;
#endif
}






