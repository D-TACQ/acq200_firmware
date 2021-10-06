/*****************************************************************************
 *
 * File:
 *
 * $RCSfile: DtacqComms.java,v $
 * 
 * Copyright (C) 1999 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: DtacqComms.java,v 1.1 2002/05/17 08:25:08 pgm Exp $
 * $Log: DtacqComms.java,v $
 * Revision 1.1  2002/05/17 08:25:08  pgm
 * subset needed for dt100rc
 *
 * Revision 1.3  2000/07/01 06:47:29  pgm
 * keeping cvs happy
 *
 * Revision 1.2  2000/04/15 13:45:31  pgm
 * added headers
 *
 * Revision 1.1  1999/10/22 16:26:49  pgm
 * first entry to cvs
 *
 *
\*****************************************************************************/

package dtacqserver;

// DtacqComms.java - defines client server comms

public interface DtacqComms {
        static final int PORT = 0xd100;   
        
        static final String SOFT_TRANSIENT = "SOFT_TRANSIENT";
        static final String GATED_TRANSIENT= "GATED_TRANSIENT";
        static final String GATED_CONTINUOUS="GATED_CONTINUOUS";
}