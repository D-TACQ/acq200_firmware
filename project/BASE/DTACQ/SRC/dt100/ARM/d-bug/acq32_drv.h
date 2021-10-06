/*****************************************************************************
 *
 * File: acq32_drv.h
 *
 * $RCSfile: acq32_drv.h,v $
 * 
 * Copyright (C) 1999 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description: interface to acq32 driver HAL
 *
 * $Id: acq32_drv.h,v 1.102.4.8 2007/04/18 19:44:20 pgm Exp $
 * $Log: acq32_drv.h,v $
 * Revision 1.102.4.8  2007/04/18 19:44:20  pgm
 * handles AB buffers part 1
 *
 * Revision 1.102.4.7  2007/01/03 23:16:16  pgm
 * double fork() is good
 *
 * Revision 1.102.4.6  2007/01/02 18:18:10  pgm
 * *** empty log message ***
 *
 * Revision 1.102.4.5  2004/09/23 21:07:29  pgm
 * B2081
 *
 * Revision 1.102.4.4  2003/12/28 19:42:52  pgm
 * background parallel transform path
 *
 * Revision 1.102.4.3  2003/11/05 22:29:57  pgm
 * hooks GATED_TRANSIENT, dio
 *
 * Revision 1.102.4.2  2003/10/27 20:38:36  pgm
 * reads status from driver
 *
 * Revision 1.102.4.1  2003/08/29 17:41:02  pgm
 * init pci command handling
 *
 * Revision 1.102  2003/06/03 10:10:02  pgm
 * B1725 pci high address swithc for HTM loc status
 *
 * Revision 1.101  2003/06/02 21:18:28  pgm
 * B1724 htstream V2
 *
 * Revision 1.100  2003/02/07 09:50:51  pgm
 * B1713 BoxCar
 *
 * Revision 1.99  2002/11/01 20:49:15  pgm
 * env, invert, acq16 offset stub, dmac tests
 *
 * Revision 1.98  2002/08/21 19:49:20  pgm
 * B1689 - auto trig, sample set adjust OK??
 *
 * Revision 1.97  2002/08/06 19:41:38  pgm
 * lower ICACHE load, add mbox boost
 *
 * Revision 1.96  2002/07/18 18:43:05  pgm
 * 44M stub, 64b copy test
 *
 * Revision 1.95  2002/04/25 10:09:30  pgm
 * B1634 fixes master trig sync
 *
 * Revision 1.94  2002/03/28 15:29:42  pgm
 * cycle reset, debounce opts
 *
 * Revision 1.93  2002/03/22 20:58:41  pgm
 * B1612 resets MULTI_TRIG_BIT correctly
 *
 * Revision 1.92  2002/03/21 15:07:11  pgm
 * merge from 1608 - multi trig safe, edge det etc
 *
 * Revision 1.91  2002/03/21 11:54:51  pgm
 * untabify
 *
 * Revision 1.90  2002/03/14 12:03:03  pgm
 * compiled on benbecula
 *
 * Revision 1.89  2002/03/13 21:43:09  pgm
 * correct formatting with emacs (Stroustrup)
 *
 * Revision 1.88  2002/03/10 12:31:42  pgm
 * no warnings
 *
 * Revision 1.87  2002/03/08 19:32:14  pgm
 * B1591 - default no mask - its fast
 *
 * Revision 1.86  2002/02/16 17:53:33  pgm
 * for release, FET Multitrig nearly works
 *
 * Revision 1.85  2002/02/16 07:40:48  pgm
 * attempting to make edge trig faster
 *
 * Revision 1.84  2002/02/14 19:28:15  pgm
 * acq16 multitrig
 *
 * Revision 1.83  2002/02/11 22:06:08  pgm
 * DO is GO
 *
 * Revision 1.82  2002/02/10 17:56:50  pgm
 * usecs timing reliable over time
 *
 * Revision 1.81  2002/02/08 22:41:24  pgm
 * multi num channel configs for acq16
 *
 * Revision 1.80  2002/01/18 18:33:10  pgm
 * *** empty log message ***
 *
 * Revision 1.79  2002/01/13 22:21:58  pgm
 * acq32 fast threshtrig kludge coded
 *
 * Revision 1.78  2002/01/05 21:48:26  pgm
 * fixes DUMDMA stride probs
 *
 * Revision 1.76  2001/12/05 20:59:32  pgm
 * sink 3 x 4 is the biz
 *
 * Revision 1.75  2001/12/05 19:51:02  pgm
 * works, but not at 2.5MSPS
 *
 * Revision 1.74  2001/12/04 21:28:53  pgm
 * full rate threshtrig in switchable sink
 *
 * Revision 1.73  2001/12/02 20:28:12  pgm
 * AO th in common header
 *
 * Revision 1.72  2001/11/23 22:28:07  pgm
 * fixes AO ST, direct dumdma speed demo
 *
 * Revision 1.71  2001/11/20 22:23:53  pgm
 * better dregs, i2o in capture, fix AO soft trig
 *
 * Revision 1.70  2001/10/28 21:49:32  pgm
 * llcontrol 24 bit rolloever fixed
 *
 * Revision 1.69  2001/10/20 19:42:58  pgm
 * intclock master/slave WORKS
 *
 * Revision 1.68  2001/10/06 21:03:07  pgm
 * CPEM with PXI trig works, AI overflow to cope with
 *
 * Revision 1.67  2001/09/03 15:52:45  pgm
 * threshold triggering should now work
 *
 * Revision 1.66  2001/08/31 09:10:20  pgm
 * acq16 has channel mask, works on rearm
 *
 * Revision 1.65  2001/08/27 12:55:18  pgm
 * acq16 runs again - needs LCA bit change, caveat embedded trigger
 *
 * Revision 1.64  2001/08/20 15:50:48  pgm
 * acq32_setUserLeds nv version of acq32_setLeds()
 *
 * Revision 1.63  2001/08/13 16:43:23  pgm
 * ll counter extend works
 *
 * Revision 1.62  2001/08/10 20:47:58  pgm
 * fix lll counter overflow, add USER LEDS
 *
 * Revision 1.61  2001/07/12 21:29:55  pgm
 * DO, AO now identical - gotta work please
 *
 * Revision 1.60  2001/07/11 12:47:55  pgm
 * *** empty log message ***
 *
 * Revision 1.59  2001/06/28 21:50:09  pgm
 * distributor moves into route
 *
 * Revision 1.58  2001/06/27 22:49:41  pgm
 * better status handling, acq16 deeper fifo
 *
 * Revision 1.57  2001/06/23 19:03:36  pgm
 * acq16 part 1 captures runs
 *
 * Revision 1.56  2001/06/23 08:32:16  pgm
 * remove extra fixups, printf from acq32_run()
 *
 * Revision 1.55  2001/06/13 20:44:30  pgm
 * lca-ident, fix pre- channel swap bug
 *
 * Revision 1.54  2001/06/11 19:19:52  pgm
 * better
 *
 * Revision 1.53  2001/06/09 11:56:06  pgm
 * fifo_sink_notests()
 *
 * Revision 1.52  2001/06/08 20:02:01  pgm
 * GUT builds, fifo_sink testing
 *
 * Revision 1.51  2001/06/04 19:34:49  pgm
 * GUT added (well it compiles anyway)
 *
 * Revision 1.50  2001/05/19 12:59:56  pgm
 * low latency setup - done
 *
 * Revision 1.49  2001/05/15 08:50:33  pgm
 * even task handling, slicker status, err detect
 *
 * Revision 1.48  2001/05/14 16:24:24  pgm
 * fifo_source, increments, AO fifo 3/4 250ksps is go
 *
 * Revision 1.47  2001/05/09 07:46:59  pgm
 * more adds to Wavegen, EmbTrig
 *
 * Revision 1.46  2001/05/08 20:18:14  pgm
 * first cut wavegen in acq32_run() - compiles
 *
 * Revision 1.45  2001/05/07 14:23:01  pgm
 * storeman in
 *
 * Revision 1.44  2001/05/07 11:38:37  pgm
 * DAC output - wrk in progress
 *
 * Revision 1.43  2001/05/05 08:58:37  pgm
 * ext clk works, better debug <dr>, ldcal, channel ass correct
 *
 * Revision 1.42  2001/05/04 20:37:07  pgm
 * big upgrade to improve qwual, match spec
 *
 * Revision 1.41  2001/05/04 09:32:41  pgm
 * consistent pollcat now
 *
 * Revision 1.40  2001/04/23 20:22:17  pgm
 * fixed bugs in setCalDacXXX - no casts here -
 *
 * Revision 1.39  2001/04/19 19:55:25  pgm
 * mbox cal ops, physchan mask
 *
 * Revision 1.38  2001/04/18 20:39:09  pgm
 * built in multi cal facility - needs busprot
 *
 * Revision 1.37  2001/04/16 17:36:42  pgm
 * fifo_sink - first cut
 *
 * Revision 1.36  2001/04/14 20:37:50  pgm
 * CPCI routing coded, needs testing
 *
 * Revision 1.35  2001/04/14 19:06:13  pgm
 * remvoe WORTODOS - new LCA is GO
 *
 * Revision 1.34  2001/04/13 20:05:20  pgm
 * new LCA - compiled, not tested, must check WORKTODOS
 *
 * Revision 1.33  2001/04/09 09:58:49  pgm
 * deep grief, distributor bug fixed (testing..)
 *
 * Revision 1.32  2001/03/19 23:47:22  pgm
 * updated d-bug help, nodos
 *
 * Revision 1.31  2001/03/04 21:09:29  pgm
 * LL kludge, Data Trigger 99kHz
 *
 * Revision 1.30  2001/02/17 21:25:15  pgm
 * LL kludge part 1
 *
 * Revision 1.29  2000/11/04 23:49:20  pgm
 * works pre==0, better ID
 *
 * Revision 1.28  2000/10/29 20:43:33  pgm
 * fix bigdump end buffer problem
 *
 * Revision 1.27  2000/10/15 19:58:04  pgm
 * multi samples/cycle WORKS, streams too and is FAST enough
 *
 * Revision 1.26  2000/10/14 22:35:25  pgm
 * first pass speed up
 *
 * Revision 1.25  2000/10/01 18:10:29  pgm
 * bigdump and getFwrev first cut
 *
 * Revision 1.24  2000/10/01 08:16:10  pgm
 * improved GATED_CONTINUOUS, flash prog
 *
 * Revision 1.23  2000/08/14 19:11:28  pgm
 * nodos
 *
 * Revision 1.22  2000/08/14 19:10:53  pgm
 * bulked messages works (partly)
 *
 * Revision 1.21  2000/08/04 22:13:07  pgm
 * Streaming Works cvs status | grep Local! Ship IT Quick
 *
 * Revision 1.20  2000/06/25 19:29:27  pgm
 * acq32 push nearly works, with PIO not DMAC :-(
 *
 * Revision 1.19  2000/06/25 06:16:36  pgm
 * hacked in streaming to dataHandling et al
 *
 * Revision 1.18  2000/06/24 15:17:29  pgm
 * I2O rools OK
 *
 * Revision 1.17  2000/06/11 13:18:20  pgm
 * queries in ST_RUN allowed, also Abort works
 *
 * Revision 1.16  2000/06/05 21:33:19  pgm
 * SC1 driver stubs filled
 *
 * Revision 1.15  2000/06/04 19:52:40  pgm
 * STUB SC handling compiles
 *
 * Revision 1.14  2000/03/25 19:15:45  pgm
 * interrupt handling, acq16 tests
 *
 * Revision 1.13  1999/11/18 13:42:15  pgm
 * revised DATA_ROUTE should work wrapped
 *
 * Revision 1.12  1999/11/16 10:52:41  pgm
 * GATED_CONTINUOUS runs
 *
 * Revision 1.11  1999/11/14 18:47:20  pgm
 * update
 *
 * Revision 1.9  1999/11/12 18:11:26  pgm
 * default mbox init, LEDS
 *
 * Revision 1.3  1999/10/30 21:00:37  pgm
 * pgm backed up
 *
 * Revision 1.1  1999/10/22 19:48:24  pgm
 * first pass mbox control, compiles gcc
 *
 * Revision 1.1  1999/10/22 16:26:49  pgm
 * first entry to cvs
 *
 *
\*****************************************************************************/


