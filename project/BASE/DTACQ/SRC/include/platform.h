
/* DO NOT EDIT!! - this file automatically generated
 *                 from .s file by awk -f s2h.awk
 */
/***********************************************************************
 * * Copyright © Intel Corporation 1998.  All rights reserved.
 * ************************************************************************
 * 
 *   EBSA285 address map
 * 
 *        NOTE: This is a multi-hosted header file for use with uHAL and
 *              supported debuggers.
 * 
 *        $Id: platform.h,v 1.8 2002/03/21 11:55:44 pgm Exp $
 * 
 * ***********************************************************************/

#ifndef __address_h
#define __address_h                     1

#define PLATFORM_ID                     0x00000008

/*  Common modules for uHAL can be included or excluded by changing these
 *  definitions. These can be over-ridden by the makefile/ARM project file
 *  provided the .h file can is rebuilt.
 */
#ifndef uHAL_BOOT
#define uHAL_BOOT                       1
#endif
#ifndef uHAL_HEAP
#define uHAL_HEAP_BASE                  (PCI_DRAMBASE - 0x300000)
#define uHAL_HEAP_SIZE                  0x200000        
#define uHAL_HEAP                       1
#endif
#ifndef uHAL_TIMERS
#define uHAL_TIMERS                     1
#endif
#ifndef uHAL_INTERRUPTS
#define uHAL_INTERRUPTS                 1
#endif
#ifndef uHAL_COMPLEX_IRQ
#define uHAL_COMPLEX_IRQ                0
#endif
#ifndef uHAL_PCI
#define uHAL_PCI                        1
#endif
#ifndef uHAL_VGA
#define uHAL_VGA                        0
#endif
#ifndef uHAL_LCD
#define uHAL_LCD                        0
#endif

/* handy sizes */
#define SZ_1K                           0x00000400
#define SZ_16K                          0x00004000
#define SZ_64K                          0x00010000
#define SZ_256K                         0x00040000
#define SZ_512K                         0x00080000

#define SZ_1M                           0x00100000
#define SZ_2M                           0x00200000
#define SZ_3M                           0x00300000
#define SZ_4M                           0x00400000
#define SZ_8M                           0x00800000
#define SZ_16M                          0x01000000
#define SZ_32M                          0x02000000
#define SZ_64M                          0x04000000
#define SZ_96M                          0x09000000
#define SZ_128M                         0x08000000
#define SZ_256M                         0x10000000

#define SZ_1G                           0x40000000

#ifdef PGMCOMOUT
/* DRAM space */
#define DRAM_BASE                       0x00000000
#define DRAM_DEFAULT                    SZ_16M
#define PCI_DRAMSIZE                    SZ_2M
#define PCI_DRAMBASE                    (DRAM_DEFAULT - PCI_DRAMSIZE)
#else

// WORKTODO !!!

#define DRAM_BASE                       0x00000000
#define DRAM_DEFAULT                    SZ_4M
/*  It's rather unfortunate, but all our SDRAM must be visible over PCI
 *  so we really have 4 MB visible, however for memory management we need
 *  to define the right numbers
 */
#define PCI_DRAMSIZE                    SZ_3M
#define PCI_DRAMBASE                    (DRAM_DEFAULT - PCI_DRAMSIZE)
/* Put the LCA high up in the Memory Map, above possible SDRAM SODIMM Modules */
#define LCA_DRAMSIZE                    DRAM_DEFAULT
#define LCA_DRAMBASE                    SZ_96M
#endif
/* DC21285 Addresses */

#define DC21285_DRAM_A0MR               0x40000000
#define DC21285_DRAM_A1MR               0x40004000
#define DC21285_DRAM_A2MR               0x40008000
#define DC21285_DRAM_A3MR               0x4000C000
        
#define DC21285_XBUS_XCS0               0x40010000
#define DC21285_XBUS_XCS1               0x40011000
#define DC21285_XBUS_XCS2               0x40012000
#define DC21285_XBUS_NOCS               0x40014000

