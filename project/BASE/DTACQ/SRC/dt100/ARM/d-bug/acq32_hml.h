/*****************************************************************************
 *
 * File: acq32_hml.h - Hardware Model layer for LCA
 *
 * $RCSfile: acq32_hml.h,v $
 * 
 * Copyright (C) 2001 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: acq32_hml.h,v 1.30.4.2 2003/09/04 21:08:42 pgm Exp $
 * $Log: acq32_hml.h,v $
 * Revision 1.30.4.2  2003/09/04 21:08:42  pgm
 * getFwrev works (x156)
 *
 * Revision 1.30.4.1  2003/08/29 17:41:02  pgm
 * init pci command handling
 *
 * Revision 1.30  2002/11/29 20:42:13  pgm
 * clock invert opt now bitwise sensitive
 *
 * Revision 1.29  2002/11/01 20:49:15  pgm
 * env, invert, acq16 offset stub, dmac tests
 *
 * Revision 1.28  2002/08/21 12:06:12  pgm
 * this is as fast as it gets
 *
 * Revision 1.27  2002/08/21 09:02:39  pgm
 * B1680 dhAcq12run as fast as it will get
 *
 * Revision 1.26  2002/04/24 12:51:28  pgm
 * B1633 TRMAS for acq16
 *
 * Revision 1.25  2002/03/21 11:54:51  pgm
 * untabify
 *
 * Revision 1.24  2002/03/14 12:03:03  pgm
 * compiled on benbecula
 *
 * Revision 1.23  2002/03/13 21:43:09  pgm
 * correct formatting with emacs (Stroustrup)
 *
 * Revision 1.22  2001/11/18 10:06:00  pgm
 * leaner llcontrol - has llcount probs
 *
 * Revision 1.21  2001/11/04 20:56:06  pgm
 * HTRIGEN included
 *
 * Revision 1.20  2001/10/07 10:19:59  pgm
 * GPME looking good build 1370 checkin
 *
 * Revision 1.19  2001/10/06 21:03:07  pgm
 * CPEM with PXI trig works, AI overflow to cope with
 *
 * Revision 1.18  2001/10/01 14:01:25  pgm
 * hard trigger works for AO
 *
 * Revision 1.17  2001/09/02 21:30:19  pgm
 * multi thresh trig, vranging
 *
 * Revision 1.16  2001/07/13 16:56:26  pgm
 * AO+DO works at last
 *
 * Revision 1.15  2001/07/12 21:29:55  pgm
 * DO, AO now identical - gotta work please
 *
 * Revision 1.14  2001/07/11 21:17:58  pgm
 * AI fifo==32, should fix AO load probs
 *
 * Revision 1.13  2001/06/11 14:47:25  pgm
 * GUT ran thru
 *
 * Revision 1.12  2001/06/08 20:02:01  pgm
 * GUT builds, fifo_sink testing
 *
 * Revision 1.11  2001/06/04 19:34:49  pgm
 * GUT added (well it compiles anyway)
 *
 * Revision 1.10  2001/05/20 21:22:50  pgm
 * ll WIP - clocks, no samples
 *
 * Revision 1.9  2001/05/19 19:44:40  pgm
 * LL work in progress
 *
 * Revision 1.8  2001/05/19 12:59:56  pgm
 * low latency setup - done
 *
 * Revision 1.7  2001/05/19 10:30:04  pgm
 * low latency compiles
 *
 * Revision 1.6  2001/05/15 08:50:33  pgm
 * even task handling, slicker status, err detect
 *
 * Revision 1.5  2001/05/08 20:18:14  pgm
 * first cut wavegen in acq32_run() - compiles
 *
 * Revision 1.4  2001/05/07 19:49:11  pgm
 * loads waveforms, uses storeman OK
 *
 * Revision 1.3  2001/05/07 11:38:37  pgm
 * DAC output - wrk in progress
 *
 * Revision 1.2  2001/05/05 06:33:10  pgm
 * magic number removal - this works
 *
 * Revision 1.1  2001/05/04 20:37:07  pgm
 * big upgrade to improve qwual, match spec
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
 * GCC changes here
 */

#define __inline inline


#ifndef __ACQ32_HML_H__
#define __ACQ32_HML_H__


#ifndef _REGDEFS_H_
#include "REGDEFS.h"
#endif

#define Q32_DATA_FIFO_CTRL_XXFIFORES \
    (Q32_DATA_FIFO_CTRL_AIFIFORES|\
     Q32_DATA_FIFO_CTRL_AOFIFORES|\
     Q32_DATA_FIFO_CTRL_DOFIFORES)

#define Q32_DATA_FIFO_CTRL_XXFIFOEN \
    (Q32_DATA_FIFO_CTRL_AIFIFOEN|\
     Q32_DATA_FIFO_CTRL_AOFIFOEN|\
     Q32_DATA_FIFO_CTRL_DOFIFOEN)