#ifndef _ACQ32_DRV_H_
#define _ACQ32_DRV_H_

#include "REGDefs.h"

struct CAPTURE_DEF {
    struct TWO_LENGTHS {
        int pre;
        int post;
    }
        user_request_samples,        // WHAT the user asked for
        sys_request_samples,         // WHAT the system thought was best
        actual_samples;             // the actual result
    int mode;
    int has_triggered;
};

void acq32_setCaptureDef( struct CAPTURE_DEF* cd );
struct CAPTURE_DEF* acq32_getCaptureDef( void );

extern const char* getBuildDate( char client_buf[], int maxbuf );
extern const char* getConfig( char client_buf[], int maxbuf );

extern void makeMarker( char* file, int line );
extern void makeEmbed( char* file, int line, void* p1, void* p2 );

#define EMBED( pfrom, pto ) makeEmbed( __FILE__, __LINE__, (void*)pfrom, (void*)pto )
#ifndef MARK
#define MARK makeMarker( __FILE__, __LINE__ )
#endif
extern void acq32_drv_init_statics( void );
extern void acq32_drv_init( void );
extern void spin( unsigned msecs );
extern void uspin( unsigned usecs );
// nb the msecs are not at all accurate!

extern volatile unsigned* mbox( int );  // returns mbox base address

