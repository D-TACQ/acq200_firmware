/*****************************************************************************
 *
 * File: stream.c
 *
 * $RCSfile: stream.c,v $
 * 
 * Copyright (C) 2001 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description: streaming functions
 *
 * $Id: stream.c,v 1.30.4.5 2007/01/03 23:16:16 pgm Exp $
 * $Log: stream.c,v $
 * Revision 1.30.4.5  2007/01/03 23:16:16  pgm
 * double fork() is good
 *
 * Revision 1.30.4.4  2003/10/19 20:44:30  pgm
 * works with dt100rc
 *
 * Revision 1.30.4.3  2003/09/08 12:54:20  pgm
 * i2o file download works. ship it quick
 *
 * Revision 1.30.4.2  2003/09/04 21:08:43  pgm
 * getFwrev works (x156)
 *
 * Revision 1.30.4.1  2003/08/29 17:41:03  pgm
 * init pci command handling
 *
 * Revision 1.30  2002/06/10 20:02:41  pgm
 * B1666 multi samples per pci packet
 *
 * Revision 1.29  2002/06/10 11:53:50  pgm
 * streaming runs, but the data is crap
 *
 * Revision 1.28  2002/03/21 12:25:48  pgm
 * no warnings
 *
 * Revision 1.27  2002/03/21 11:54:52  pgm
 * untabify
 *
 * Revision 1.26  2002/03/13 21:43:09  pgm
 * correct formatting with emacs (Stroustrup)
 *
 * Revision 1.25  2002/02/16 07:40:49  pgm
 * attempting to make edge trig faster
 *
 * Revision 1.24  2002/02/09 21:31:45  pgm
 * no warnings
 *
 * Revision 1.23  2002/02/09 09:31:05  pgm
 * war on warnings
 *
 * Revision 1.22  2002/01/05 21:48:26  pgm
 * fixes DUMDMA stride probs
 *
 * Revision 1.21  2001/11/05 22:28:41  pgm
 * TEST messages, fix gut buffer overrun bug
 *
 * Revision 1.20  2001/10/01 14:01:25  pgm
 * hard trigger works for AO
 *
 * Revision 1.19  2001/10/01 09:14:36  pgm
 * LCT_NOTIFY done
 *
 * Revision 1.18  2001/09/30 18:38:01  pgm
 * i2o two commands still
 *
 * Revision 1.17  2001/09/30 14:08:20  pgm
 * i2o boot time - two commands
 *
 * Revision 1.16  2001/09/24 12:54:31  pgm
 * GPEM AO works
 *
 * Revision 1.15  2001/03/31 20:45:13  pgm
 * DUMDMA works with debugs on ...
 *
 * Revision 1.14  2001/03/30 20:20:13  pgm
 * UDMA first cut working
 *
 * Revision 1.13  2001/03/25 19:20:28  pgm
 * first cut i2o command handling
 *
 * Revision 1.12  2001/03/25 10:24:45  pgm
 * incoming i2o works with cache disabled. outgoing trahsed :-(
 *
 * Revision 1.11  2001/03/24 20:47:17  pgm
 * rejig I2O, add incoming hooks (outgoing still works)
 *
 * Revision 1.10  2001/03/20 21:51:05  pgm
 * added headers
 *
 * Revision 1.2  2000/12/28 11:03:00  pgm
 * *** empty log message ***
 *
 * Revision 1.1  1999/10/22 16:26:49  pgm
 * first entry to cvs
 *
 *
\*****************************************************************************/

#warning NOT HERE, NOT NOW - ACQ32 ONLY
/*
 * stream.c ... controls streaming
 */

#include <stdio.h>

#include "local.h"
#include "dt100api.h"

#include "acq32_drv.h"
#include "dataHandling.h"
#include "memmap.h"

#include "stream.h"

#include "dlog.h"

#include "hal.h"



typedef Message* pMessage;

#define RI2O_IFH    (*(pMessage**)(CSR_BASE+I2O_IFH))        // 0x120
#define RI2O_IPT    (*(pMessage**)(CSR_BASE+I2O_IPT))        // 0x124
#define RI2O_IFC    (*(unsigned*) (CSR_BASE+I2O_IFC))        // 0x130
#define RI2O_IPC    (*(unsigned*) (CSR_BASE+I2O_IPC))        // 0x138