#define DC21285_FLASH                   0x41000000
/* FLASH base */
#define FLASH_BASE                      DC21285_FLASH
#define FLASH_SIZE                      SZ_1M
#define FLASH_END                       (FLASH_BASE + FLASH_SIZE - 1)
/* ROM is really FLASH on EBSA-285, but you can call it ROM if you want.. */
#define DC21285_ROM                     DC21285_FLASH
/* ROM base */
#define EPROM_BASE                      DC21285_FLASH
#define EPROM_SIZE                      FLASH_SIZE
#define EPROM_END                       (EPROM_BASE + EPROM_SIZE - 1)
#define ROMTop                          EPROM_END


#define DC21285_ARMCSR_BASE             0x42000000

#define DC21285_SA_CACHE_FLUSH          0x50000000
/* Reserve Virtual Address for cache flush */
#define CLEAN_BASE                      DC21285_SA_CACHE_FLUSH

#define DC21285_OUTBOUND_WRITE_FLUSH    0x78000000

#define DC21285_PCI_IACK                0x79000000
        
#define DC21285_PCI_TYPE_1_CONFIG       0x7A000000

#define DC21285_PCI_TYPE_0_CONFIG       0x7B000000

#define DC21285_PCI_IO                  0x7C000000

#define DC21285_PCI_MEMORY              0x80000000

#define PCI_IO                          DC21285_PCI_IO
#define PCI_MEM                         DC21285_PCI_MEMORY

/*  macro to standardise conversion from ISA (X bus) to sparse address..
 *  and some to standardise conversion from mapped memory to PCI
 */
#define _MapAddress(a)         (((unsigned int) a << 2) | DC21285_XBUS_XCS1)
#define _MapIOAddress(a)       ((U32)PCI_IO + (U32)(a))
#define _MapMemAddress(a)      ((U32)PCI_MEM + (U32)(a))
        

#define COM1Port                        0x3f8
#define COM2Port                        0x2f8           
#define IOShift                         2

/* Serial port constants */
#define XBUS_DEV_0                      DC21285_XBUS_XCS0
#define XBUS_DEV_1                      DC21285_XBUS_XCS1
#define XBUS_DEV_2                      DC21285_XBUS_XCS2
#define XBUS_DEV_3                      DC21285_XBUS_NOCS
/* Should be debug only, use COM1Port not COM1 */
#define COM1                            (XBUS_DEV_1 + (COM1Port << IOShift))
#define COM2                            (XBUS_DEV_1 + (COM2Port << IOShift))

/* LED definitions */
#define SA_LED_ON                       0
#define SA_LED_OFF                      1

#define LED_BANK                        XBUS_DEV_2
#define RED_LED                         0x04
#define GREEN_LED                       0x02
#define YELLOW_LED                      0x01
#define ALL_LEDS                        0x07
#define SA_NUM_OF_LEDS                  3
#define SA_LED_MASKS   { 0, YELLOW_LED, GREEN_LED, RED_LED }
#define SA_LED_OFFSETS { 0, (void *)LED_BANK, (void *)LED_BANK, (void *)LED_BANK }
#ifndef HIGH
#define HIGH                            1
#endif


#define CSR_BASE                        DC21285_ARMCSR_BASE

