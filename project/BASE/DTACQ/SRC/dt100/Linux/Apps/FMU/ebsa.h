/*
 * ebsa.h - header for ebsa device driver
 */


#include <asm/ioctl.h>

/*
 * Macros to help debugging
 */

#include "printp.h"
#define printk printp

#undef PDEBUG             /* undef it, just in case */
#ifdef EBSA_DEBUG
#  ifdef __KERNEL__
     /* This one if debugging is on, and kernel space */
#    define PDEBUG(fmt, args...) printk( KERN_DEBUG "ebsa: " fmt, ## args)
#  else
     /* This one for user space */
#    define PDEBUG(fmt, args...) fprintf(stderr, fmt, ## args)
#  endif
#else
#  define PDEBUG(fmt, args...) /* not debugging: nothing */
#endif

#undef PDEBUGG
#define PDEBUGG(fmt, args...) /* nothing: it's a placeholder */



/*
 * things that traditionally belong in local.h
 */

#define IN_RANGE( xx, mm, MM ) ((xx)>=(mm)&&(xx)<=(MM))

#ifdef __KERNEL__
#define ASSERT( cond ) \
    while ( !cond ){ \
        PDEBUG( "ASSERTION: %s %d", __FILE__, __LINE__ );\
        while(1) { schedule(); } \
    }
#else
#define ASSERT( cond ) \
    while( !cond ){ \
        fprintf( stderr, "ASSERTION: %s %d\n", __FILE__, __LINE__ );\
        exit( 1 );\
    }
#endif


#ifndef EBSA_MAJOR
#define EBSA_MAJOR 0   /* dynamic major by default */
#endif

#ifndef EBSA_NR_DEVS
#define EBSA_NR_DEVS 4    /* ebsa0 through ebsa3 */
#endif

#define BUFLEN  0x1000

#ifdef __KERNEL__

typedef struct Ebsa_Dev {
   void **data;
   struct Ebsa_Dev *next;   /* next listitem */
   int quantum;              /* the current quantum size */
   int qset;                 /* the current array size */
   unsigned long size;
   unsigned int access_key;  /* used by uid and ebsapriv */
   unsigned int usage;       /* lock the device while using it */

   struct pci_dev* p_pci;

  u32* p_iomapped;    /* ROM for ROM driver, SDRAM for RAM driver */
  u32* p_csr;         /* points to CSR mapping */

  u32 buffer[BUFLEN];
} Ebsa_Dev;

#define CSR( dev, byte_off )    ((u32*)((char*)(dev)->p_csr+(byte_off)))

/*
 * Split minors in two parts
 * Masks: Channel: 0x1f { channel 0..31 }
 *        Board  : 0x60 { board 0..3 }
 *        Rom    : 0x80 { + board to select }
 */

#define IS_ROM(dev)     ((MINOR(dev)&0x80)!=0)
#define BOARD(dev)      ((MINOR(dev)&0x60)>>5)
#define CHANNEL(dev)    ( MINOR(dev)&0x1f )

#define ACQ32_MAX_CHANNELS 32 /* thinking ahead!! */
/*
 * The different configurable parameters
 */
extern int ebsa_major;     /* main.c */
extern int ebsa_nr_devs;

#ifdef EBSA_DEBUG
#    define EBSA_USE_PROC
#endif

#ifndef min
#  define min(a,b) ((a)<(b) ? (a) : (b))
#endif

#endif                      /* __KERNEL__ */

/*
 * Ioctl definitions
 */

/* Use 'e' as magic number */
#define EBSA_IOC_MAGIC  'e'

#define EBSA_IOCRESET    _IO(EBSA_IOC_MAGIC, 0)

/*
 * S means "Set" through a ptr,
 * T means "Tell" directly with the argument value
 * G means "Get": reply by setting through a pointer
 * Q means "Query": response is on the return value
 * X means "eXchange": G and S atomically
 * H means "sHift": T and Q atomically
 */

#define EBSA_IOCGMEM      _IOR(EBSA_IOC_MAGIC,   1, sizeof(void*))
#define EBSA_IOCHARDRESET _IO (EBSA_IOC_MAGIC, 15 ) /* debugging tool */

/*
 * access the mailboxes 0 <= ix <= 3
 */

#define EBSA_IO_MBX_NR 10

#define EBSA_IOSMBX( ix ) \
    _IO (EBSA_IOC_MAGIC,  EBSA_IO_MBX_NR+(ix) )

#define EBSA_IOGMBX( ix ) \
    _IOR(EBSA_IOC_MAGIC, EBSA_IO_MBX_NR +(ix), sizeof(unsigned*))

#define EBSA_IOC_MAXNR 15