#define RI2O_OPH    (*(pMessage**)(CSR_BASE+I2O_OPH))        // 0x128
#define RI2O_OFT    (*(pMessage**)(CSR_BASE+I2O_OFT))        // 0x12C
#define RI2O_OPC    (*(unsigned*) (CSR_BASE+I2O_OPC))        // 0x134

#define RSA_CONTROL (*(unsigned*) (CSR_BASE+SA_CONTROL))     // 0x13C

#define SA_CONTROL_I2O_SZ_MASK  0x000001c00
#define SA_CONTROL_I2O_SZ_256   0x000000000

#define NFIFO                   256             // smallest FIFO on 21285
#define NFIFOBYTEMASK   ((NFIFO-1)<<2)

#define MFA_INVALID         0xffffffffU
#define I2O_COUNT_SET      0x80000000U
#define I2O_COUNT_INCR     0x00000000U

#define RESET_COUNT( reg )       ((reg) = I2O_COUNT_SET | 0 )
#define BUMP_COUNT( reg )        ((reg) = I2O_COUNT_INCR)

typedef pMessage Fifo[NFIFO];           // 256*4 = 1K
typedef Message MessageBlock[NFIFO]; // 256K

typedef struct FIFOBLOCK {
    struct FIFO_LISTS {
        struct FIFO_DATA_PAIR {
            Fifo free;
            Fifo post;
        }
        in, out;
    }
    lists;
        
    struct {
        MessageBlock in;
#ifdef PGMCOMOUT                // this feature obsolete, out of memory - discard
        MessageBlock out;    
#endif      
    }
    buffers;
    int end;
}
FifoBlock;                                              // 4K

/*
 * locate the software pointer impl in regular cached memory
 */
static  struct SA110_FIFO_POINTERS {
    pMessage* OPT;       // Output Postlist Tail
    pMessage* OFH;       // Output Freelist Head

    pMessage* IFT;
    pMessage* IPH;
}
sa_pointers;

/*
 * access the globals thru these macros ...
 */
#define FB  ((FifoBlock*)(I2O_BASE))
#define SI2O    (sa_pointers)

static __inline pMessage* increment( pMessage* fp )
{ 
    unsigned ubase = ((unsigned)fp) & ~NFIFOBYTEMASK;
    unsigned uindx = (unsigned)++fp;

  
    uindx &= NFIFOBYTEMASK;
    uindx |= ubase;

    return (pMessage*)uindx;
}

static int isValidIncomingMessage( pMessage message )
{
    return IN_RANGE( message, &FB->buffers.in[0], &FB->buffers.in[NFIFO] );
}


static void init_outbound_regs()
/* WARNING: assumes NFIFO is 256 */
{
    /*
     * config size
     */
    PRINTF( "SA_CONTROL 0x%08x was 0x%08x\n", &RSA_CONTROL, RSA_CONTROL );
    RSA_CONTROL &= ~SA_CONTROL_I2O_SZ_MASK;
    RSA_CONTROL |=  SA_CONTROL_I2O_SZ_256;
    PRINTF( "SA_CONTROL 0x%08x now 0x%08x\n", &RSA_CONTROL, RSA_CONTROL );

    /*
     * init I2O regs
     */
    RI2O_OPH = SI2O.OPT = &FB->lists.out.post[0];
    RI2O_OFT = SI2O.OFH = &FB->lists.out.free[0];

    RESET_COUNT( RI2O_OPC );
    
    PRINTF( "init_outbound_regs() done" );
}

