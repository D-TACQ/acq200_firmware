/*****************************************************************************
 *
 * File: ACQ32PCI LCA REG Defs
 *
 * $RCSfile: REGDefs.h,v $
 * 
 * Copyright (C) 1999 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: REGDefs.h,v 1.38.4.2 2003/10/27 20:38:36 pgm Exp $
 * $Log: REGDefs.h,v $
 * Revision 1.38.4.2  2003/10/27 20:38:36  pgm
 * reads status from driver
 *
 * Revision 1.38.4.1  2003/08/29 17:41:02  pgm
 * init pci command handling
 *
 * Revision 1.38  2002/07/24 18:07:41  pgm
 * acq16 trigger offset comp - use 1st TW
 *
 * Revision 1.37  2002/06/09 15:57:56  pgm
 * from handa
 *
 * Revision 1.36  2002/04/25 10:09:30  pgm
 * B1634 fixes master trig sync
 *
 * Revision 1.35  2002/04/24 12:51:28  pgm
 * B1633 TRMAS for acq16
 *
 * Revision 1.34  2002/03/28 15:29:42  pgm
 * cycle reset, debounce opts
 *
 * Revision 1.33  2002/03/21 11:54:51  pgm
 * untabify
 *
 * Revision 1.32  2002/03/14 12:03:03  pgm
 * compiled on benbecula
 *
 * Revision 1.31  2002/02/08 22:41:24  pgm
 * multi num channel configs for acq16
 *
 * Revision 1.30  2001/11/04 20:57:54  pgm
 * HTRIGEN included
 *
 * Revision 1.29  2001/10/20 19:42:58  pgm
 * intclock master/slave WORKS
 *
 * Revision 1.28  2001/10/06 21:03:07  pgm
 * CPEM with PXI trig works, AI overflow to cope with
 *
 * Revision 1.27  2001/08/31 09:10:20  pgm
 * acq16 has channel mask, works on rearm
 *
 * Revision 1.26  2001/08/27 15:15:40  pgm
 * acq16 - uses lut for nsamples slightly faster
 *
 * Revision 1.25  2001/08/27 12:55:18  pgm
 * acq16 runs again - needs LCA bit change, caveat embedded trigger
 *
 * Revision 1.24  2001/08/18 20:00:52  pgm
 * updates route correctly, we get data
 *
 * Revision 1.23  2001/08/10 20:47:58  pgm
 * fix lll counter overflow, add USER LEDS
 *
 * Revision 1.22  2001/07/13 16:56:26  pgm
 * AO+DO works at last
 *
 * Revision 1.21  2001/07/11 21:17:58  pgm
 * AI fifo==32, should fix AO load probs
 *
 * Revision 1.20  2001/06/27 22:49:41  pgm
 * better status handling, acq16 deeper fifo
 *
 * Revision 1.19  2001/06/13 20:44:29  pgm
 * lca-ident, fix pre- channel swap bug
 *
 * Revision 1.18  2001/06/08 20:02:01  pgm
 * GUT builds, fifo_sink testing
 *
 * Revision 1.17  2001/06/02 19:58:33  pgm
 * *** empty log message ***
 *
 * Revision 1.16  2001/05/19 19:44:40  pgm
 * LL work in progress
 *
 * Revision 1.15  2001/05/19 12:59:56  pgm
 * low latency setup - done
 *
 * Revision 1.14  2001/05/19 10:30:04  pgm
 * low latency compiles
 *
 * Revision 1.13  2001/05/15 08:50:32  pgm
 * even task handling, slicker status, err detect
 *
 * Revision 1.12  2001/05/04 20:37:07  pgm
 * big upgrade to improve qwual, match spec
 *
 * Revision 1.11  2001/04/20 19:38:12  pgm
 * 66/44 internal clock
 *
 * Revision 1.10  2001/04/14 20:37:50  pgm
 * CPCI routing coded, needs testing
 *
 * Revision 1.9  2001/04/14 19:06:13  pgm
 * remvoe WORTODOS - new LCA is GO
 *
 * Revision 1.8  2001/04/13 20:05:20  pgm
 * new LCA - compiled, not tested, must check WORKTODOS
 *
 * Revision 1.7  2001/04/13 18:43:39  pgm
 * First Update LCA rev 2
 *
 * Revision 1.6  2000/12/29 22:28:43  pgm
 * better diags, correct int clock, opt1
 *
 * Revision 1.5  2000/06/05 21:33:19  pgm
 * SC1 driver stubs filled
 *
 * Revision 1.4  1999/11/14 18:47:20  pgm
 * update
 *
 * Revision 1.2  1999/11/12 18:11:26  pgm
 * default mbox init, LEDS
 *
 * Revision 1.1  1999/11/12 15:21:35  pgm
 * regdefs from Des Spec R1 "better late than never"
 *
 * Revision 1.1  1999/10/22 16:26:49  pgm
 * first entry to cvs
 *
 *
\*****************************************************************************/

