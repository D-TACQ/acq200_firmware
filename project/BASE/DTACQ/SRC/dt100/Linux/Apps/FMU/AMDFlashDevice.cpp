/*****************************************************************************
 *
 * File: AMDFlashDevice.cpp: impl of flash device for AMD
 *           
 *
 * $RCSfile: AMDFlashDevice.cpp,v $
 * 
 * Copyright (C) 1999 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: AMDFlashDevice.cpp,v 1.13 2002/07/30 12:58:00 pgm Exp $
 * $Log: AMDFlashDevice.cpp,v $
 * Revision 1.13  2002/07/30 12:58:00  pgm
 * more retries, test on exit
 *
 * Revision 1.12  2002/07/30 11:00:52  pgm
 * with retry on write
 *
 * Revision 1.11  2001/10/14 08:59:39  pgm
 * works on Alpha
 *
 * Revision 1.10  2001/08/12 08:33:08  pgm
 * works on Lewis
 *
 * Revision 1.9  2001/05/06 18:47:20  pgm
 * cleanup
 *
 * Revision 1.8  2000/09/30 09:50:08  pgm
 * MakeArmSpin call added for boot66 op
 *
 * Revision 1.7  1999/10/31 21:02:49  pgm
 * command line args are go
 *
 * Revision 1.6  1999/10/31 20:22:43  pgm
 * help and ident opts added
 *
 * Revision 1.5  1999/10/31 19:41:11  pgm
 * works again with acq32 device
 *
 * Revision 1.4  1999/10/11 10:34:56  pgm
 * Works at DTACQ
 *
 * Revision 1.3  1999/10/01 21:47:05  pgm
 * WORD rwrite fixup/frigs for 21285
 *
 * Revision 1.2  1999/10/01 09:18:06  pgm
 * fix leader instance bug
 *
 * Revision 1.1  1999/09/30 20:46:04  pgm
 * *** empty log message ***
 *
 * Revision 1.1  1999/09/30 10:14:33  pgm
 * pfmu - enhanced fmu utility
 *
 *
\*****************************************************************************/



#include "local.h"

#include <sys/ioctl.h>

#include "acq32ioctl.h"

#include <stdio.h>

#include <stdlib.h>           // exit()
#include <unistd.h>           // sleep()

#include "FlashDevice.h"

extern int G_debug;

#define DPRINTF    if ( G_debug ) printf

#define DPROMPT    do {\
    if ( G_debug > 2 ) { \
        printf( "<CR> to continue\n" ); getchar(); \
    } } while( 0 )
 
class AMDFlash: public FlashDevice
// works for AMD29DL800B ... add others as required
{
    static const BlockDef m_block_def[];

protected:
    virtual volatile unsigned BASE( int offset );
    virtual volatile unsigned BASE( int offset, unsigned value );
    virtual volatile unsigned short WBASE( int woffset, unsigned value );    
    
protected:
    bool OnInitIfPresent() 
    {
        bool found_it = false;

        WBASE( 0x0000, 0x00f0 );             // Reset

        WBASE( 0x0555, 0x00aa );            // Unlock

        WBASE( 0x02aa, 0x0055 );

        WBASE( 0x0555, 0x0090 );                // Autoselect
        
        unsigned man_id = BASE( 0 );

        printf( "AMDFlash: looking for 0x%08x got 0x%08x\n", 
                   GetId(), man_id );
       if ( man_id == GetId() ){
           found_it = true;
       }

    /* Make sure the flashPtr is back in normal mode so 
       that we can do user I/O via firmware in flash */

        WBASE( 0x0000, 0x00f0 );              // Reset	

        return found_it;
    }
    AMDFlash( unsigned *const p_base, unsigned *const p_csr  ):
        FlashDevice( m_block_def, p_base, p_csr, 0x225b0001 )
    {

    }


public:
    AMDFlash() :
        FlashDevice( m_block_def )
    {
        m_ident = "AMDFlashDevice";
    }
    virtual FlashDevice* Clone( unsigned *const p_base, unsigned *const p_csr )
    {
        return new AMDFlash( p_base, p_csr );
    }

    virtual unsigned read( int offset );
    virtual int onWrite( int offset, unsigned data );
    virtual bool DeleteBlock( int iblock );

    virtual void ChipErase()
    {
        MakeArmSpin(); 
        printf( "ChipErase()\n" );
        WBASE( 0x555, 0x00aa );
        WBASE( 0x2aa, 0x0055 );
        WBASE( 0x555, 0x0080 );
        WBASE( 0x555, 0x00aa );
        WBASE( 0x2aa, 0x0055 );
        WBASE( 0x555, 0x0010 );
        printf( "ChipErase() done\n" );
        sleep( 1 );
        WBASE( 0x000, 0x00f0 );
    }
};