static void init_inbound_regs()
/* WARNING: assumes NFIFO is 256 */
{
    /*
     * config size
     */
    PRINTF( "SA_CONTROL 0x%08x was 0x%08x\n", &RSA_CONTROL, RSA_CONTROL );
    RSA_CONTROL &= ~SA_CONTROL_I2O_SZ_MASK;
    RSA_CONTROL |=  SA_CONTROL_I2O_SZ_256;
    PRINTF( "SA_CONTROL 0x%08x now 0x%08x\n", &RSA_CONTROL, RSA_CONTROL );

    /*
     * init I2O regs
     */
    RI2O_IPT = SI2O.IPH = &FB->lists.in.post[0];
    RI2O_IFH = SI2O.IFT = &FB->lists.in.free[0];

    RESET_COUNT( RI2O_IPC );
    RESET_COUNT( RI2O_IFC );
    PRINTF( "init_inbound_regs() done" );    
}


static void initLocalBuffersForHost( 
    struct FIFO_DATA_PAIR* fifo_pair,
    MessageBlock* mb,
    int nfifo
    )
{
    int imessage;  

    for ( imessage = 0; imessage != nfifo; ++imessage ){
        fifo_pair->free[imessage] = &(*mb)[imessage];
        (*mb)[imessage].header.id  = imessage;
        memset( (*mb)[imessage].payload.sdata, imessage+'0', MESSAGE_DATA_LEN );
        
        fifo_pair->post[imessage] = (pMessage)MFA_INVALID;
    }
}
void i2o_initOutboundHostPull( int nfifo )
{
#ifdef PGMCOMOUT
    if ( nfifo <= 0 || nfifo > NFIFO ) nfifo = NFIFO;
    /*
     * init buffers fill the Outbound free list
     */

    initLocalBuffersForHost( &FB->lists.out,  &FB->buffers.out, nfifo );
    init_outbound_regs();    
#else
    PRINTF( "i2o_initOutboundHostPull() - STUB - "
            "obsoleted by i2o_initOutboundAcq32Push() \n" );
#endif          
}

void i2o_initOutboundAcq32Push( 
    int nfifo, unsigned pci_addr, unsigned buf_len )
// divvy out a host buffer amount the fifo slots
{
    struct FIFO_DATA_PAIR* fifo_pair = &FB->lists.out;
    int message_len = buf_len/nfifo;

    int imessage;

    PRINTF( "i2o_initOutboundAcq32Push() base 0x%08x message_len 0x%x\n",
            pci_addr, message_len );

    /*if ( nfifo <= 0 || nfifo > NFIFO ) nfifo = NFIFO;*/
    nfifo = NFIFO;
    /*
     * init buffers fill the Outbound free list
     */
    for ( imessage=0; imessage != nfifo; ++imessage, pci_addr += message_len){
        fifo_pair->free[imessage] = (pMessage)pci_addr;
        fifo_pair->post[imessage] = (pMessage)MFA_INVALID;
    }

    init_outbound_regs();
}

