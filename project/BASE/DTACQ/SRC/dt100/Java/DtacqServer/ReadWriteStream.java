/*****************************************************************************
 *
 * File:
 *
 * $RCSfile: ReadWriteStream.java,v $
 * 
 * Copyright (C) 1999 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: ReadWriteStream.java,v 1.5 2002/04/20 08:50:12 pgm Exp $
 * $Log: ReadWriteStream.java,v $
 * Revision 1.5  2002/04/20 08:50:12  pgm
 * nodos
 *
 * Revision 1.3  2000/04/18 09:45:41  pgm
 * repaired channel comms, primitive status rep
 *
 * Revision 1.2  2000/04/15 13:45:31  pgm
 * added headers
 *
 * Revision 1.1  1999/10/22 16:26:49  pgm
 * first entry to cvs
 *
 *
\*****************************************************************************/



// ReadWriteStream.java

import java.net.*;
import java.io.*;

public abstract class ReadWriteStream {
    public abstract PrintWriter getOutput();
    public abstract DataInputStream getInput(); // NB replace with own class
    
    public void close() {
        getOutput().close();
        try {
            getInput().close();
        } catch ( IOException e ) {
            System.out.println( "ReadWriteStream: close failed " + e );
        }
    }
}