extern int kb_hit( void );           // returns 1 when char available
extern void acq32_setMode( int mode, int prelen, int postlen );

extern void acq32_setArm( void );

extern int acq32_getNumSamples( void );
extern int acq32_getPreSamples( void );

extern void acq32FlushDCache( void );                   // impl in cache.s
extern void acq32CleanDCache( void );
extern void acq32DrainWriteBuffer( void );
extern void acq32EnableClockSW( void );
extern void acq32DisableClockSW( void );
extern void acq32FxnTest( void );
extern void acq32FlushTLBs( void );

void acq32_printFifoStats( void );

enum DAC_POT_DIR { DAC_POT_UP, DAC_POT_DN, DAC_POT_DONE };

extern void acq32_setDacPot( enum DAC_POT_DIR dir );

// LED_CONTROL

enum LEDCON {
    LED_RED_ON    = 0x1,
    LED_RED_OFF   = 0x2,
    LED_RED_FLIP  = 0x3,

    LED_RED_MASK  = 0x3,

    LED_GREEN_ON  = 0x4,
    LED_GREEN_OFF = 0x8,
    LED_GREEN_FLIP= 0xc,

    LED_GREEN_MASK= 0xc,
   
    LED_3_ON      = 0x10,
    LED_3_OFF     = 0x20,
    LED_3_FLIP    = 0x30,
    LED_3_MASK    = 0x30,
   