#define PCI_COMMAND                     0x04
#define PCI_MEM_BAR                     0x10
#define PCI_IO_BAR                      0x14
#define PCI_DRAM_BAR                    0x18
#define PCI_OUT_INT_STATUS              0x30
#define PCI_OUT_INT_MASK                0x34
#define PCI_INT_LINE                    0x3C
#define PCI_I2O_INBOUND_FIFO            0x40
#define PCI_I2O_OUTBOUND_FIFO           0x44
#define MAILBOX_0                       0x50
#define MAILBOX_1                       0x54
#define MAILBOX_2                       0x58
#define MAILBOX_3                       0x5C
#define DOORBELL                        0x60
#define DOORBELL_SETUP                  0x64
#define ROM_BYTE_WRITE                  0x68
#define CHAN_1_BYTE_COUNT               0x80
#define CHAN_1_PCI_ADDR                 0x84
#define CHAN_1_DRAM_ADDR                0x88
#define CHAN_1_DESC_PTR                 0x8C
#define CHAN_1_CONTROL                  0x90
#define CHAN_2_BYTE_COUNT               0xA0
#define CHAN_2_PCI_ADDR                 0xA4
#define CHAN_2_DRAM_ADDR                0xA8
#define CHAN_2_DESC_PTR                 0xAC
#define CHAN_2_CONTROL                  0xB0
#define DRAM_BASE_ADDR_MASK             0x100
#define DRAM_BASE_ADDR_OFF              0x104
#define ROM_BASE_ADDR_MASK              0x108
#define DRAM_TIMING                     0x10C
#define DRAM_ADDR_SIZE_0                0x110
#define DRAM_ADDR_SIZE_1                0x114
#define DRAM_ADDR_SIZE_2                0x118
#define DRAM_ADDR_SIZE_3                0x11C
#define I2O_IFH                         0x120
#define I2O_IPT                         0x124
#define I2O_OPH                         0x128
#define I2O_OFT                         0x12C
#define I2O_IFC                         0x130
#define I2O_OPC                         0x134
#define I2O_IPC                         0x138
#define SA_CONTROL                      0x13C
#define PCI_ADDR_EXT                    0x140
#define PREFETCH_RANGE                  0x144
#define XBUS_CYCLE                      0x148
#define XBUS_STROBE                     0x14C
#define DBELL_PCI_MASK                  0x150
#define DBELL_SA_MASK                   0x154
#define UARTDR                          0x160
#define UMSEOI                          0x164
#define RXSTAT                          0x164 
#define H_UBRLCR                        0x168 
#define M_UBRLCR                        0x16C 
#define L_UBRLCR                        0x170 
#define UARTCON                         0x174 
#define UARTFLG                         0x178 
#define IRQ_STATUS                      0x180 
#define IRQ_RAW_STATUS                  0x184 
#define IRQ_ENABLE                      0x188 
#define IRQ_ENABLE_SET                  0x188 
#define IRQ_ENABLE_CLEAR                0x18C 
#define IRQ_SOFT                        0x190 
#define FIQ_STATUS                      0x280 
#define FIQ_RAW_STATUS                  0x284 
#define FIQ_ENABLE                      0x288 
#define FIQ_ENABLE_SET                  0x288 
#define FIQ_ENABLE_CLEAR                0x28C 
#define FIQ_SOFT                        0x290 
#define TIMER_1_LOAD                    0x300 
#define TIMER_1_VALUE                   0x304 
#define TIMER_1_CONTROL                 0x308 
#define TIMER_1_CLEAR                   0x30C 

#define TIMER_2_LOAD                    0x320 
#define TIMER_2_VALUE                   0x324 
#define TIMER_2_CONTROL                 0x328 
#define TIMER_2_CLEAR                   0x32C 

#define TIMER_3_LOAD                    0x340 
#define TIMER_3_VALUE                   0x344 
#define TIMER_3_CONTROL                 0x348 
#define TIMER_3_CLEAR                   0x34C 

#define TIMER_4_LOAD                    0x360 
#define TIMER_4_VALUE                   0x364 
#define TIMER_4_CONTROL                 0x368 
#define TIMER_4_CLEAR                   0x36C 


/* These are all defined in terms of offsets from DC21285_ARMCSR_BASE */

#define DC21285_ARMCSR_UART_BASE        UARTDR
/* DC21285_UCR                    EQU     DC21285_ARMCSR_BASE+0x160
 * DC21285_UDR                    EQU     DC21285_ARMCSR_BASE+0x164
 * DC21285_USR                    EQU     DC21285_ARMCSR_BASE+0x168
 */
        
#define DC21285_ARMCSR_IRQ_BASE         IRQ_STATUS
/* Offsets from IRQ_BASE, or FIQ_BASE (in words) */
#define INTStatus                       0
#define INTRawStatus                    1
#define INTEnable                       2
#define INTEnableSet                    2
#define INTEnableClear                  3
#define INTSoft                         4

#define DC21285_IRQStatus               DC21285_ARMCSR_BASE+0x180

#define DC21285_ARMCSR_TIMER_BASE       TIMER_1_LOAD
/* Timer enable bit in control register */
#define TIMER_CONTROL_ENABLE            (1 << 7) 