#ifndef _REGDEFS_H_
#define _REGDEFS_H_

#include "local.h"


#include "platform.h"
#undef CSR_BASE
extern unsigned csr_buffer[];
#define CSR_BASE (unsigned)csr_buffer

typedef volatile u32 R32;
typedef volatile u16 R16;
typedef volatile u8  R8;

/*
 * 20010413 - Update to new LCA Spec Rev ??.??
 *
 * standard for Reg Defs:
 * - reg address is max 16 chars
 * - bit fields are prefixed with reg address
 * - prefix Q32 for ACQ32
 * - prefix Q16 for ACQ16
 * - prefic C32 for ACQ32CPCI specific
 */

//      123456789012345689
#ifdef PGMCOMOUT
#define Q32_CONFIG               ((R16*)0x40011000)
#define Q32_BUILD_LEVEL          ((R16*)0x40011004) 
#define Q32_BRD_ID_CTRL          ((R16*)0x40011008)
#define Q32_BRD_ID_DATA          ((R16*)0x4001100C)
#define Q32_DATA_FIFO_CTRL       ((R16*)0x40011010)
#define Q32_DATA_FIFO_STAT       ((R16*)0x40011014)
#define Q32_FUNC_CTRL            ((R16*)0x40011018)
#define Q32_DI_EMBED_MASK        ((R16*)0x4001101C)
#define Q32_INT_CLK_CTRL         ((R16*)0x40011020)
#define Q32_INT_CLK              ((R16*)0x40011024)
#define Q32_AI_CAL               ((R16*)0x40011028)
#define Q32_AO_CAL               ((R16*)0x4001102C)
#define Q32_DIO_DIR              ((R16*)0x40011030)
#define Q32_DIO_DATA             ((R16*)0x40011034)
#define Q32_DIO_AI_SFR           ((R16*)0x40011038)
#define Q32_DIO_AO_SFR           ((R16*)0x4001103C)
#define Q32_DIO_DO_SFR           ((R16*)0x40011040)
#define Q32_DOT_IMMED_LO         ((R16*)0x40011044)
#define Q32_DOT_IMMED_HI         ((R16*)0x40011048)
#define Q32_DOT_LATCH_LO         ((R16*)0x4001104c)
#define Q32_DOT_LATCH_HI         ((R16*)0x40011050)
#else
extern R16* regs[];
#define Q32_CONFIG               regs[0]
#define Q32_BUILD_LEVEL          regs[1]
#define Q32_BRD_ID_CTRL          regs[2]
#define Q32_BRD_ID_DATA          regs[3]
#define Q32_DATA_FIFO_CTRL       regs[4]
#define Q32_DATA_FIFO_STAT       regs[5]
#define Q32_FUNC_CTRL            regs[6]
#define Q32_DI_EMBED_MASK        regs[7]
#define Q32_INT_CLK_CTRL         regs[8]
#define Q32_INT_CLK              regs[9]
#define Q32_AI_CAL               regs[10]
#define Q32_AO_CAL               regs[11]
#define Q32_DIO_DIR              regs[12]
#define Q32_DIO_DATA             regs[13]
#define Q32_DIO_AI_SFR           regs[14]
#define Q32_DIO_AO_SFR           regs[15]
#define Q32_DIO_DO_SFR           regs[16]
#define Q32_DOT_IMMED_LO         regs[17]
#define Q32_DOT_IMMED_HI         regs[18]
#define Q32_DOT_LATCH_LO         regs[19]
#define Q32_DOT_LATCH_HI         regs[20]
#endif
// fields for Q32_CONFIG

