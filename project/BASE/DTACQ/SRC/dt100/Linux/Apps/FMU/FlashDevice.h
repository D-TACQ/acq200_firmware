/*****************************************************************************
 *
 * File: FlashDevice.h: universal flash device and manager for ACQ32, EBSA. 
 *             extendible to other flash types
 *
 * $RCSfile: FlashDevice.h,v $
 * 
 * Copyright (C) 1999 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: FlashDevice.h,v 1.9 2002/07/30 12:58:00 pgm Exp $
 * $Log: FlashDevice.h,v $
 * Revision 1.9  2002/07/30 12:58:00  pgm
 * more retries, test on exit
 *
 * Revision 1.8  2002/07/30 11:00:52  pgm
 * with retry on write
 *
 * Revision 1.7  2001/10/14 08:59:39  pgm
 * works on Alpha
 *
 * Revision 1.6  2000/09/30 09:50:08  pgm
 * MakeArmSpin call added for boot66 op
 *
 * Revision 1.5  1999/10/31 20:22:43  pgm
 * help and ident opts added
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
 * Revision 1.1  1999/09/30 10:14:33  pgm
 * pfmu - enhanced fmu utility
 *
 *
\*****************************************************************************/


#ifndef _PFMU_H_
#define _PFMU_H_

// convert to "dword" offset

#define OFFSET( byte_off )    ((byte_off)>>2)
#define WOFFSET( byte_off )   ((byte_off)>>1)


class FlashDevice;              // ABC to model a flash device

class Board
// models a pci board with onboard flash
{
    enum { MAXDEVS = 10 };          // plenty for now

    static FlashDevice* m_known_devices[MAXDEVS];    
    unsigned *const m_p_flash;
    unsigned *const m_p_csr;

public:
    Board( const char* device_name );
    // create an instance of Board for the device
    // throwsup if can't use device_name  

    FlashDevice* GetFlashDevice() const;

    static void Register( FlashDevice* flash_device );
    static int GetFileId();
};

class FlashDevice
// ABC for the flash device
{
protected:
    volatile unsigned *const m_p_base;
    volatile unsigned *const m_p_csr;

    const bool is_class_leader;          // class_leader used for cloning only

    struct BlockDef {
        unsigned startoff, nbytes;
    };

    const BlockDef *const m_pBlockDef;

    static unsigned alternate_id;
    static bool alternate_id_valid;

    unsigned const main_id;
protected:
    char* m_ident;

    void D21285WordFix( int woffset );

    void D21285OffsetFixup( int& offset );

    void D21285WordOffsetFixup( int& woffset );

    virtual bool OnInitIfPresent() = 0;  // Derived class MUST implement

    FlashDevice( 
        const BlockDef *const pBlockDef, 
        unsigned *const p_base, 
        unsigned *const p_csr = 0,
        unsigned _main_id = 0
    );
    unsigned GetId() const
    {
        if ( alternate_id_valid ){
            return alternate_id;
        }else{
            return main_id;
        }
    }  
    void MakeArmSpin();    

public:
    FlashDevice( const BlockDef *const pBlockDef );

    const char* GetIdent() { return m_ident; }

    virtual void ChipErase()
    {
        printf( "ERROR ChipErase() func not available for %s\n",
                   GetIdent() );
    }
    virtual FlashDevice* Clone( 
        unsigned *const p_base, unsigned *const p_csr 
    ) = 0;
    void Recycle();

    void Register();
   
    static void SetAlternateId( unsigned id )
    {
        alternate_id = id;
        alternate_id_valid = true;
    }    
    
    bool InitIfPresent();

    /*
     * basic access methods assume memory mapped access
     * can override if not memory mapped OR if more efficient block copy poss
     *
     * All offsets, lengths and counts in units of sizeof(int)
     */

 protected:
    enum { MAXRETRY = 8 };

    virtual int onWrite( int offset, unsigned data );
    // returns 1 on success
    virtual int onWrite( 
        int offset, const unsigned* client_buf, int nints
    );
    // the above seemed like a good feature, but is not used

 public:
    int write( int offset, unsigned data );
    // write with retry - returns 1 on success

    
    virtual unsigned read( int offset );

    virtual int read( 
        int offset, unsigned* client_buf, int nints
    );

    int GetNumBlocks() const;
    int GetBlockStart( int iblock ) const;
    int GetBlockLen( int iblock ) const;

    virtual bool DeleteBlock( int iblock ) = 0;
    // returns true on success


};


#endif