/* 
 *  Now define individual registers in the clumps.
 * 
 * 
 *  AMBA port registers.  These offsets and bit settings are taken from
 *  the AMBA uart (AP13) data sheet.
 * 
 */
#define AMBA_UARTDR                     0x00
#define AMBA_RXSTAT                     0x04
#define AMBA_H_UBRLCR                   0x08
#define AMBA_M_UBRLCR                   0x0C
#define AMBA_L_UBRLCR                   0x10
#define AMBA_UARTCON                    0x14
#define AMBA_UARTFLG                    0x18

#define AMBA_UARTFLG_TFF                0x20
#define AMBA_UARTFLG_RFE                0x10
#define AMBA_UARTFLG_TBY                0x08
#define AMBA_UARTFLG_TMSK               (AMBA_UARTFLG_TFF + AMBA_UARTFLG_TBY)

#define AMBA_UARTCON_I                  0x04
#define AMBA_UARTCON_S                  0x02
#define AMBA_UARTCON_UE                 0x01

#define AMBA_H_UBRLCR_DSS_8             0x60
#define AMBA_H_UBRLCR_DSS_7             0x40
#define AMBA_H_UBRLCR_DSS_6             0x20
#define AMBA_H_UBRLCR_DSS_5             0x00
#define AMBA_H_UBRLCR_EF                0x10
#define AMBA_H_UBRLCR_SBS               0x08
#define AMBA_H_UBRLCR_OES               0x04
#define AMBA_H_UBRLCR_PE                0x02
#define AMBA_H_UBRLCR_BRK               0x01

/* ARM control register constants */
#define INIT_COMPLETE                   1
#define PCI_NRESET                      0x200
#define XCS_2                           0x40000000
#define XCS_1                           0x20000000
#define XCS_0                           0x10000000
#define PCI_CFN                         0x80000000
#define ROM_TIMING                      0x0fff0000
#define ROM_SETUP                       0x04aa0000

/*  Initial PCI Interrupt ID is 1.
 */
#define SETUP_PCI_INT_ID                0x100

/*  These definitions assume that if either of these are defined,
 *  then both are 
 */
#ifdef HANDLE_INTERRUPTS_ON_IRQ
#if HANDLE_INTERRUPTS_ON_IRQ != 0
#define ANGEL_INT_ENABLE                (CSR_BASE + IRQ_ENABLE_SET)
#define ANGEL_INT_DISABLE               (CSR_BASE + IRQ_ENABLE_CLEAR)
#define ANGEL_INT_SOFT                  (CSR_BASE + IRQ_SOFT)
#define ANGEL_INT_STATUS                (CSR_BASE + IRQ_STATUS)
#define ANGEL_INT_RAW                   (CSR_BASE + IRQ_RAW_STATUS)
#endif
#endif
#ifdef HANDLE_INTERRUPTS_ON_FIQ
#if HANDLE_INTERRUPTS_ON_FIQ != 0
#define ANGEL_INT_ENABLE                (CSR_BASE + FIQ_ENABLE_SET)
#define ANGEL_INT_DISABLE               (CSR_BASE + FIQ_ENABLE_CLEAR)
#define ANGEL_INT_SOFT                  (CSR_BASE + FIQ_SOFT)
#define ANGEL_INT_STATUS                (CSR_BASE + FIQ_STATUS)
#define ANGEL_INT_RAW                   (CSR_BASE + FIQ_RAW_STATUS)
#endif
#endif

/*  On the verification board we have two UARTS, on the evaluation board
 *  one AMBA port. When debugging, we may need to access both.
 */
#define XBAUD_115200                    0x001 
#define XBAUD_57600                     0x002 
#define XBAUD_38400                     0x003 
#define XBAUD_19200                     0x006 
#define XBAUD_14400                     0x009 
#define XBAUD_9600                      0x00C 
#define XBAUD_2400                      0x018 
#define XBAUD_1200                      0x060    
#define XDEFAULT_HOST_BAUD              BAUD_9600
#define XDEFAULT_OS_BAUD                BAUD_38400

#define EB285_USE_UARTS                 0

#if EB285_USE_UARTS != 0
#define UARTS                           1
/* Default port to talk to host (via debugger) */
#define HOST_COMPORT                    COM1Port
/* Default port for use by Operating System or program */
#define OS_COMPORT                      COM2Port
#define DEBUG_COMPORT                   OS_COMPORT