    LED_4_ON      = 0x40,
    LED_4_OFF     = 0x80,
    LED_4_FLIP    = 0xc0,
    LED_4_MASK    = 0xc0
};

extern void acq32_setLeds( /*enum LEDCON*/ int ledcon );
extern void acq32_setUserLeds( /*enum LEDCON*/ int ledcon );
extern void acq32_setGreenLed( /*enum LEDCON*/ int ledcon );
extern void acq32_setRedLed( /*enum LEDCON*/ int ledcon );

extern void acq32_setCalDacBlock( enum CALDAC dac, const unsigned short *pstart, int stride );

extern void acq32_go( int nsamples );

#define GC_TRIGGER 1
#define GC_GATE    0

extern void acq32_go_continuous( int nsamples, int trigger_not_gate );
extern void acq32_go_soft_continuous( int nsamples );

extern int acq32_hasTriggered( void );

extern void acq32_setCommandInterruptAck( int on );
extern void acq32_assertPciInterrupt( unsigned short bits );


extern unsigned acq32_setInternalClock( unsigned hz );
extern unsigned acq32_getInternalClock( void );
extern void acq32_setExternalClock( int DIx, unsigned divisor, int is_master, int DOx );
void acq32_setInternalClockMasterOutput( int DOx );
void acq32_clrInternalClockMasterOutput( void );


extern int acq32_SC_SetDioDir( unsigned dir );
extern int acq32_SC_SetDioOutputs( unsigned outputs );
extern unsigned int acq32_SC_GetDioInputs( void );