const FlashDevice::BlockDef AMDFlash::m_block_def[] = {
  { OFFSET(    0*KB ), 16*KB },
  { OFFSET(   16*KB ),  8*KB },
  { OFFSET(   24*KB ),  8*KB },
  { OFFSET(   32*KB ), 32*KB },
  { OFFSET(   64*KB ), 64*KB },
/*
  { OFFSET(    0*KB ), 16*KB },
  { OFFSET(   16*KB ), 32*KB },
  { OFFSET(   48*KB ),  8*KB },
  { OFFSET(   56*KB ),  8*KB },
  { OFFSET(   64*KB ),  8*KB },
  { OFFSET(   72*KB ),  8*KB },
  { OFFSET(   80*KB ), 32*KB },
  { OFFSET(  112*KB ), 16*KB },
*/
  { OFFSET( 2*64*KB ), 64*KB },
  { OFFSET( 3*64*KB ), 64*KB },
  { OFFSET( 4*64*KB ), 64*KB },
  { OFFSET( 5*64*KB ), 64*KB },
  { OFFSET( 6*64*KB ), 64*KB },
  { OFFSET( 7*64*KB ), 64*KB },
  { OFFSET( 8*64*KB ), 64*KB },
  { OFFSET( 9*64*KB ), 64*KB },
  { OFFSET(10*64*KB ), 64*KB },
  { OFFSET(11*64*KB ), 64*KB },
  { OFFSET(12*64*KB ), 64*KB },
  { OFFSET(13*64*KB ), 64*KB },
  { OFFSET(14*64*KB ), 64*KB },
  { OFFSET(15*64*KB ), 64*KB },
  {}
};


unsigned AMDFlash::read( int offset )
{
    return BASE( offset );
}

struct LongWordProgDef {
    struct {
        int woffset;
        unsigned short data;
    }
        defs[2];

   enum { LO, HI };                 // Little endian uggh

    LongWordProgDef( int offset, unsigned data )
    {
        defs[LO].woffset = offset*2;
        defs[LO].data    = data&0x0ffff;
        defs[HI].woffset = offset*2+1;
        defs[HI].data    = data>>16;
    }   
};

#define DQ7(data)    ((data&0x80) != 0)
#define DQ5(data)    ((data&0x20) != 0)


int AMDFlash::onWrite( int offset, unsigned data )
{
    LongWordProgDef lw( offset, data );

    for ( int iword =  LongWordProgDef::LO; 
                          iword <=  LongWordProgDef::HI; ++iword ){

        int woffset          = lw.defs[iword].woffset;
        unsigned short wdata = lw.defs[iword].data;

        WBASE( 0x0000, 0x00f0 );              // Reset

        WBASE( 0x0555, 0x00aa );              // Unlock
        WBASE( 0x02aa, 0x0055 );

        WBASE( 0x0555, 0x00a0 );              // Program

        WBASE( woffset,  wdata );             // Do it

        unsigned status;
        bool second_chance = false;
        int poles = 0;
//getchar();
// sequence from AMD manual Fig5
        while(1) {
            status = BASE( woffset/2 );
            if ( woffset&0x1 ){
	        status >>= 16;          // get hi word
            }            
/*
            printf( "woff %04x 2c %d wdata 0x%04x status %08x\n",
                      woffset, second_chance, wdata, status );
*/
            if ( !second_chance ){
                if ( DQ7(status) == DQ7(wdata) ){
                    break;                   // SUCCESS
                }else if ( !DQ5(status) ){
                    if ( (++poles&0xffff) == 0xffff )
                    printf( "woffset %08x status %04x wdata %04x\n",
                              woffset, status, wdata                   );
                    
                    continue;
                }else{
                    second_chance = true;
                }
            }else{
                if ( DQ7(status) != DQ7(wdata) ){
                   WBASE( 0, 0x00f0 );      // FAIL
                   return 0;
                }else{
                   break;                   // SUCCESS
                }                  
            }
        }
        status = BASE(woffset/2);  // read again just to slow down
    }

    WBASE( 0, 0x00f0 );                      // Reset

    /* finally check the word is correct */
    
    return BASE(offset) == data;
}