/* Values to set given baud rates */
#define BAUD_115200                     XBAUD_115200
#define BAUD_57600                      XBAUD_57600
#define BAUD_38400                      XBAUD_38400
#define BAUD_19200                      XBAUD_19200
#define BAUD_14400                      XBAUD_14400
#define BAUD_9600                       XBAUD_9600
#define BAUD_2400                       XBAUD_2400
#define BAUD_1200                       XBAUD_1200
#define DEFAULT_HOST_BAUD               XDEFAULT_HOST_BAUD
#define DEFAULT_OS_BAUD                 XDEFAULT_OS_BAUD

#define IO_READ(p)             (*(unsigned char *)(_MapAddress(p)))
#define IO_WRITE(p, c)         (*(unsigned char *)(_MapAddress(p)) = c)

#else
#define UARTS                           0
/* Default port to talk to host (via debugger) */
#define HOST_COMPORT                    (CSR_BASE + DC21285_ARMCSR_UART_BASE)
/* Default port for use by Operating System or program */
#define OS_COMPORT                      HOST_COMPORT
/*  WARNING: COM2Port Needs UART daughtercard to work!!
 */
#define DEBUG_COMPORT                   COM2Port

/* Values to set given baud rates for a UART clock frequency of 50 MHz */
#define BAUD_115200                     0x006 
#define BAUD_57600                      0x00D 
#define BAUD_38400                      0x013 
#define BAUD_19200                      0x028 
#define BAUD_14400                      0x03C 
#define BAUD_9600                       0x050 
#define BAUD_2400                       0x145 
#define BAUD_1200                       0x28A    
#define BAUD_110                        0x1BBD
#define DEFAULT_HOST_BAUD               BAUD_9600
#define DEFAULT_OS_BAUD                 DEFAULT_HOST_BAUD

#define GET_STATUS(p)          (inb( (void *) ((p) + AMBA_UARTFLG) ) & 0xFF)
#define GET_CHAR(p)            (inb( (void *) ((p) + AMBA_UARTDR) ))
#define PUT_CHAR(p, c)         (outb(c, (void *)((p) + AMBA_UARTDR)))
#define IO_READ(p)             (AMBA_REG_READ(p))
#define IO_WRITE(p, c)         (AMBA_REG_WRITE(p, c))
#define RX_DATA(s)             (((s) & AMBA_UARTFLG_RFE) == 0)
#define TX_READY(s)            (((s) & AMBA_UARTFLG_TFF) == 0)
#define TX_EMPTY(p)            ((GET_STATUS(p) & AMBA_UARTFLG_TMSK) == 0)

/* Used by Angel for interrupts */
#define READ_INTERRUPT(port)   ( AMBA_REG_READ(ANGEL_INT_STATUS) )
/* Enable & disable write the bit to different addresses */
#define ENABLE_INT(v, b, p)    ( AMBA_REG_WRITE(ANGEL_INT_ENABLE, b) )
#define DISABLE_INT(v, b, p)   ( AMBA_REG_WRITE(ANGEL_INT_DISABLE, b) )
#define ENABLE_INTERRUPT(p)    /* Do nothing */
#define NO_INTERRUPT                    0x00
#define TX_INTERRUPT                    0x08
#define RX_INTERRUPT                    0x04

#define TX_ENABLE                       TX_INTERRUPT
#define RX_ENABLE                       RX_INTERRUPT

#endif

#define AMBA_REG_READ(r)       *((unsigned int *)(r))
#define AMBA_REG_WRITE(r, v)   *(unsigned int *)((unsigned int)(r)) = v

/* Keep aliases for XBus UART */
#define XIO_READ(p)            (*(unsigned char *)(_MapAddress(p)))
#define XIO_WRITE(p, c)                (*(unsigned char *)(_MapAddress(p)) = c)

#define Baud1200                        XBAUD_1200
#define Baud2400                        XBAUD_2400
#define Baud9600                        XBAUD_9600
#define Baud19200                       XBAUD_19200
#define Baud38400                       XBAUD_38400
#define Baud57600                       XBAUD_57600
#define Baud115200                      XBAUD_115200