#define Q32_CONFIG_CONFI        0x0007  // R; reserved
#define Q32_CONFIG_IS_200KHz    0x0008  // R: 200Khz convertors fitted
#define Q32_CONFIG_RS232ON      0x0010  // RW: power the RS232
#define Q32_CONFIG_RXIGNORE         0x0020      // RW: 1: RX on
#define Q32_CONFIG_RXVALID          0x0040  // R: device connected
#define Q32_CONFIG_LED1         0x0080  // RW 1: RED LED on
#define Q32_CONFIG_LED2         0x0100  // RW 1: GREEN LED on
#define Q32_CONFIG_LED3             0x0200      // RW 1: CPCI LED on
#define Q32_CONFIG_LED4         0x0400  // RW 1: CPCI LED on

// acq16 encodes Q32_CONFIG_CONFI RW:
#define Q32_CONFIG_Q16_12       0x0
#define Q32_CONFIG_Q16_08       0x1
#define Q32_CONFIG_Q16_16       0x2
#define Q32_CONFIG_Q16_04       0x3
#define Q32_CONFIG_Q16_02       0x7

// fields for Q32_BRD_ID_CTRL

#define Q32_BRD_ID_CTRL_INIT    0x0001  // HOWTO USE ??
#define Q32_BRD_ID_CTRL_READ    0x0002
#define Q32_BRD_ID_CTRL_BUSY    0x0004


// fields for Q32_DATA_FIFO_CTRL

#define Q32_DATA_FIFO_CTRL_AIFIFOEN    0x0001
#define Q32_DATA_FIFO_CTRL_AIFIFORES   0x0002

#define Q32_DATA_FIFO_CTRL_AOFIFOEN    0       // obsolete function
#define Q32_DATA_FIFO_CTRL_AOFIFORES   0x0008

#define Q32_DATA_FIFO_CTRL_DOFIFOEN    0        // obsolete function
#define Q32_DATA_FIFO_CTRL_DOFIFOWREN  0x0010
#define Q32_DATA_FIFO_CTRL_DOFIFORES   0x0020

// fields for Q32_DATA_FIFO_STAT

#define Q32_DATA_FIFO_STAT_AIFIFOPTR   0x001f
#define Q32_DATA_FIFO_STAT_AOFIFOPTR   0x0f00
#define Q32_DATA_FIFO_STAT_DOFIFOPTR   0xf000
#define Q32_DATA_FIFO_STAT_AIFIFOFF    0x0020
#define Q32_DATA_FIFO_STAT_AOFIFOEF    0x0040
#define Q32_DATA_FIFO_STAT_DOFIFOEF    0x0080
#define Q32_DATA_FIFO_STAT_FLAGS \
 (Q32_DATA_FIFO_STAT_AIFIFOFF+Q32_DATA_FIFO_STAT_AOFIFOEF+Q32_DATA_FIFO_STAT_DOFIFOEF)

#define Q32_DATA_FIFO_STAT_AIHALF      0x0010

#define Q32_GET_AI_FIFOSAMPLES(fsw)    ((fsw)&Q32_DATA_FIFO_STAT_AIFIFOPTR)
#define Q32_GET_AO_FIFOSAMPLES(fsw)    (((fsw)&Q32_DATA_FIFO_STAT_AOFIFOPTR)>>8)
#define Q32_GET_DO_FIFOSAMPLES(fsw)    (((fsw)&Q32_DATA_FIFO_STAT_DOFIFOPTR)>>12)

// old style def - this should change
//#define Q16_DATA_FIFO_STAT_AIFIFOFF    0x1000
#define Q16_DATA_FIFO_STAT_AIFIFOFF     Q32_DATA_FIFO_STAT_AIFIFOFF

#define Q16_DATA_FIFO_STAT_AIFIFOPTR   0x001f

#define Q16_AI_FIFO_BITS( fsw )        ((fsw)&Q16_DATA_FIFO_STAT_AIFIFOPTR)

// fields for Q32_FUNC_CTRL

#define Q32_FUNC_CTRL_AIEN             0x0001 // RW Enable AI
#define Q32_FUNC_CTRL_AIC_EXT          0x0002 // RW 1=> select ext clock
#define Q32_FUNC_CTRL_AISTRIG_HI       0x0004 // RW 1=> soft trig TTL HI
#define Q32_FUNC_CTRL_SOFTCLK_HI       0x0008 // RW 1=> CLock at TTL Hi
#define Q32_FUNC_CTRL_SCLKSYNC         0x0010 // RW sync to DO clock
#define Q32_FUNC_CTRL_CHANNEL_ID_SIM   0x0020 // RW 

