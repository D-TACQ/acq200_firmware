/*****************************************************************************
 *
 * File: dataHandling.h
 *
 * $RCSfile: dataHandling.h,v $
 * 
 * Copyright (C) 1999 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: dataHandling.h,v 1.80.4.2 2004/02/15 21:39:20 pgm Exp $
 * $Log: dataHandling.h,v $
 * Revision 1.80.4.2  2004/02/15 21:39:20  pgm
 * local slave command handling
 *
 * Revision 1.80.4.1  2003/10/25 15:12:01  pgm
 * gutted
 *
 * Revision 1.80  2003/02/07 09:50:51  pgm
 * B1713 BoxCar
 *
 * Revision 1.79  2002/11/01 20:49:15  pgm
 * env, invert, acq16 offset stub, dmac tests
 *
 * Revision 1.78  2002/08/23 18:27:30  pgm
 * B1691 - runners, err status
 *
 * Revision 1.77  2002/08/21 19:49:20  pgm
 * B1689 - auto trig, sample set adjust OK??
 *
 * Revision 1.76  2002/08/21 09:02:39  pgm
 * B1680 dhAcq12run as fast as it will get
 *
 * Revision 1.75  2002/08/06 19:41:38  pgm
 * lower ICACHE load, add mbox boost
 *
 * Revision 1.74  2002/07/25 17:31:48  pgm
 * B1687 - pipeline back to 2
 *
 * Revision 1.73  2002/07/25 15:13:12  pgm
 * acq16 trig comp now automatic
 *
 * Revision 1.72  2002/07/24 18:07:41  pgm
 * acq16 trigger offset comp - use 1st TW
 *
 * Revision 1.71  2002/07/22 20:01:34  pgm
 * fixes gap in pre gpem P1
 *
 * Revision 1.70  2002/06/10 20:02:40  pgm
 * B1666 multi samples per pci packet
 *
 * Revision 1.69  2002/03/21 15:07:11  pgm
 * merge from 1608 - multi trig safe, edge det etc
 *
 * Revision 1.68  2002/03/21 11:54:51  pgm
 * untabify
 *
 * Revision 1.67  2002/03/13 21:43:09  pgm
 * correct formatting with emacs (Stroustrup)
 *
 * Revision 1.66  2002/03/08 19:32:14  pgm
 * B1591 - default no mask - its fast
 *
 * Revision 1.65  2002/02/16 07:40:49  pgm
 * attempting to make edge trig faster
 *
 * Revision 1.64  2002/02/14 19:28:15  pgm
 * acq16 multitrig
 *
 * Revision 1.63  2002/02/09 21:32:12  pgm
 * work on acq16 2/4/8/16 (boots)
 *
 * Revision 1.62  2002/02/09 09:31:05  pgm
 * war on warnings
 *
 * Revision 1.61  2002/02/08 22:41:24  pgm
 * multi num channel configs for acq16
 *
 * Revision 1.60  2002/02/02 15:13:41  pgm
 * usec timing changes from EPFL visit 01/2002
 *
 * Revision 1.59  2002/01/20 20:54:00  pgm
 * threshtrig in debug
 *
 * Revision 1.58  2002/01/19 18:26:09  pgm
 * fast th almost
 *
 * Revision 1.57  2002/01/18 18:33:10  pgm
 * *** empty log message ***
 *
 * Revision 1.56  2002/01/18 10:05:41  pgm
 * onwards and upwards
 *
 * Revision 1.55  2002/01/13 22:21:58  pgm
 * acq32 fast threshtrig kludge coded
 *
 * Revision 1.54  2002/01/07 14:14:35  pgm
 * Build 1452 - works with P1==0, disID works with GPEM
 *
 * Revision 1.53  2002/01/05 21:48:26  pgm
 * fixes DUMDMA stride probs
 *
 * Revision 1.51  2001/12/05 20:59:32  pgm
 * sink 3 x 4 is the biz
 *
 * Revision 1.50  2001/12/05 19:51:02  pgm
 * works, but not at 2.5MSPS
 *
 * Revision 1.49  2001/12/04 21:28:53  pgm
 * full rate threshtrig in switchable sink
 *
 * Revision 1.48  2001/11/23 22:28:07  pgm
 * fixes AO ST, direct dumdma speed demo
 *
 * Revision 1.47  2001/10/22 20:00:17  pgm
 * Build 1400 edge trig works rel to Scalar
 *
 * Revision 1.46  2001/10/12 20:52:22  pgm
 * edge trigger done
 *
 * Revision 1.45  2001/10/06 21:03:07  pgm
 * CPEM with PXI trig works, AI overflow to cope with
 *
 * Revision 1.44  2001/09/24 20:25:21  pgm
 * soft OK, hard not yet
 *
 * Revision 1.43  2001/09/24 12:54:31  pgm
 * GPEM AO works
 *
 * Revision 1.42  2001/09/02 21:30:19  pgm
 * multi thresh trig, vranging
 *
 * Revision 1.41  2001/08/18 20:00:52  pgm
 * updates route correctly, we get data
 *
 * Revision 1.40  2001/06/28 21:11:28  pgm
 * fix phys channe mixup on acq16 flavour fetch, also trheshtrig experiments
 *
 * Revision 1.39  2001/06/27 22:49:42  pgm
 * better status handling, acq16 deeper fifo
 *
 * Revision 1.38  2001/06/23 19:03:36  pgm
 * acq16 part 1 captures runs
 *
 * Revision 1.37  2001/06/09 20:28:59  pgm
 * GUT - in testing
 *
 * Revision 1.36  2001/06/05 19:45:12  pgm
 * meshed gut, datahandling - willit work
 *
 * Revision 1.35  2001/06/04 19:34:49  pgm
 * GUT added (well it compiles anyway)
 *
 * Revision 1.34  2001/05/07 11:38:37  pgm
 * DAC output - wrk in progress
 *
 * Revision 1.33  2001/05/06 18:46:04  pgm
 * embedded trigger reporting in
 *
 * Revision 1.32  2001/05/04 20:37:08  pgm
 * big upgrade to improve qwual, match spec
 *
 * Revision 1.31  2001/05/04 09:32:41  pgm
 * consistent pollcat now
 *
 * Revision 1.30  2001/05/01 20:59:23  pgm
 * first pass em trig runs - runs away in fact
 *
 * Revision 1.29  2001/04/29 12:29:18  pgm
 * embedded trigger incl, lets see if it works
 *
 * Revision 1.28  2001/04/19 19:55:25  pgm
 * mbox cal ops, physchan mask
 *
 * Revision 1.27  2001/03/26 20:25:07  pgm
 * fetch data compiled not tested
 *
 * Revision 1.26  2001/03/20 21:51:04  pgm
 * added headers
 *
 * Revision 1.25  2001/03/18 17:38:44  pgm
 * builds with max 2 warnings
 *
 * Revision 1.24  2001/03/04 21:09:29  pgm
 * LL kludge, Data Trigger 99kHz
 *
 * Revision 1.23  2001/02/15 21:16:06  pgm
 * *** empty log message ***
 *
 * Revision 1.22  2000/10/29 20:43:33  pgm
 * fix bigdump end buffer problem
 *
 * Revision 1.21  2000/10/14 22:35:25  pgm
 * first pass speed up
 *
 * Revision 1.20  2000/10/01 08:16:10  pgm
 * improved GATED_CONTINUOUS, flash prog
 *
 * Revision 1.19  2000/09/18 13:04:39  pgm
 * from uist
 *
 * Revision 1.18  2000/08/14 19:11:29  pgm
 * nodos
 *
 * Revision 1.17  2000/08/14 19:10:53  pgm
 * bulked messages works (partly)
 *
 * Revision 1.16  2000/06/25 06:16:37  pgm
 * hacked in streaming to dataHandling et al
 *
 * Revision 1.15  2000/06/11 13:18:20  pgm
 * queries in ST_RUN allowed, also Abort works
 *
 * Revision 1.14  2000/06/05 21:33:19  pgm
 * SC1 driver stubs filled
 *
 * Revision 1.13  2000/06/04 19:52:40  pgm
 * STUB SC handling compiles
 *
 * Revision 1.12  2000/03/25 19:15:45  pgm
 * interrupt handling, acq16 tests
 *
 * Revision 1.11  1999/11/19 20:07:01  pgm
 * distributor func, internal samplerate 200k
 *
 * Revision 1.10  1999/11/16 10:52:41  pgm
 * GATED_CONTINUOUS runs
 *
 * Revision 1.9  1999/11/14 18:47:20  pgm
 * update
 *
 * Revision 1.7  1999/11/12 18:11:26  pgm
 * default mbox init, LEDS
 *
 * Revision 1.2  1999/10/30 21:00:37  pgm
 * pgm backed up
 *
 *
\*****************************************************************************/