Message* i2o_getFreeOutboundMessage()
{
    static int recursion;
    static int total_recursions;

    PRINTFL(3)( "SI2O.OFH is 0x%08x\n", SI2O.OFH );
 
    if ( P_debug > 8 ){
        i2o_showOutboundStatus();
    }else{
        PRINTFL(8)( "SI2O.OFH is 0x%08x\n", SI2O.OFH );
    }
     
    if ( increment( SI2O.OFH ) == RI2O_OFT ){
        dlogPrintf( FN " - buffer underrun, free list is EMPTY\n" );
        PRINTF( FN " - buffer underrun, free list is EMPTY\n" );
        return NULL;
    }else{


        Message* message = *SI2O.OFH;
        SI2O.OFH = increment( SI2O.OFH );
        
        PRINTFL(3)( FN " returning 0x%08x\n", message );
        PRINTFL(3)( "OFH now 0x%08x, RI2O_OFT now 0x%08x\n",SI2O.OFH,RI2O_OFT);
        dlogPrintf( "OFH now 0x%08x, RI2O_OFT now 0x%08x\n",SI2O.OFH,RI2O_OFT);

        {       // trap duff message receive
            // is this still needed ????

            u32 temp = (u32)message;

            if ( (temp&0xffff) == 0x9191 ){
                    
                printf( "ERROR: getting a duff message 0x%08x (depth %d)\n", 
                        temp, ++recursion );
                                                                        
                if ( total_recursions++ > 10 ){
                    acq32_setInternalAbort( ACQ32_SET_INTERNAL_ABORT );
                    printf( FN " baling out\n" );
                    return NULL;
                }
                return i2o_getFreeOutboundMessage();
            }
        }

        if ( recursion != 0 ){
            printf( "leaving recursion returning 0x%08x\n", message );
            recursion = 0;
        }

		ST_PRINTF( FN " %p\n", message );                
        return message;
    }
}
void i2o_postOutboundMessage( Message* message )
{
    ST_PRINTF( FN, " %p\n", message );
    {   // trap duff message send

        u32 temp = (u32)message;

        if ( (temp&0xffff) == 0x9191 ){
            printf( "ERROR: posting a duff message 0x%08x\n", temp );
        }
    }

    if ( RI2O_OPC < NFIFO ){
/*
 * sometimes the mfa rx'd by host is duff. copy into mbox2 for good measure
 * this is a gross hack, but will work for Q length 1
 * which we've had to do elsewhere anyway
 */
        *mbox( 2 ) = (u32)message;
    
        *SI2O.OPT = message;
        SI2O.OPT = increment( SI2O.OPT );
        BUMP_COUNT( RI2O_OPC );
        
        PRINTFL(3)( FN " RI2O_OPC now 0x%08x\n", RI2O_OPC );
        dlogPrintf( FN " RI2O_OPC now 0x%08x\n", RI2O_OPC );
    }else{
        PRINTF( FN " - buffer overrun, discard" );
        dlogPrintf( FN " - buffer overrun, discard" );
    }
}

void i2o_showOutboundStatus( void ) 
{
    PRINTF( "FB %p ends %p %s\n", FB, &FB->end, 
            I2O_END>(unsigned)&FB->end? "OK": "OVERRUN" );
    PRINTF( "FB->lists.out.free[0] = 0x%08x\n", &FB->lists.out.free[0] );

#define PCSRV( csr ) PRINTF( "%20s 0x%08x\n", #csr, csr )
    PCSRV( RI2O_OFT );
    PCSRV( SI2O.OFH );
    PCSRV( RI2O_OPH );
    PCSRV( SI2O.OPT );
    PCSRV( RI2O_OPC );
#undef PCSRV            
}

int i2o_numberOutboundMessagesPending( void )
{
    return RI2O_OPC;
}

/*
 * INBOUND
 */
void i2o_initInboundHostPush( int nfifo )
{
    if ( nfifo <= 0 || nfifo > NFIFO ) nfifo = NFIFO;
    /*
     * init buffers fill the Outbound free list
     */

    initLocalBuffersForHost( &FB->lists.in,  &FB->buffers.in, nfifo );
    init_inbound_regs();    
    while( nfifo-- ){
        BUMP_COUNT( RI2O_IFC );
    } 
        
        
    PRINTF( "i2o_initInboundHostPush() done\n" );   
} 

static int acq32Message( Message* message );

Message* i2o_getInboundMessage( void )
{
    if ( RI2O_IPC != 0 ){
        Message* the_message = *SI2O.IPH;
        SI2O.IPH = increment( SI2O.IPH );
        BUMP_COUNT( RI2O_IPC );
            
        PRINTFL(1)( FN "new message %p, IPH now %p\n", the_message, SI2O.IPH );
            
        if ( !isValidIncomingMessage( the_message ) ){
            printf( FN "Invalid message received %p\n", the_message );
            return NO_MESSAGE;
        }else if ( !acq32Message( the_message ) ){
            return NO_MESSAGE;
        }else{
            return the_message;
        }
    }else{
        return NO_MESSAGE;
    }
}

void i2o_returnInboundMessage( Message* message )
{
    *SI2O.IFT = message;
    SI2O.IFT = increment( SI2O.IFT );
    BUMP_COUNT( RI2O_IFC );
    
    if ( P_debug > 8 ){
        i2o_showInboundStatus(); // DEBUG 
    }
}