/*  Purely for compatability with ANGEL (defined in uart.h in uHAL):
 */
#define InterruptId                     2
/*  Purely for compatability with uHAL (defined in superio_h.s in ANGEL):
 */
#define EnableInt                       0x04
#define CntlModem                       0x10

#ifdef LOGTERM_DEBUGGING
/*  For Angel only, need to include this header file in platform.h
 */
#include        "angel285.h"
#endif

/*  timer stuff:
 *  On EB285 there is an external clock which ticks at 3.68Mhz. This is wired
 *  into the DC21285 chip's timer #3. There are 24 bits of counter that we can
 *  set to divide this external timer down. Each external tick is 3.68*(10**-6)
 *  seconds long, 1mSEC needs counter of 3680 (every 3680 external ticks, we
 *  get one clock interrupt).
 */

#define OS_TIMER                        3
#define MAX_TIMER                       4
/*  If DEMON were ever to be ported to the EBSA-285, or ANGEL needed a timer,
 *  it would use MAX_TIMER.
 */
#define HOST_TIMER                      MAX_TIMER

#define mSEC_1                          3680    
#define mSEC_5                          (mSEC_1 * 5)
#define mSEC_10                         (mSEC_1 * 10)
#define SEC_1                           (mSEC_1 * 1000)

#define MAX_PERIOD                      0xFFFF          

/*  Timer definitions.            
 *  The irq numbers of the individual timers
 */
#define TIMER_VECTORS  { 0, 4, 5, 6, 7 }

/*JWM Start - Change this for ACQ32 needs 
 xcs<0> is for LCA Configuration
 xcs<1> is the main LCA register area
 xcs<2> is the off board Digital I/O expansion
 !!!!WORKTODO the default code uses xcs<2> area for the LED flashes - REMOVE it
*/ 
#define INIT_XBUS_CYCLE                 0x10001492
#define INIT_XBUS_STROBE                0xf8f8f8f8

/*  xcs<2> = output (softIO, leds and jumpers), xcs<1> = output (SuperIO), 
 *  xcs<0> = input (interrupt)
 */
/* #define XBUS_INIT                       (XCS_2 | XCS_1) */
/*  Note: This is a bodge to get around an assembler limitation. XBUS_BITS
 *  should really be:
 * XBUS_BITS              EQU     (XCS_2 | XCS_1 | XCS_0)
 *  .. but the assembler complains!
 */
/* change this to something simpler */
#define XBUS_BITS                       EQU     0x70000000   /* XCS_2 | XCS_1 | XCS_0 */
#define SA_CTL_INIT                     (XBUS_BITS | ROM_SETUP)
#define SA_CTL_COMPLETE                 (INIT_COMPLETE | PCI_NRESET)

/*  PCI Command Register
 */
#define PCI_IO_ENABLE                   0x0001
#define PCI_MEM_ENABLE                  0x0002
#define PCI_MASTER_ENABLE               0x0004
#define PCI_MEM_WRINVALID               0x0010
#define PCI_FAST_B2B                    0x0200
/*  Another workaround the Assembler limitation. PCI_CFN_INIT should be:
 * PCI_CFN_INIT           EQU     (PCI_IO_ENABLE | PCI_MEM_ENABLE | PCI_MASTER_ENABLE | PCI_MEM_WRINVALID)
 */
#define PCI_CFN_ENABLE                  (PCI_IO_ENABLE | PCI_MEM_ENABLE)
#define PCI_MAS_ENABLE                  (PCI_MEM_WRINVALID | PCI_MASTER_ENABLE)
#define PCI_CFN_INIT                    (PCI_CFN_ENABLE | PCI_MAS_ENABLE)

/*  PCI Outbound Interrupt Status/Mask
 */
#define PCI_OUT_DOORBELL                0x04
#define PCI_OUT_POSTLIST                0x08
#define NO_OUTBOUND_INTS                (PCI_OUT_DOORBELL | PCI_OUT_POSTLIST)

/*  21285 puts it's DRAM memory in this Base Address Register
 */
#define HOST_MEMBAR                     2        /*  Standard nomenclature */