bool AMDFlash::DeleteBlock( int iblock )
{
    printf( "AMDFlash::DeleteBlock starting at 0x%08x\n",
              GetBlockStart( iblock ) * 4 );

    bool success = true;

    WBASE( 0x0000, 0x00f0 );              // Reset

    WBASE( 0x0555, 0x00aa );              // Unlock
    WBASE( 0x02aa, 0x0055 );

    WBASE( 0x0555, 0x0080 );              // Erase Block
    WBASE( 0x0555, 0x00aa );           
    WBASE( 0x02aa, 0x0055 );

    unsigned offset = GetBlockStart( iblock )*2;

    WBASE( offset, 0x30 );

    sleep( 1 );

    unsigned status;

    unsigned spinner = 0;

    do {
        status = BASE( offset );

        if ( (++spinner&0xffff) == 0 ){
            fprintf( stderr, "DeleteBlock status 0x%02x wanted 0x%02x\n",
                                  status, 0xa0 );
            sleep( 1 );
            if ( spinner&0x20000 ){
                exit( 1 );
            }
	}
    }
        while( (status&0xa0) == 0 );

    if ( (status&0xa0) == 0x20 ){
        fprintf( stderr, "AMDFlash::DeleteBlock() timed out\n" );
        success = false;
    }


    WBASE( 0, 0x00f0 );

    return success;
}

#ifdef PGMCOMOUT
volatile unsigned AMDFlash::BASE( int offset ) 
{  
    D21285OffsetFixup( offset );
    volatile unsigned value = m_p_base[offset]; 

    DPRINTF( "READ ( 0x%04x ) 0x%08x is 0x%08x\n", 
                 offset, (unsigned)&m_p_base[offset], value );

    DPROMPT;
    return value;
}

volatile unsigned AMDFlash::BASE( int offset, unsigned value )
{
    D21285OffsetFixup( offset );
    DPRINTF( "WRITE( 0x%04x, 0x%08x ) 0x%08x := 0x%08x\n",
                 offset, value, (unsigned)&m_p_base[offset], value  );

    m_p_base[offset] = value;
   
    return value;
}

volatile unsigned short AMDFlash::WBASE( int woffset, unsigned value )
{
    volatile unsigned short* p_wbase =
              (volatile unsigned short *)m_p_base;

    D21285WordOffsetFixup( woffset );

    D21285WordFix( woffset );
    woffset &= ~0x1;

    DPRINTF( "WRITE( 0x%04x, 0x%08x ) 0x%08x := 0x%04x\n",
                 woffset, value, (unsigned)&p_wbase[woffset], value  );

    p_wbase[woffset] = value;
   
    DPROMPT;

    return value;
}
#else
/* for Alpha, and ultimately x86, use ioctls instead */
volatile unsigned AMDFlash::BASE( int offset ) 
{  
    struct ACQ32_RW_DEF def;

	def.offset = offset*4;
	
	int rc = ioctl( Board::GetFileId(), ACQ32_IOG_READ32, &def );
	
	ASSERT( rc == 0 );
	
	return def.data.l;
}

volatile unsigned AMDFlash::BASE( int offset, unsigned value )
{
    struct ACQ32_RW_DEF def;

	def.offset = offset*4;
	def.data.l = value;
	
	int rc = ioctl( Board::GetFileId(), ACQ32_IOS_WRITE32, &def );
	
	ASSERT( rc == 0 );

    return value;	    
}

volatile unsigned short AMDFlash::WBASE( int woffset, unsigned value )
{
    static struct ACQ32_RW_DEF def;

    def.offset = woffset*2;
    def.data.w = value;

    int rc = ioctl( Board::GetFileId(), ACQ32_IOS_WRITE16, &def );

    ASSERT( rc == 0 );

    return value;
}
#endif

static bool initAMDFlash()
{
    static AMDFlash AMD_flash_leader;

    AMD_flash_leader.Register();
    return true;                      // it's OK
}
extern "C" bool __AMD_FLASH_INIT;

bool __AMD_FLASH_INIT = initAMDFlash();