#ifndef __DATAHANDLING_H__
#define __DATAHANDLING_H__


#include "memmap.h"

extern int G_event_count;

extern void acq32_setRowLut( unsigned* base, unsigned nlongs );

enum ROUTE { ROUTE_ONESHOT, ROUTE_CONTINUOUS, ROUTE_COUNT };
struct DATA_ROUTE_TABLE* getRoute( enum ROUTE route );            // may not need to be public??

enum SETTIMER { ST_START, ST_END };

enum RUN_CONTROL { 
    RC_CONTINUE = 0x0, 
    RC_RESTART  = 0x1, 
    RC_QUIT     = 0x2, 
    RC_MASK     = 0x3 
};

#define RC_QUIT_SET( rc )       (((rc)&RC_QUIT)!=0)
#define RC_RESTART_SET( rc )    (((rc)&RC_RESTART)!=0)


#define RC_SET_NACCEPT( naccept )       ((naccept)<<16)
#define RC_GET_NACCEPT( rc )            ((rc)>>16)

typedef u32 TriggerWord;

struct RC_Word {
    int rc_naccept:16;
    int rc_request:16;
};

typedef struct RC_Word (*RunCallBack)( 
    int nsamples, 
    int isample, 
    int nfifo, 
    TriggerWord trigger_word 
    );