/*  The following are used in the startup code for the EB285. There are two
 *  DIMMs of RAM on the EB285, and there may be DRAM in  either block. The
 *  DRAM is initialised by the startup code.
 */
#define LowRAMBase                      0

#define MAX_SDRAM                       4

/* See 21285 Page 7-31 of data sheet: */
#define DC21285_Trp1                    0x0
#define DC21285_Trp2                    0x1
#define DC21285_Trp3                    0x2
#define DC21285_Trp4                    0x3
#define DC21285_Tdal2                   (0x0 << 2)
#define DC21285_Tdal3                   (0x1 << 2)
#define DC21285_Tdal4                   (0x2 << 2)
#define DC21285_Tdal5                   (0x3 << 2)
#define DC21285_Trcd2                   (0x2 << 4)
#define DC21285_Trcd3                   (0x3 << 4)
#define DC21285_Tcas2                   (0x2 << 6)
#define DC21285_Tcas3                   (0x3 << 6)
#define DC21285_Trc4                    (0x1 << 8)
#define DC21285_Trc5                    (0x2 << 8)
#define DC21285_Trc6                    (0x3 << 8)
#define DC21285_Trc7                    (0x4 << 8)
#define DC21285_Trc8                    (0x5 << 8)
#define DC21285_Trc9                    (0x6 << 8)
#define DC21285_Trc10                   (0x7 << 8)
#define DC21285_Tcd0                    (0x0 << 11)
#define DC21285_Tcd1                    (0x1 << 11)
#define DC21285_Parity0                 (0x0 << 12)
#define DC21285_Parity1                 (0x1 << 12)
/*  Tref, No Parity, CDT=0, Trc=4, Tcas=2, Trcd=2, Tdal(Trp+Trdl)=3, Trp=2
 *        0          0      010      10      01                01
 */

#define saTrp                           DC21285_Trp2
#define saTdal                          DC21285_Tdal3
#define saTrcd                          DC21285_Trcd2
#define saTcas                          DC21285_Tcas3
/*Changed this despite all the warnings from 2 to 3 see Platform.s 
*/
#define saTrc                           DC21285_Trc4
#define saTcd                           DC21285_Tcd0
#define saParity                        DC21285_Parity0

#define SDRAM_MODEBITS                  (saParity + saTcd + saTrc + saTcas + saTrcd + saTdal + saTrp)

/* Tref=1 */
#define INITIAL_TREF                    (0x01 << 16)     /*  Top half of word */
#define INITIAL_TIMING                  (INITIAL_TREF + SDRAM_MODEBITS)

/* Tref=1a (64 ms x 4096 rows) = 15.xx us */
#define SETUP_TREF                      (0x1a << 16)     /*  Top half of word */
#define SETUP_TIMING                    (SETUP_TREF + SDRAM_MODEBITS)

#define REFRESH_COUNT                   32               /*  No. of cycles scaled per wait */
#define REFRESH_WAIT                    8                /*  Max. wait per SDRAM */

/* Interrupt Flags */
#define VALID_INTMASK                   0xf3c3fffe
#define SOFT_INT                        0x00000002
#define AMBACOM1_RX                     0x00000004
#define AMBACOM1_TX                     0x00000008
#define TIMER1_INT                      0x00000010
#define TIMER2_INT                      0x00000020
#define TIMER3_INT                      0x00000040
#define TIMER4_INT                      0x00000080
#define IRQ_IN3_INT                     0x00000400       /*  StrongARM 3.186MHz Xtal here */
#define XSCOM1_INT                      0x00001000       /*  XBUS_DEV_1 interrupts on xcs_l[0] */
#define XSCOM2_INT                      0x00001000
#define DOORBELL_INT                    0x00008000
#define DMA1_INT                        0x00010000
#define DMA2_INT                        0x00020000
#define MEM_PARITY_INT                  0x01000000
#define I2O_INPOST_INT                  0x02000000
#define RAM_PARITY_INT                  0x10000000
#define PCI_MASTER_INT                  0x20000000
#define PCI_TARGET_INT                  0x40000000
#define PCI_PARITY_INT                  0x80000000
#ifdef AMBA_SERIAL
#define ANGEL_INTS                      (AMBACOM1_TX | AMBACOM1_RX)
#else
#define ANGEL_INTS                      XSCOM1_INT
#endif