void i2o_showInboundStatus( void ) 
{
    PRINTF( "FB->lists.in.free[0] = 0x%08x\n", &FB->lists.out.free[0] );

#define PCSRV( csr ) PRINTF( "%20s 0x%08x\n", #csr, csr )
    PCSRV( SI2O.IFT );
    PCSRV( RI2O_IFH );
    PCSRV( RI2O_IPT );
    PCSRV( SI2O.IPH );
    PCSRV( RI2O_IPC );
#undef PCSRV    
}

/*
 * I2O basics
 */
 
#include <stddef.h>
#include "dmac.h"
#include "i2o.h"

// [1] Fig 4.3.8

struct StatusBlock {
    u16 OrganizationID;
    u16 reserved;
    u16 IOP_ID;            // 12 bits
    u16 HostUnitID;
    u16 SegmentNumber;    // 12 bits, top==I2O Version
    u8 IopState;
    u8 MessengerType;
    u16 InboundMFrameSize;
    u8 InitCode;
    u8 reserved2;
    u32 MaxInboundMFrames;
    u32 CurrentInboundMFrames;
    u32 MaxOutboundMFrames;
    char ProductIDString[24];
    u32 ExpectedLctSize;
    u32 IopCapabilities;
    u32 DesiredPrivateMemSize;
    u32 CurrentPrivateMemSize;
    u32 CurrentPrivateMemBase;
    u32 DesiredPrivateOSize;
    u32 CurrentPrivateOSize;
    u32 CurrentPrivateIOBase;
    u8  reserved3[3];                
    u8  EndMarker;
};

enum IOPSTATE {
    IOPS_INIT     = 0x01,
    IOPS_RESET    = 0x02,
    IOPS_HOLD     = 0x04,
    IOPS_READY    = 0x05,
    IOPS_OP       = 0x08,
    IOPS_FAILED   = 0x10,
    IOPS_FAULTED  = 0x11
};

static struct StatusBlock sb;

void i2o_set_device_state( int state )
{
    sb.IopState = state;
}

static void initStatusBlock( void )
{
    memset( &sb, 0, sizeof(sb) );
    strcpy( sb.ProductIDString, "D-TACQ acq32" );
    sb.IOP_ID = 0x0fff;
    sb.SegmentNumber = 0x1000;     // hi  nibble version==1
    sb.IopState = IOPS_RESET;
    sb.InboundMFrameSize = 16;
    sb.CurrentInboundMFrames = 256;
    sb.MaxInboundMFrames     = 256;
    sb.MaxOutboundMFrames    = 256;
    
    sb.IopCapabilities = 0x4;        // 32 bit context only
    sb.ExpectedLctSize = 20;        // SWAG
    sb.EndMarker = 0xff;
    
    printf( "initStatusBlock() sb at %p state set to %d\n", &sb, sb.IopState );
}

static void handleStatusGet( struct i2o_message* mh )
{
	int idx = 4;									// YAHBOO take that ARM SDT WARNING!
    u32 replyAddr = mh->initiator_context[idx];
    
    memcpy4( pci2vma( replyAddr ), (unsigned*)&sb, sizeof(sb) );
   
    printf( "handleStatusGet():Status Block size %d"
            "offset of EndMarker %d state %d\n",
            sizeof(struct StatusBlock), 
            offsetof( struct StatusBlock, EndMarker),
            sb.IopState );    
}

static void handleOutboundInit(  struct i2o_message* mh )
{
	int idx = 5;									// YAHBOO take that ARM SDT WARNING!
    u32 replyAddr = mh->initiator_context[idx]; //yes, SGL+1 buffer context==addr

    *(R32*)pci2vma( replyAddr ) = 1;    // IN PROGRESS
    printf( "handleOutboundInit() set 0x%08x = 0x%08x\n", 
            replyAddr, *(u32*)pci2vma( replyAddr ) );

    init_outbound_regs();
    sb.IopState = IOPS_HOLD;
        
    *(R32*)pci2vma( replyAddr ) = 4; // COMPLETE
    
    printf( "handleOutboundInit() set 0x%08x = 0x%08x\n", 
            replyAddr, *(u32*)pci2vma( replyAddr ) );
}

static void handleHrtGet( struct i2o_message* mh )
{
    printf( "handleHrtGet() STUB\n" );
}