#define FIFO_ENABLE( xx ) \
    (*q32_data_fifo_ctrl |= Q32_DATA_FIFO_CTRL_##xx##FIFOEN)

#define FIFO_DISABLE( xx ) \
    (*q32_data_fifo_ctrl &= ~Q32_DATA_FIFO_CTRL_##xx##FIFOEN)


#define FIFO_RESET( xx ) \
    do { \
         FIFO_DISABLE( xx );\
         *q32_data_fifo_ctrl |=  Q32_DATA_FIFO_CTRL_##xx##FIFORES; \
         *q32_data_fifo_ctrl &= ~Q32_DATA_FIFO_CTRL_##xx##FIFORES; \
        } while( 0 )
   
static __inline void acq32_AIfifoReset( void )
{
    R16* q32_data_fifo_ctrl = Q32_DATA_FIFO_CTRL;
    FIFO_RESET( AI );
}

static __inline void acq32_AOfifoReset( void )
{
    R16* q32_data_fifo_ctrl = Q32_DATA_FIFO_CTRL;
    FIFO_RESET( AO );
}

static __inline void acq32_DOfifoReset( void )
{
    R16* q32_data_fifo_ctrl = Q32_DATA_FIFO_CTRL;
    FIFO_RESET( DO );
}

static __inline void acq32_fifoReset( void )
// reset all da fifos
{
    R16* q32_data_fifo_ctrl = Q32_DATA_FIFO_CTRL;
    FIFO_RESET( XX );
}
#undef FIFO_RESET  
#undef FIFO_DISABLE
#undef Q32_DATA_FIFO_CTRL_XXFIFOEN
#undef Q32_DATA_FIFO_CTRL_XXFIFORES 


static __inline void acq32_AIfifoEnable( void )
{
    R16* q32_data_fifo_ctrl = Q32_DATA_FIFO_CTRL;
    FIFO_ENABLE( AI );
}

static __inline void acq32_fifoEnable( void )
{
    acq32_AIfifoEnable();
}
#undef FIFO_ENABLE


#define SET_DIO_SFR( XX, DIx, RISING ) \
    R16* sfr = Q32_DIO_##XX##_SFR; \
    u16 tmp = *sfr; \
    if ( G_InvertedInputs&(1<<DIx) ) RISING = !RISING; \
    tmp &= ~(Q32_DIO_xx_SFR_CLKSRC_MASK|Q32_DIO_xx_SFR_CLKPOL_RISING);\
    tmp |= Q32_DIO_xx_SFR_CLKSRC(DIx)|(RISING?Q32_DIO_xx_SFR_CLKPOL_RISING:0);\
    *sfr = tmp;
  
static __inline void acq32_AI_ExtClkSrcSelect( int DIx, int rising )
{
    R16* q32_func_ctrl = Q32_FUNC_CTRL;
    SET_DIO_SFR( AI, DIx, rising );
    *q32_func_ctrl |= Q32_FUNC_CTRL_AIC_EXT;
}
static __inline void acq32_AO_ExtClkSrcSelect( int DIx, int rising )
{
    R16* q32_func_ctrl = Q32_FUNC_CTRL;
    SET_DIO_SFR( AO, DIx, rising );
    *q32_func_ctrl |= Q32_FUNC_CTRL_AOC_EXT;
}
static __inline void acq32_DO_ExtClkSrcSelect( int DIx, int rising )
{
    R16* q32_func_ctrl = Q32_FUNC_CTRL;
    SET_DIO_SFR( DO, DIx, rising );
    *q32_func_ctrl |= Q32_FUNC_CTRL_DOC_EXT;
}


static __inline void acq32_AI_IntClkSrcSelect( void )
{
    R16* q32_func_ctrl = Q32_FUNC_CTRL;
    *q32_func_ctrl &= ~Q32_FUNC_CTRL_AIC_EXT;
}
static __inline void acq32_AO_IntClkSrcSelect( void )
{
    R16* q32_func_ctrl = Q32_FUNC_CTRL;
    *q32_func_ctrl &= ~Q32_FUNC_CTRL_AOC_EXT;
}
static __inline void acq32_DO_IntClkSrcSelect( void )
{
    R16* q32_func_ctrl = Q32_FUNC_CTRL;
    *q32_func_ctrl &= ~Q32_FUNC_CTRL_DOC_EXT;
}

#undef SET_DIO_SFR

#define SET_DIO_SFR_TRIG( XX, DIx, RISING, xtra ) \
    u16 temp = *Q32_DIO_##XX##_SFR; \
    temp &= ~(Q32_DIO_xx_SFR_TRSRC_MASK|Q32_DIO_xx_SFR_TRPOL_RISING);\
    temp |= Q32_DIO_xx_SFR_TRSRC(DIx)| \
            (RISING?Q32_DIO_xx_SFR_TRPOL_RISING:0)|(xtra);\
    *Q32_DIO_##XX##_SFR = temp;\
    return temp;

#define CLR_DIO_SFR_TRIG( XX ) \
    *Q32_DIO_##XX##_SFR &= ~Q32_DIO_xx_SFR_HTRIGEN

static __inline u16 acq16_AI_MultiTrigMasterSrcSelect( int DIx, int enable )
{
    SET_DIO_SFR_TRIG( AI, DIx, enable, 0 );
}

static __inline u16 acq32_AO_ExtTrigSrcSelect( int DIx, int rising )
{
    SET_DIO_SFR_TRIG( AO, DIx, rising, Q32_DIO_xx_SFR_HTRIGEN );
}

static __inline u16 acq32_DO_ExtTrigSrcSelect( int DIx, int rising )
{
    SET_DIO_SFR_TRIG( DO, DIx, rising, Q32_DIO_xx_SFR_HTRIGEN );
}

static __inline void acq32_AO_ExtTrigSrcDisable( void )
{
    CLR_DIO_SFR_TRIG( AO );
}

static __inline void acq32_DO_ExtTrigSrcDisable( void  )
{
    CLR_DIO_SFR_TRIG( DO );
}


/*
 * convert enable - either individual or group 
 */
static __inline void acq32_AI_ConvertEnable( void )
{
    R16* q32_func_ctrl = Q32_FUNC_CTRL;
    *q32_func_ctrl |= Q32_FUNC_CTRL_AIEN;
}
static __inline void acq32_AO_ConvertEnable( void )
{
    R16* q32_func_ctrl = Q32_FUNC_CTRL;
    *q32_func_ctrl |= Q32_FUNC_CTRL_AOEN;
}
static __inline void acq32_DO_ConvertEnable( void )
{
    R16* q32_func_ctrl = Q32_FUNC_CTRL;
    *q32_func_ctrl |= Q32_FUNC_CTRL_DOEN;
}


static __inline void acq32_convertEnable( int AI, int AO, int DO )
{
    R16* q32_func_ctrl = Q32_FUNC_CTRL;
    *q32_func_ctrl |= (AI? Q32_FUNC_CTRL_AIEN: 0) +
        (AO? Q32_FUNC_CTRL_AOEN: 0) +
        (DO? Q32_FUNC_CTRL_DOEN: 0);
}

static __inline void acq32_convertDisable( int AI, int AO, int DO )
{
    R16* q32_func_ctrl = Q32_FUNC_CTRL;
    *q32_func_ctrl &= ~(  (AI? Q32_FUNC_CTRL_AIEN: 0) +
                          (AO? Q32_FUNC_CTRL_AOEN: 0) +
                          (DO? Q32_FUNC_CTRL_DOEN: 0)   );
}

#define GET_SFR_STATE( XX, CC ) \
    ( (*Q32_DIO_##XX##_SFR & Q32_DIO_xx_SFR_##CC##VAL) != 0)

static __inline int acq32_getAI_ClkState( void )
{
    return GET_SFR_STATE( AI, CLK );
}
static __inline int acq32_getAI_TrigState( void )
{
    return GET_SFR_STATE( AI, TR );
}
static __inline int acq32_getAO_ClkState( void )
{
    return GET_SFR_STATE( AO, CLK );
}
static __inline int acq32_getAO_TrigState( void )
{
    return GET_SFR_STATE( AO, TR );
}
static __inline int acq32_getDO_ClkState( void )
{
    return GET_SFR_STATE( DO, CLK );
}
static __inline int acq32_getDO_TrigState( void )
{
    return GET_SFR_STATE( DO, TR );
}
/*
 * WORKTODO - JML confirm - soft clock/ trig is a pulse ??
 */
 
static __inline void acq32_softClock( int hi )
{
    if ( hi ){
        *Q32_FUNC_CTRL |= Q32_FUNC_CTRL_SOFTCLK_HI;
    }else{
        *Q32_FUNC_CTRL &= ~Q32_FUNC_CTRL_SOFTCLK_HI; 
    }
}

static __inline void acq32_enableDACS( )
{
    *Q32_FUNC_CTRL |= Q32_FUNC_CTRL_NDAC_RESET;
}
static __inline void acq32_softTrigRaw( u16 trigger )
{
    u16 fcr = *Q32_FUNC_CTRL;
 
    PRINTF( "acq32_softTrigRaw() %p 0x%04x 0x%04x\n", 
            Q32_FUNC_CTRL, fcr|trigger, fcr );
       
    *Q32_FUNC_CTRL = fcr | trigger;
    
    // slow it down - will it make DO work ??
    PRINTF( "acq32_softTrigRaw() 0x%04x\n", *Q32_FUNC_CTRL );    
    
    *Q32_FUNC_CTRL = fcr;
    
    PRINTF( "acq32_softTrigRaw() 0x%04x\n", *Q32_FUNC_CTRL );     
}
static __inline void acq32_softTrig( int AI, int AO, int DO )
{
    u16 trigger = (AI? Q32_FUNC_CTRL_AISTRIG_HI: 0) +
        (AO? Q32_FUNC_CTRL_AOSTRIG_HI: 0) +
        (DO? Q32_FUNC_CTRL_DOSTRIG_HI: 0);
                          
    acq32_softTrigRaw( trigger );
}

static __inline void acq32_setDO_waveform( int enable )
{
    if ( enable ){
        *Q32_FUNC_CTRL |= Q32_FUNC_CTRL_DO_WAVE_MODE+Q32_FUNC_CTRL_DOEN;
    }else{
        *Q32_FUNC_CTRL &= ~(Q32_FUNC_CTRL_DO_WAVE_MODE+Q32_FUNC_CTRL_DOEN);
    }
}

#define FSW  u32 /*  FifoStatusWord */

static __inline int getAIfifoNumSamples( FSW fsw )
{
    return Q32_GET_AI_FIFOSAMPLES(fsw);
}
static __inline int acq32_getAIfifoNumSamples( void )
{
    return getAIfifoNumSamples( *Q32_DATA_FIFO_STAT );
}

static __inline int getAOfifoNumSamples( FSW fsw )
{
    return Q32_GET_AO_FIFOSAMPLES(fsw);
}
static __inline int acq32_getAOfifoNumSamples( void )
{
    return getAOfifoNumSamples( *Q32_DATA_FIFO_STAT );
}

static __inline int getDOfifoNumSamples( FSW fsw )
{
    return Q32_GET_DO_FIFOSAMPLES(fsw);
}
static __inline int acq32_getDOfifoNumSamples( void )
{
    return getDOfifoNumSamples( *Q32_DATA_FIFO_STAT );
}


static __inline FSW acq32_getFifoStatus( void ) 
{
    return  *Q32_DATA_FIFO_STAT;
}

#define Q32_DATA_FIFO_STAT_XXFIFOERR Q32_DATA_FIFO_STAT_FLAGS

static __inline int acq32_getAIfifoError( void )
{
    return (*Q32_DATA_FIFO_STAT&Q32_DATA_FIFO_STAT_AIFIFOFF) != 0;
}

static __inline int acq32_getAOfifoError( void )
{
    return (*Q32_DATA_FIFO_STAT&Q32_DATA_FIFO_STAT_AOFIFOEF) != 0;
}

static __inline int acq32_getDOfifoError( void )
{
    return (*Q32_DATA_FIFO_STAT&Q32_DATA_FIFO_STAT_DOFIFOEF) != 0;
}


static __inline void acq32_setDIOinputs( u16 inputs )
{
    *Q32_DIO_DIR &= ~inputs;
}

static __inline void acq32_setDIOoutputs( u16 outputs )
{
    *Q32_DIO_DIR |= outputs;
}

static __inline void acq32_clrDObits( u16 bits ){
    *Q32_DIO_DATA &= ~bits;
}

static __inline void acq32_setDObits( u16 bits ){
    *Q32_DIO_DATA |= bits;
}
static __inline u16 acq32_getDI( void ) {
    return *Q32_DIO_DATA;
}


static __inline void acq32_enableMaskBits( u16 the_bits )
{
    *Q32_DI_EMBED_MASK |= the_bits;
}

static __inline void acq32_disableMaskBits( u16 the_bits )
{
    R16* q32_di_embed_mask = Q32_DI_EMBED_MASK;
    *q32_di_embed_mask &= ~the_bits;
}

static __inline u32 _getDOT( R16* hi, R16* lo )
{
    u32 dot = *lo;
    u16 tmp = *hi&(Q32_DOT_xxxx_HI_RUN+Q32_DOT_xxxx_HI_COUNT);
    
    return dot += tmp<<16;
}

#define TIMER_RUNNING (Q32_DOT_xxxx_HI_RUN<<16)

static __inline u32 acq32_getDOT_Immediate()
// returns Immediate time value, d0..d23, running in d31
{
    return _getDOT( Q32_DOT_IMMED_HI, Q32_DOT_IMMED_LO );
}

static __inline u32 acq32_getDOT_Latched()
// returns Immediate time value, d0..d23, running in d31
{
    return _getDOT( Q32_DOT_LATCH_HI, Q32_DOT_LATCH_LO );
}

static volatile __inline unsigned* _mbox( int imbox )
{
    return ((unsigned*)(CSR_BASE+MAILBOX_0)) + imbox;
} 




#endif     // __ACQ32_HML_H__


