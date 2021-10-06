/*****************************************************************************
 *
 * File: EBSAFlashDevice.cpp: impl of flash device for EBSA
 *           
 *
 * $RCSfile: EbsaFlashDevice.cpp,v $
 * 
 * Copyright (C) 1999 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: EbsaFlashDevice.cpp,v 1.7 2001/05/06 18:47:20 pgm Exp $
 * $Log: EbsaFlashDevice.cpp,v $
 * Revision 1.7  2001/05/06 18:47:20  pgm
 * cleanup
 *
 * Revision 1.6  2001/01/02 12:35:02  pgm
 * top-down Make, release targets
 *
 * Revision 1.5  1999/10/11 10:34:56  pgm
 * Works at DTACQ
 *
 * Revision 1.4  1999/10/01 21:47:05  pgm
 * WORD rwrite fixup/frigs for 21285
 *
 * Revision 1.3  1999/10/01 09:18:06  pgm
 * fix leader instance bug
 *
 * Revision 1.2  1999/09/30 20:45:12  pgm
 * pfmu hacked to pgm archi satis
 *
 * Revision 1.1  1999/09/30 10:14:33  pgm
 * pfmu - enhanced fmu utility
 *
 *
\*****************************************************************************/



#include "local.h"

#include <stdio.h>

#include "FlashDevice.h"


class EbsaFlash: public FlashDevice
// actually Intel, but who cares ...
{
    static const BlockDef m_block_def[];

    volatile unsigned *const BASE() { return m_p_base; }

protected:
    bool OnInitIfPresent()
    {
        PDEBUG( "EbsaFlash:looking for Intel flash\n" );
        bool found_it = false;

        BASE()[0] = 0xffffffff;
        BASE()[0] = 0x90909090;
        
        unsigned man_id = BASE()[0];
      
        if ( man_id == 0x89898989 ){
            unsigned chip_id = BASE()[1];

            if ( !(chip_id < 0xa1a1a1a1) ){
                found_it = true;
            }
        }
    /* Make sure the flashPtr is back in normal mode so 
       that we can do user I/O via firmware in flash */

	BASE()[0] = 0xffffffff;

        PDEBUG( "EbsaFlash: %s Intel flash\n",
               found_it? "FOUND": "NOT found" );
        return found_it;
    }
    EbsaFlash( unsigned *const p_base, unsigned*const p_csr  ):
        FlashDevice( m_block_def, p_base, p_csr )
    {

    }


public:
    EbsaFlash() :
        FlashDevice( m_block_def )
    {
        m_ident = "EbsaFlashDevice";
    }
    virtual FlashDevice* Clone( unsigned *const p_base, unsigned *const p_csr )
    {
        return new EbsaFlash( p_base, p_csr );
    }

    virtual unsigned read( int offset );
    virtual int write( int offset, unsigned data );
    virtual bool DeleteBlock( int iblock );
};

#define LONGS( bytes )  ((bytes)/sizeof(long))

const FlashDevice::BlockDef EbsaFlash::m_block_def[] = {
  { OFFSET(  0*256*KB ), LONGS( 256*KB ) },
  { OFFSET(  1*256*KB ), LONGS( 256*KB ) },
  { OFFSET(  2*256*KB ), LONGS( 256*KB ) },
  { OFFSET(  3*256*KB ), LONGS( 256*KB ) },
  { OFFSET(  4*256*KB ), LONGS( 256*KB ) },
  { OFFSET(  5*256*KB ), LONGS( 256*KB ) },
  { OFFSET(  6*256*KB ), LONGS( 256*KB ) },
  { OFFSET(  7*256*KB ), LONGS( 256*KB ) },
  { OFFSET(  8*256*KB ), LONGS( 256*KB ) },
  { OFFSET(  9*256*KB ), LONGS( 256*KB ) },
  { OFFSET( 10*256*KB ), LONGS( 256*KB ) },
  { OFFSET( 11*256*KB ), LONGS( 256*KB ) },
  { OFFSET( 12*256*KB ), LONGS( 256*KB ) },
  { OFFSET( 13*256*KB ), LONGS( 256*KB ) },
  { OFFSET( 14*256*KB ), LONGS( 256*KB ) },
  { OFFSET( 15*256*KB ), LONGS( 256*KB ) },
  {}
};


unsigned EbsaFlash::read( int offset )
{
    D21285OffsetFixup( offset );

    return BASE()[offset];
}
int EbsaFlash::write( int offset, unsigned data )
{
    D21285OffsetFixup( offset );

    BASE()[offset] = 0x40404040;

    BASE()[offset] = data;

    unsigned status;

    do {
        BASE()[offset] = 0x70707070;
        status = BASE()[offset]; 
    }
        while( (status&0x80808080) != 0x80808080 );

    BASE()[0] = 0xffffffff;

    // maybe just 
    // error = (status&0x10101010) != 0 is better

    return (status&0x10101010) == 0;
}

bool EbsaFlash::DeleteBlock( int iblock )
{
    unsigned offset = GetBlockStart( iblock );

    BASE()[offset] = 0x20202020;
    BASE()[offset] = 0xd0d0d0d0;

    unsigned status;

    do {
        BASE()[offset] = 0x70707070;
        status = BASE()[offset];
    }
        while( (status&0x80808080) != 0x80808080 );

    BASE()[0] = 0xffffffff;

    return (status&0x20202020) ==0;
}

static bool initEbsaFlash()
{
    static EbsaFlash ebsa_flash_leader;

    ebsa_flash_leader.Register();

    return true;                      // it's OK
}
extern "C" bool __EBSA_FLASH_INIT;

//bool __EBSA_FLASH_INIT = initEbsaFlash();




