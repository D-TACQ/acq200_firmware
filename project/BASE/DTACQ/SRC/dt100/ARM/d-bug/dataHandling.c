/*****************************************************************************
 *
 * File: dataHandling.c
 *
 * $RCSfile: dataHandling.c,v $
 * 
 * Copyright (C) 1999 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description: process to empty the fifo and sort the data via LUTs
 *
 * $Id: dataHandling.c,v 1.189.4.14 2007/01/03 23:16:16 pgm Exp $
 * $Log: dataHandling.c,v $
 * Revision 1.189.4.14  2007/01/03 23:16:16  pgm
 * double fork() is good
 *
 * Revision 1.189.4.13  2007/01/02 18:24:19  pgm
 * trim from 106K to 82K
 *
 * Revision 1.189.4.12  2007/01/02 18:18:10  pgm
 * *** empty log message ***
 *
 * Revision 1.189.4.11  2004/10/14 20:12:25  pgm
 * no warnings
 *
 * Revision 1.189.4.10  2004/08/29 11:48:18  pgm
 * improve number enabled channels handling
 * also ext clk
 *
 * Revision 1.189.4.9  2003/12/12 14:19:15  pgm
 * *** empty log message ***
 *
 * Revision 1.189.4.8  2003/11/05 22:29:57  pgm
 * hooks GATED_TRANSIENT, dio
 *
 * Revision 1.189.4.7  2003/10/30 14:12:36  pgm
 * B2017
 *
 * Revision 1.189.4.6  2003/10/27 20:38:36  pgm
 * reads status from driver
 *
 * Revision 1.189.4.5  2003/10/26 21:19:15  pgm
 * DUMDMA works 1MS
 *
 * Revision 1.189.4.4  2003/10/25 15:12:00  pgm
 * gutted
 *

 *
 *
\*****************************************************************************/

/*
 * data handling.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "local.h"
#include "ctype.h"

#include "dataHandling.h"
#include "mbox_slave.h"                 // mbox_abort()
#include "acq32_drv.h"
#include "acq32_hml.h"
#include "dt100api.h"

#include "dmac.h"
#include "stream.h"
#include "acq32busprot.h"               // tag def
#include "dlog.h"
#include "hal.h"
#include "diags.h"
#include "memmap.h"                     // for scratchpad - UGLY


 
static int S_NPAIRS;

#define MAXPAIRS 16
#define NPAIRS S_NPAIRS

#define FULL_MASK       0xffffffff
#define START_MASK      0x00000003
#define SHIFT_MASK(m)   ((m)<<=2)
#define MATCH_MASK(m,t) (((m)&(t))!=0)


int G_use_dma;
int G_simulate_dio;




extern int pgm_putchar( int cc );

/*
 * the DataDistributor is called AFTER the FIFO to memory Xfer has taken place
 * the DataDistributor has the opportunity to 
 * analyse or modify the data if required
 * If returning a non-zero TW, MUST update pointers itself
 */
typedef TriggerWord (*DataDistributor)(     
    struct DATA_ROUTE_TABLE* route,      // starting point for this sample  
    u32* current,                        // current put pointer
    TriggerWord trigger                  // current trigger condition
    );


// data in DATA_ROUTE_TABLE is indexed by PhysicalChannel

#define PHYSCHAN( ilogpair )  ((((ilogpair)&0x3)<<2)+(((ilogpair)&0xc)>>2))
#define LOGCHAN( iphyspair )  PHYSCHAN( iphyspair )




#define MAXTAGBUFFER 0x4000        /* must be power of 2 */


static __inline unsigned inctagix( unsigned tix )
{
    return (tix+1)&(MAXTAGBUFFER-1);
}

#define INCTAGIX( p )  inctagix( p )
  

extern const char* getCaptureStats( char client_buf[], int maxbuf )
{
#define FNAME "/proc/driver/acq200/stat_timing"
	FILE *fp = fopen(FNAME, "r" );

	if (!fp){
		sprintf(client_buf, FNAME "\n");
	}else{
		fread(client_buf, 1, maxbuf, fp);
		fclose(fp);
	}

	return client_buf;
#undef FNAME
}




static unsigned S_channel_mask;
static unsigned S_physchan_mask;


void acq32_setChannelMask( unsigned mask )
{
	mask = acq200_setChannelMask(mask);
	S_physchan_mask = S_channel_mask = mask;
}


unsigned acq32_getChannelMask( void )
{
    return S_channel_mask;
}

unsigned acq32_getPhyschanMask( void )
{    
    return S_physchan_mask;
}
void acq32_setChannelsInFifo( int nchan )
{
    S_NPAIRS = nchan/2;

    printf( "acq32_setChannelsInFifo( %d )\n", nchan );
}



void acq32_set_distributor_function( enum DFUNC dfunc )
{

}




int acq32_setInternalAbort( unsigned* _abort_flag )
{
	return 0;
}



void acq32_printFifoStats( void )
{
	
}


void buildStatusMessage( 
	int mtype, 
	const char* (*getData)( char client_buf[], int nbuf) 
	)
{
	Message my_message = {};

        my_message.header.type = mtype;
        my_message.header.id = 0;

        PRINTF( "buildStatusMessage() sizeof %d my_message.data %p\n", 
                MESSAGE_DATA_LEN, my_message.payload.cdata );
                                
        getData( my_message.payload.cdata, MESSAGE_DATA_LEN );

        my_message.header.length = 
		strlen(my_message.payload.cdata)+1+MESSAGE_HEADER_SIZE;                               
        
        PRINTF( "buildStatusMessage() getData() outputs \"%s\" length %d\n",
                my_message.payload.cdata, my_message.header.length  );

	i2o_postOutboundMessage(&my_message);
}
void acq32_getFwrev()
{
    buildStatusMessage( MT_FWREV, getBuildDate );
}


void acq32_getConfig()
{
    buildStatusMessage( MT_GETCONFIG, getConfig );
}

void acq32_getCaptureStats( u8 select ) 
{
    buildStatusMessage( MT_GETCAPTURESTATS, getCaptureStats );
}

extern char* calGetName();
extern char* calGetBoard();
extern char* calGetDate();
extern int calGetNumChannels();

const char* getCalInfo( char client_buf[], int maxbuf ) 
{
    sprintf( client_buf, 
             "CALNAME=%s\n"
             "CALBOARD=%s\n"
             "CALDATE=%s\n"
	         "CALNUMCHAN=%d\n",
             calGetName(), calGetBoard(), calGetDate(), calGetNumChannels() );
    
    ARM_ASSERT( strlen(client_buf) < maxbuf );
    
    return client_buf;
}

// kludge getRegs onto getCalInfo
// WORKTODO - develop a separate command

void acq32_getCalInfo()
{
    buildStatusMessage( MT_GETCALINFO, getCalInfo );
}

void acq32_setDataThreshold( int ichannel, short trigger_below, short trigger_above )
// ichannel is logical!
{

}