#define Q16_FUNC_CTRL_NEMBED_TRIG      0x0040 // RW 1=> disable embedded trigge
#define Q16_FUNC_CTRL_SETCLKDEB        0x0080 // RW 1=> debounc trigger input
#define Q32_FUNC_CTRL_AOEN             0x0100 // RW Enable AO
#define Q32_FUNC_CTRL_AOC_EXT          0x0200 // RW 1=>select ext clock
#define Q32_FUNC_CTRL_AOSTRIG_HI       0x0400 // RW 1=> sof trig TTL HI
#define Q32_FUNC_CTRL_NDAC_RESET       0x0800 // RW 1=> lift DACS from reset
#define Q32_FUNC_CTRL_DOEN             0x1000 // RW Enable DO
#define Q32_FUNC_CTRL_DOC_EXT          0x2000 // RW 1=> select ext clock
#define Q32_FUNC_CTRL_DOSTRIG_HI       0x4000 // RW 1=> soft trig at TTL 1
#define Q32_FUNC_CTRL_DO_WAVE_MODE     0x8000 // RW 1=> select waveform output    

// fields for Q32_DI_EMBED_MASK

#define Q32_DI_EMBED_MASK_MASK         0x003f // RW 1=>look for events on these bits#


// fields for Q32_INT_CLK_CTRL

#define Q32_INT_CLK_CTRL_CLKSEL_MASK   0x0007 // RW mask the clocks sel bits

enum CLKSEL {
    INT_CLKSEL_DO0,
    INT_CLKSEL_DO1,
    INT_CLKSEL_DO2,
    INT_CLKSEL_DO3,
    INT_CLKSEL_DO4,
    INT_CLKSEL_DO5,
    INT_CLKSEL_DO15,
    INT_CLKSEL_AISOFT
};

#define Q32_INT_CLK_CTRL_CLKMAS        0x0008 // RW Enable Clock mastering
#define Q32_INT_CLK_CTRL_CLKSRC_MASK   0x0070 // RW Clock divider DI source

enum INT_CLKSRC {
    INT_CLKSRC_DI0,
    INT_CLKSRC_DI1,
    INT_CLKSRC_DI2,
    INT_CLKSRC_DI3,
    INT_CLKSRC_DI4,
    INT_CLKSRC_DI5,
    INT_CLKSRC_INT_44MHZ,
    INT_CLKSRC_INT_66MHZ,
    INT_CLKSRC_MASK = 0x7
};
#define MK_CLKSRC( clksrc ) ((clksrc)<<4)


#define Q32_INT_CLK_CTRL_EXTCLK        0x0080 // RW select ext clock
#define Q32_INT_CLK_CTRL_GASYNC        0x0100 // RW slect LLC ext trig sync
#define Q32_INT_CLK_CTRL_GASFTCLK      0x0200 // RW 1:=> output soft clock,  0:int

// fields for Q32_INTCLK 

// fields for Q32_AI_CAL

enum CALDAC {
        DAC0 = 0x0001,
        DAC1,
        DAC2,
        DAC3,
        DAC4,
        DAC5,
        DAC6,
        DAC7
};

#define Q32_AI_CAL_CALDATA                 0x03ff
#define Q32_AI_CAL_DACADDR             0x3c00
#define Q32_AI_CAL_DACADDR_SHIFT           10
#define Q32_AI_CAL_HSHAKE                  0x4000
#define Q32_AI_CAL_UPDATE                  0x8000       // 0: Load, 1: Update




// fields for Q32_AO_CAL

#define Q32_AO_CAL_CAL    0x0001        // 0: normal 1: cal
#define Q32_AO_CAL_UP     0x0002        // 0: down, 1: up
#define Q32_AO_CAL_INC    0x0004        // 0: stick, 1:move


// fields for Q32_DIO_DIR

#define Q32_DIO_DIR_SET_INPUTS( mask )  (*Q32_DIO_DIR &= ~(mask))
#define Q32_DIO_DIR_SET_OUTPUTS( mask ) (*Q32_DIO_DIR |= (mask))

#define Q32_DIO_WAVEFORM_BITS    ((0xff)<<6)

// fields for Q32_DIO_DATA

#define Q32_DIO_DATA_CLK      0x0001  
//#define Q32_DIO_DATA_GATE     0x0002    // was supposed to be thus

#define Q32_DIO_DATA_GATE_SRC    xx_SFR_CLKSRC_DI2
#define Q32_DIO_DATA_GATE        (1<<Q32_DIO_DATA_GATE_SRC)

// fields for Q32_DIO_AI_SFR
// fields for Q32_DIO_AO_SFR