/* Auto-select the appropriate value: */
#if saTcas != DC21285_Tcas2
#define CAS_OFFSET                      0xc8             /*  If CAS Latency is 3 cycles */
#else
#define CAS_OFFSET                      0x88             /*  If CAS Latency is 2 cycles */
#endif

/* SD_OFFSET is the difference between SDRAM Array Register banks */
#define SD_OFFSET                       (DC21285_DRAM_A1MR - DC21285_DRAM_A0MR)

/* Row/Col Mux modes shifted into correct postion (bits 6, 5 and 4) */
#define MUX_MODE0                       0x00
#define MUX_MODE1                       0x10
#define MUX_MODE2                       0x20
#define MUX_MODE3                       0x30
#define MUX_MODE4                       0x40

#define DRAM_4BANKS                     0x08
#define DRAM_2BANKS                     0x00
#define MUX_MASK                        0x78

/* Bit patterns for SDRAM memory sizes */
#define SDSIZE_1M                       0x01
#define SDSIZE_2M                       0x02
#define SDSIZE_4M                       0x03
#define SDSIZE_8M                       0x04
#define SDSIZE_16M                      0x05
#define SDSIZE_32M                      0x06
#define SDSIZE_64M                      0x07

/* 8MB bit pattern for Verification Vehicle base memory */
#define VV_BASE_MEM                     (SDSIZE_8M | MUX_MODE1)

#define ALLBITS                         0xffffffff

#define uHAL_MEMORY_SIZE                (DRAM_DEFAULT)

#define MAXIRQNUM                       31
#define MAXFIQNUM                       31
#define MAXSWINUM                       31

#ifndef LINUX_NEW_PCI
/* causes name clash with some kernels */
#define NR_IRQS                         (MAXIRQNUM + 1)
#endif
/*  StrongARM Flash block definitions. Used by the primary bootloader to
 *  allow one of several images to be executed at boot-up.
 */
#define FLASH_BLOCK_SIZE                SZ_256K
#define IMAGE_SWITCH                    XBUS_DEV_2       /*  Anywhere in XCS2 space */

#define IMAGE_MASK                      0x0F
#define BENCH_BIT                       0x40
        
/* Flash header definition */
#define FH_TYPE                         0x0
#define FH_NUMBER                       0x4
#define FH_SIG_START                    0x5
#define FH_SIG_END                      0x8
#define FH_MAP                          0x8
#define FH_CHECKSUM                     0xC
#define FH_LENGTH                       0x10
#define FH_NAME                         0x14
#define FH_NAME_END                     0x24
#define FH_BOOTFLAGS                    0x24
#define FH_AIF                          0x40
#define FH_END                          0xC0

#define FH_NOBOOTMASK                   1
                
/* AIF header definition */
#define AIF_DECOMPRESS                  0x0
#define AIF_SELF_RELOC                  0x4
#define AIF_INIT                        0x8
#define AIF_ENTRY                       0xC
#define AIF_EXIT                        0x10
#define AIF_RO_SIZE                     0x14
#define AIF_RW_SIZE                     0x18
#define AIF_DEBUG_SIZE                  0x1C
#define AIF_ZERO_INIT_SIZE              0x20
#define AIF_DEBUG_TYPE                  0x24
#define AIF_BASE                        0x28
#define AIF_WORKSPACE                   0x2C
#define AIF_ADDR_MODE                   0x30
#define AIF_DATA_BASE                   0x34
#define AIF_DEBUG_INIT                  0x40
#define AIF_ZINIT_CODE                  0x44
#define AIF_END                         0x80
        
#define FH_AIF_BASE                     ((FH_AIF)+(AIF_BASE))
#define FH_AIF_ENTRY                    ((FH_AIF)+(AIF_ENTRY))

/*
 * some I2O defs
 */
#define PCI_I2O_INVALID_MFA         0xffffffff
#define PCI_OUT_INT_STATUS_DOORBELL 0x4
#define PCI_OUT_INT_STATUS_OUTPOST  0x8


#endif

/*         END */