extern void acq32_SC_SetChannel( int ichan, int g1, int g2, int v_ex );
extern void acq32_SC_SetPot( int iboard, int volt_sel, int dir );

/*
 * implemented in lowlatency.c
 */
 
extern void acq32_setupLowLatencyControlSoftClock(
    int clkpos, int trpos, int intsoftclock
    );
extern void acq32_setupLowLatencyControlExtClock(
    int clkpos, int trpos, int intdivclk, unsigned divisor
    );
extern void acq32_doLowLatencyControl( void );
// "protected"

#ifdef PGMCOMOUT
extern int pgm_getchar( void );

void acq32_fifoReset( void );

void acq32_AIfifoReset( void );
void acq32_AOfifoReset( void );
void acq32_DOfifoReset( void );

int acq32_getAIfifoNumSamples( void );
int acq32_getAOfifoNumSamples( void );
int acq32_getDOfifoNumSamples( void );

int acq32_getAIfifoError( void );
int acq32_getAOfifoError( void );
int acq32_getDOfifoError( void );
#endif
extern void SAir_FlushDCacheEntry( unsigned );


extern int acq32_trigger_is_off( void );
extern int acq32_trigger_is_on( void );
extern int acq32_clock_is_hi( void );

extern int acq32_doBigDump( unsigned pci_addr, unsigned len );



extern int fifo_sink(
    unsigned** ppdest,    // R0
    unsigned** ppfifo,    // R1
    int nsamples,         // R2
    unsigned mask         // R3
    );

extern int fifo_sink_notests(
    unsigned** ppdest,    // R0
    unsigned** ppfifo,    // R1
    int nsamples          // R2
    );

extern int fifo_sink_notests_12ch(
    unsigned** ppdest,    // R0
    unsigned** ppfifo,    // R1
    int nsamples          // R2
    );

extern int fifo_sink_12ch(
    unsigned** ppdest,             // r0
    unsigned** ppfifo,             // r1
    int nsamples,                  // r2 - min 8
    unsigned channel_mask          // r3
    );
extern int fifo_sink_12ch_nomask(
    unsigned** ppdest,             // r0
    unsigned** ppfifo,             // r1
    int nsamples,                  // r2 - min 8
    unsigned channel_mask          // r3
    );
extern int fifo_sink_12ch_nomask_notrigger(
    unsigned** ppdest,             // r0
    unsigned** ppfifo,             // r1
    int nsamples,                  // r2 - min 8
    unsigned channel_mask          // r3
    );

extern int fifo_sink_12ch_rising_edge_ZZ(
    unsigned** ppdest,             // r0
    unsigned** ppfifo,             // r1
    int nsamples,                  // r2 - min 8
    unsigned channel_mask          // r3
    );

extern int fifo_sink_12ch_nomask4(
    unsigned** ppdest,             // r0
    unsigned** ppfifo,             // r1
    int nsamples,                  // r2 - min 8
    unsigned channel_mask          // r3
    );

extern void *memcpy64(void *dest, const void *src );

/*
 * NB NB: default acq16 sink now has NO MASK
 *
 * software mask is a bit of an anachronism with new acq16 lca
 */
 
#define DEFSINK( lbl ) \
    extern int lbl( unsigned**, unsigned **, int, unsigned );\
    extern void lbl##99 ( void ); \
    extern int lbl##m( unsigned**, unsigned **, int, unsigned );\
    extern void lbl##m99 ( void )
    


DEFSINK( acq16_fifo_sink_16 );
DEFSINK( acq16_fifo_sink_12 );
DEFSINK( acq16_fifo_sink_08 );
DEFSINK( acq16_fifo_sink_04 );
DEFSINK( acq16_fifo_sink_02 );

DEFSINK( fifo_sink_12ch_mean4 );
DEFSINK( fifo_sink_12ch_mean4_nomask );

DEFSINK( fifo_sink_12ch_falling_edge_01 );
DEFSINK( fifo_sink_12ch_falling_edge_23 );
DEFSINK( fifo_sink_12ch_falling_edge_45 );
DEFSINK( fifo_sink_12ch_falling_edge_67 );
DEFSINK( fifo_sink_12ch_falling_edge_89 );
DEFSINK( fifo_sink_12ch_falling_edge_AB );
DEFSINK( fifo_sink_12ch_rising_edge_01 );
DEFSINK( fifo_sink_12ch_rising_edge_23 );
DEFSINK( fifo_sink_12ch_rising_edge_45 );
DEFSINK( fifo_sink_12ch_rising_edge_67 );
DEFSINK( fifo_sink_12ch_rising_edge_89 );
DEFSINK( fifo_sink_12ch_rising_edge_AB );