enum xx_SFR_CLKSRC {
    xx_SFR_CLKSRC_DI0,
    xx_SFR_CLKSRC_DI1,
    xx_SFR_CLKSRC_DI2,
    xx_SFR_CLKSRC_DI3,
    xx_SFR_CLKSRC_DI4,
    xx_SFR_CLKSRC_DI5,
    xx_SFR_CLKSRC_SOFT,
    xx_SFR_CLKSRC_SOFT2,
    xx_SFR_CLKSRC_MASK = 0x7
};

enum xx_SFR_TRSRC {
    xx_SFR_TRSRC_DI0,
    xx_SFR_TRSRC_DI1,
    xx_SFR_TRSRC_DI2,
    xx_SFR_TRSRC_DI3,
    xx_SFR_TRSRC_DI4,
    xx_SFR_TRSRC_DI5,
    xx_SFR_TRSRC_DI6,
    xx_SFR_TRSRC_DI7,
    xx_SFR_TRSRC_MASK = 0x7
};



#define Q32_DIO_xx_SFR_CLKSRC( clk )   ((clk)&0x7)      // RW clk 0..7
#define Q32_DIO_xx_SFR_CLKSRC_MASK     Q32_DIO_xx_SFR_CLKSRC(xx_SFR_CLKSRC_MASK)
#define Q32_DIO_xx_SFR_CLKPOL_RISING   0x0008       // RW 0: falling edge, 1: rising 
#define Q32_DIO_xx_SFR_CLKVAL          0x0010       // RO current value

/* trigger definition (AO, DO only ) */

#define Q32_DIO_xx_SFR_TRSRC(trig)         (((trig)&0x7)<<8) // RW trig 0..7
#define Q32_DIO_xx_SFR_TRSRC_MASK      Q32_DIO_xx_SFR_TRSRC(xx_SFR_TRSRC_MASK)
#define Q32_DIO_xx_SFR_TRPOL_RISING    0x0800       //  RW 0: falling 1:frsing
#define Q32_DIO_xx_SFR_TRVAL           0x1000       // RO current value
#define Q32_DIO_xx_SFR_HTRIGEN         0x2000       // RW Detect Hard Trigger

/* for acq16, xx is AI, and the trigger bits have different semantics */
/* this func may become available on acq32 as well ?? */

#define Q16_DIO_AI_SFR_TRMSRC(trig)   Q32_DIO_xx_SFR_TRSRC(trig)
#define Q16_DIO_AI_SFR_TRMAS          0x0800


/*
 * nominal DIO useage:
 */
 
#define DEF_AICLK        0x01
#define DEF_AITRIG       0x02
#define DEF_AOCLK        0x04
#define DEF_AOTRIG       0x08
#define DEF_DOCLK        0x10
#define DEF_DOTRIG       0x20


/*
 * Do Time Counter Bits
 */
 
#define Q32_DOT_xxxx_HI_RUN    0x8000U
#define Q32_DOT_xxxx_HI_COUNT  0x00ffU

#ifdef ACQ200COMOUT
/*
 * ACQ32CPCI
 */
 
#define Q32C_ROUTESEL                    ((R32*)0x40012200)

// ROUTESEL[0] .. ROUTESEL[4] are valid

#define Q32C_ROUTEDEBUG                  ((R32*)0x40012214)

#endif










#ifdef ACQ200COMOUT
/*
 * Signal Conditioning SC1
 */


#define SC1_B1_ID          (*(R16*)0x40012000)
#define SC1_B1_CCR1        (*(R16*)0x40012004) 
#define SC1_B1_CCR2        (*(R16*)0x40012008)
/* ix: {0..15} */
#define SC1_B1_CCRX( ix )   ((R16*)0x4001200C)[ix*2]
/* ix: {0..3}  */ 
#define SC1_B1_RCRX( ix )   ((R16*)0x4001202C)[ix*2] 
#define SC1_B1_DIODIR      (*(R16*)0x4001203c)

#define SC1_B2_ID          (*(R16*)0x40012040)
#define SC1_B2_CCR1        (*(R16*)0x40012044) 
#define SC1_B2_CCR2        (*(R16*)0x40012048)
#define SC1_B2_CCRX( ix )   ((R16*)0x4001204C)[ix*2] 
#define SC1_B2_RCRX( ix )   ((R16*)0x4001206C)[ix*2] 
#define SC1_B2_DIODIR      (*(R16*)0x4001207c)

#endif

#endif                // _REGDEFS_H_