typedef int (*Runner)( 
    int nsamples, 
    struct DATA_ROUTE_TABLE* route,
    RunCallBack call_back    
    );

#define TW_GET_SAMPLES( tw )         ((tw)&0x00ff)
#define TW_SET_SAMPLES( tw, nsam )   (((tw)&0xffff0000)|(nsam))

#define TW_HAS_FLAGS( tw ) (((tw)&0xffffff00)!=0)
#define TW_FLAGS( tw )     ((tw)>>16)   // the raw flags from LCA

#define TW_GET_PROCESS( tw ) ((tw)&0xff00)

#define TW_GET_LOCAL( tw ) ((tw)&0x0800)
#define TW_SET_LOCAL( tw ) ((tw)|0x0800)
#define TW_CLR_LOCAL( tw ) ((tw)&~0x0800)

#define TW_AI_PROCESS_DONE    0x0100
#define TW_AO_PROCESS_DONE    0x0200
#define TW_DO_PROCESS_DONE    0x0400

#define TW_APP_MASK       0x0f00

#define TW_TRIG_MASK    (0x3f<<6)

#define TW_GET_DIO_TRIG( tw ) ((TW_FLAGS(tw)>>6)&0x3f)
#define TW_GET_DIO_INST( tw ) ((TW_FLAGS(tw)&0x03f)^G_InvertedInputs)

#define TW_GET_ACQ16UPSTREAM_COUNT(tw)    (TW_FLAGS(tw)>>8)
#define TW_ACQ16_PIPELINE_OFFSET   2        // pipeline delay in adc

// new lca 20020320
#define TW_GET_ACQ16DIO_TRIG(tw)          ((TW_FLAGS(tw)>>4)&0x0f)
#define TW_GET_ACQ16DIO_INST(tw)          (TW_FLAGS(tw)&0x0f)  

extern int fifo_sink(
    unsigned** ppdest,    // R0
    unsigned** ppfifo,    // R1
    int nsamples,         // R2
    unsigned mask         // R3
    );

static __inline int tw_fifo_sink(
    unsigned** ppdest,    // R0
    unsigned** ppfifo,    // R1
    int nsamples,         // R2
    unsigned mask         // R3
    )
{
    return fifo_sink( ppdest, ppfifo, nsamples, mask )& ~TW_APP_MASK;
}

typedef u32 FifoStatusWord;

// returns # samples to accept in d0..15

enum DFUNC{ 
    DF_NORMAL, DF_TEST, DF_NULL, DF_FAST, DF_ID, DF_ID_ALL, DF_DACSIM, 
    DF_DATA_TRIGGER,
    DF_DATA_TRIGGER_MULTI,
    DF_DATA_EDGE_TRIGGER,
    DF_DATA_EDGE_TRIGGER_MULTI,
    DF_DATA_SINK_DETECTOR_MULTI
};

void acq32_set_distributor_function( enum DFUNC dfunc );


/*
 * capture nsamples from fifo_data, wrapping at fifo_data_max
 * fifo_data == 0, fifo_data_max==0 implies use the real thing
 */

// select ROUTE_CONTINUOUS with this mask

#define ROUTE_CONTINUOUS_SELECT 0x80000000U


void acq32_RestartLut( enum ROUTE route );  // rearm lut

int acq32_copy_pcibuf_pair( int ipair, int start, int length, int stride );