static void handleAdapterReset( struct i2o_message* mh )
{
    initStatusBlock();
}

static struct lct_notify_reply lct;

extern unsigned long strtoul( const char*, int, int );

static void init_lct( void )
{
    memset( &lct, 0, sizeof(lct) );
    lct.TableSize = sizeof(lct)/4;
    
    lct.entries[0].TableEntrySize = 9;
    lct.entries[0].IdentityTag = strtoul( calGetBoard()+1, 0, 0 );
    
}
static void handleLctNotify( struct i2o_message* mh )
{
	int idx = 5;									// YAHBOO take that ARM SDT WARNING!
    struct i2o_message* reply = (struct i2o_message*)i2o_getFreeOutboundMessage();
    u32 replyAddr = mh->initiator_context[idx]; // yes, SGL+1 buffer context==addr
    memcpy( pci2vma(replyAddr), &lct, sizeof(lct) );
  
    mh->flags = 0xc0;
              
    if ( !reply ) printf ( "AARGH - not poossible to reply\n" );
        
    memcpy4( pci2vma( (u32)reply ), (unsigned*)mh, sizeof(struct i2o_message));
    i2o_postOutboundMessage( (Message*)reply );
        
    printf( "handleLctNotify() done reply %p\n", reply );
}

static void dumpI2O( struct i2o_message* mh )
{
#define DUMPMH( field, fmt ) \
    printf( "%30s " fmt "\n", #field, mh->field )
#define WFMT "%04x"
#define BFMT "%02x"
#define LFMT "%08x"
#define SP   " "

    switch( mh->function ){
    case I2O_CMD_STATUS_GET:
        handleStatusGet( mh );
        break;
    case I2O_CMD_OUTBOUND_INIT:
        handleOutboundInit( mh );
        break; 
    case I2O_CMD_HRT_GET:
        handleHrtGet( mh );
        break;
    case I2O_CMD_ADAPTER_RESET:
        handleAdapterReset( mh );
        break;
    case I2O_CMD_LCT_NOTIFY:
        handleLctNotify( mh );
        break;
    default:
        printf( "WARNING:unknown function\n" );
    }
                
    //u32* pcontext = &mh->initiator_context;
    printf( "I2O message received %p\n", mh );
        
    DUMPMH( version_offset,    BFMT );
    DUMPMH( flags,    BFMT );
    DUMPMH( size,     WFMT );
    DUMPMH( target_tid, "0x%03x" );
    DUMPMH( init_tid, "0x%03x" );
    DUMPMH( function, BFMT );
    DUMPMH( initiator_context[0], LFMT );
#ifdef PGMCOMOUT
// cut down on the warnings ...
    DUMPMH( initiator_context[1], LFMT );
    DUMPMH( initiator_context[2], LFMT );
    DUMPMH( initiator_context[3], LFMT );
    DUMPMH( initiator_context[4], LFMT );
    DUMPMH( initiator_context[5], LFMT );
    DUMPMH( initiator_context[6], LFMT );
#endif  
    i2o_returnInboundMessage( (Message*)mh );
}
static int acq32Message( Message* message )
{
// WORKTODO ... we may have to change pkt format to be compatible with I2O!!

#define MINMSG (sizeof(MessageHeader)) 
#define MAXMSG sizeof( struct MESSAGE )

    if ( IS_ID_DTACQ( message->header.id ) &&
         IN_RANGE( message->header.length,  MINMSG, MAXMSG ) &&
         MT_ID(message->header.type) == MT_COMMAND_PACKET &&
#ifdef WE_LOVE_WARNINGS
         IN_RANGE( MT_PRAM(message->header.type), MTC_TEST, MTC_LAST )
#endif
         MT_PRAM(message->header.type) <= MTC_LAST ){
        return 1;
    }else{
        dumpI2O( ( struct i2o_message*)message );
        return 0;
    }
}

void i2o_static_init( void )
{
    initStatusBlock();
    init_lct();
    i2o_initInboundHostPush(256);
    init_outbound_regs(); // sets up the pointers. contents not yet initialized.    
}