extern int fifo_source (
    unsigned* pdest,        // R0
    unsigned** ppsource,    // R1
    int nsamples            // R3 multiples of 16 only please
    );

typedef int (*Sink)( 
    unsigned** ppdest,             // r0
    unsigned** ppfifo,             // r1
    int nsamples,                      // r2 - min 8
    unsigned channel_mask          // r3
    );

void acq32_selectCalset( int calset ); // {0..7}

#define SHOW_ALL -1
const char* acq32_showRegs( char buf[], int maxbuf, int isel /* = SHOW_ALL */ );
const char* acq32_showRegsAll( char buf[], int maxbuf );
unsigned getDramArrayLength( int array );


/*
 * FIFO_STATUS - tried bitfields, they're crap,
 * at least masks we can translate into asm
 */
 
#define FS_AI_COUNT    0x0000000f
#define FS_AO_HF       0x00000010
#define FS_DO_HF       0x00000020
#define FS_ERROR       0x00000040
#define FS_RAW         0xffff0000



/*
 * request activation of AO, DO services
 */
 
void acq32_AO_activate_waveform( int activate );
void acq32_DO_activate_waveform( int activate );

extern int numberOfEnabledChannels( void );

char* get_acqXX_id( void );
void acq32_load_lca( u32 alt );

void acq32_configureForLcaFlavor( void );

int acq32_fixRoutes( void );
// returns TRUE if capture successful

void acq16_setSimulatedData( int enable );

void acq32_getConfig( void );
 
int isAcq16( void );
int isAcq32( void );

int acq32_getAIcount( void );
int acq32_getAOcount( void );

int teeup_AO_waveform( void );
int teeup_DO_waveform( void );
void teeupOutputWaveforms( int enable_AI );

void llSetRtClockTiming( int enable );

/*
 * recommended refill thresholds
 */
 
#define AO_FIFO_TH    0x0800
#define DO_FIFO_TH    0x0800

#define BELOW_TH( fsw, xx ) \
    (((fsw)&(Q32_DATA_FIFO_STAT_##xx##FIFOPTR))<=(xx##_FIFO_TH))

#define AO_FIFO_FILL  32
#define DO_FIFO_FILL  32

void reportSAConfig( void );

int acq32_getActiveChannels( void );
void acq16_setActiveChannelCount( int nchannels );

const char* get_acq16_AI_SamplesLut( int nchannels );

int acq16_setBoost( int iboost );

#define DEFAULT_AITRIG      0x4
#define AITRIG_ON_MY_BOARD      0x4

void acq32_setAItrigDIx( int Dix );
int  acq32_getAItrigDIx( void );

struct DATA_ROUTE_TABLE;                // blind structure 


/*
 * MULTI_TRIG - first board to detect trigger pulls this bit
 */
#define MULTI_TRIG_BIT        Q32_DIO_DATA_GATE    // cos we know that one works!
#define MULTI_TRIG_BIT_SRC    Q32_DIO_DATA_GATE_SRC

void multi_trig_cleanup( struct DATA_ROUTE_TABLE* _this );


unsigned acq16_getFullMask( void );
int acq16_getActiveChannels( void );
int acq16_getFittedChannelCount( void );

void acq32_setChannelsInFifo( int nchan );

void acq32_setTrigDebounce( int enable );
void acq16_disableEmbeddedTrigger( int nem );

extern unsigned G_InvertedInputs;
extern unsigned G_Acq16OffsetAdjust;
extern unsigned G_UsesNewClock;
extern const char* G_PostProcessCommand;


extern void setPciAddressExtension( int enable );


extern void acq32_setChannelMask( unsigned mask );
extern void acq16_setChannelMask( unsigned mask );

extern int hasStarted(void);
extern int hasStopped(void);
extern void stopPostprocess(void);

#endif               // _ACQ32_DRV_H_
