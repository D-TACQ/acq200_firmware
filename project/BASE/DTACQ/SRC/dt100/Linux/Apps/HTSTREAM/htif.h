/*****************************************************************************
 *
 * File: htif.h
 *
 * $RCSfile: htif.h,v $
 * 
 * Copyright (C) 2001 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: htif.h,v 1.2 2002/04/10 13:14:32 pgm Exp $
 * $Log: htif.h,v $
 * Revision 1.2  2002/04/10 13:14:32  pgm
 * compiles
 *
 * Revision 1.1  2002/04/10 12:36:47  pgm
 * *** empty log message ***
 *
 * Revision 1.1  2002/04/02 10:25:14  pgm
 * *** empty log message ***
 *
 *
 * Revision 1.1  1999/10/22 16:26:49  pgm
 * first entry to cvs
 *
 *
\*****************************************************************************/


class ContiguousDmaBuffer {
  // factory pattern
 protected:
    ContiguousDmaBuffer() {}
    ~ContiguousDmaBuffer() {}
    unsigned vaddr;
    unsigned paddr;
    unsigned length;

 public:
    unsigned getVaddr( void )   const { return vaddr; }
    unsigned getPaddr( void )   const { return paddr; }
    unsigned getLength ( void ) const { return length; }
    

 public:
    static ContiguousDmaBuffer& getBuffer( int board, unsigned length );
};