int acq32_copy_pcibuf_channel( int ichan, int istart, int length, int stride );

int acq32_copy_pcibuf_row( int start, int length );

void acq32_setTagging( unsigned  on );
//start in samples, length in samples
// return #samples copied

int acq32_setStreaming( int stride, unsigned flags );
// enable streaming (stride==0) => disable

void setMaxMessageLength( int length );

extern int acq32_setInternalAbort( unsigned* code );
#define ACQ32_CLEAR_INTERNAL_ABORT (unsigned*)0
#define ACQ32_SET_INTERNAL_ABORT   (unsigned*)1

void acq32_setDataThreshold( int ichannel, short trigger_below, short trigger_above );
// ichannel is logical!


// these values inhibit Data Trigger
#define DT_BELOW_NEVER -32768
#define DT_ABOVE_NEVER  32767

void acq32_inhibitDataTresholdAll( void );

void acq32_getFwrev( void );
void acq32_getCaptureStats( u8 select );

struct MESSAGE;

void acq32ProcessDataRequestPacket( 
    struct MESSAGE* request_list 
    );
void acq32ProcessXDataRequestPacket( 
    struct MESSAGE* request_list 
    );

void setChannelMask( unsigned mask );
int getActivePairCount( unsigned chan_mask );
unsigned acq32_getChannelMask( void );
unsigned acq32_getPhyschanMask( void );

u32 acq32_getFifoErrorMask( void );

extern unsigned G_last_trigger_line[];

struct DATA_ROUTE_TABLE;

u32* getRouteCurrent( struct DATA_ROUTE_TABLE* route );
void wrapRoute( struct DATA_ROUTE_TABLE* route );

int kb_abort( void );

TriggerWord distribute32(
    struct DATA_ROUTE_TABLE* route,
    u32* current,
    TriggerWord trigger,
    int tag_now
    );

struct DATA_ROUTE_TABLE* getRoute(  enum ROUTE route );

void fixRoute( struct DATA_ROUTE_TABLE* route );

int getRouteInfo( struct DATA_ROUTE_TABLE* route, char buf[], int nbuf );

void buildStatusMessage( int mtype, const char* (*getData)( char buf[], int maxbuf) );


void acq32_getCalInfo( void );
void acq32_debugGetRegs( void );

void dh_initDataTrigger( void );

void dhFixRouteFull( 
    struct DATA_ROUTE_TABLE* route, int nsamples, int is_wrapped 
    );

int dhIncrementRefillCount( struct DATA_ROUTE_TABLE* route, int id );
void dhSetEventCount( struct DATA_ROUTE_TABLE* route, int* nevents );

extern void fillIcache32( struct DATA_ROUTE_TABLE* route );

void dhInitStatics( void );


enum DH12SINKS { 
    DH12SINK_NORMAL, 
    DH12SINK_NOMASK, 
    DH12SINK_POSITIVE_FALLING_CH0,
    DH4,
    DH_NOMASK_NOTRIGGER,
    DH_fifo_sink_12ch_rising_edge_ZZ = 1037,
    DH_fifo_sink_12ch_rising_edge_01 = 1038
};

void dhSetDh12Sink( enum DH12SINKS sink_id );
void dhSetDh12SinkIx( int ix );

void acq32_selectConsumers( void );

void dh_runPreamble( void );

void dh_clearLastTriggerLine( void );

void dhConfigureAcq16ChannelRouting( int nchannels );


void dhSetUpstreamCount( struct DATA_ROUTE_TABLE* route );
void dhSetTw( struct DATA_ROUTE_TABLE* route, unsigned tw );

void dhAdjustTriggerPosition( int nsamples );    // user tweak
void dhAdjustDigitalTriggerPosition( void );     // auto tweak

void dhConfigureRunner( void );

/* set up the virtual call - impl in acq32_drv.c */

extern void acq32_setRunner( Runner runner );

enum RUN_REASONS {
    RR_MBOX_ABORT = 2,
    RR_RC_QUIT = 3,
    RR_TOO_MANY_EVENTS = 5,
    RR_CACHE_FILL = 99,
    RR_FIFO_ERROR = -1
};

void dhAcq16OffsetAdjust( void );


int dhCaptureHadErrors( void );
int dhGetRouteReason( struct DATA_ROUTE_TABLE* route ); 

void dhBoxCar( int order ); // box car filter post => pre, decimate 1<<order

const char* getCaptureStats( char client_buf[], int maxbuf );
const char* getCalInfo( char client_buf[], int maxbuf );
#endif //__DATAHANDLING_H__
