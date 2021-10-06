/*****************************************************************************
 *
 * File: mbox.h
 *
 * $RCSfile: mbox.h,v $
 * 
 * Copyright (C) 2001 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: mbox.h,v 1.3 2002/08/12 10:56:07 pgm Exp $
 * $Log: mbox.h,v $
 * Revision 1.3  2002/08/12 10:56:07  pgm
 * streaming works, display from bigdump buf
 *
 * Revision 1.2  2002/04/10 18:23:11  pgm
 * first runner
 *
 * Revision 1.1  2002/04/10 15:00:04  pgm
 * *** empty log message ***
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
 * defines interface to mailbox
 */

extern int verbose;

#define PRINTF( n ) if ( verbose>=(n) ) printf

typedef unsigned u32;        // define a 32 bit quantity
struct _Mailboxes; // internal structure

class Mailboxes {
    _Mailboxes* m;

 public:
    int set( int ibox, u32 value );      
// set a mail mbox register: ibox {0..3}. return 0 on success
    u32 get( int ibox );      
// get contents of mailbox register: ibox {0..3}

    int setBits( int ibox, u32 bits_to_set );
// just set these bits

    int clrBits( int ibox, u32 bits_to_clr );
// just clr these bits

    int setField( int ibox, u32 field_mask, u32 field_value );
// set the field in the register only


    u32 pollBits( int ibox, u32 mask, u32 goal );
// return when any of the bits become true

    int pollAck();
// polls regular ack from acq32

    void showLastWrites();

    void print();
    void printerr();

    int equals( Mailboxes& previous );

    Mailboxes( int iboard );
// iboard {1..3}. return mapping on success

    // doesn't really belong to mailbox, but these funcs result in dev ioctl()
    void validateBigBuffer( unsigned nsamples );
    void maskInts( unsigned mask );
    void enableInts( unsigned mask );
};


