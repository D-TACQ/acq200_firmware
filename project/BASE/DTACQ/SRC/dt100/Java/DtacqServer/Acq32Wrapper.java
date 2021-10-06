/*****************************************************************************
 *
 * File:
 *
 * $RCSfile: Acq32Wrapper.java,v $
 * 
 * Copyright (C) 1999 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: Acq32Wrapper.java,v 1.6 2002/04/20 12:37:27 pgm Exp $
 * $Log: Acq32Wrapper.java,v $
 * Revision 1.6  2002/04/20 12:37:27  pgm
 * fixes file handle leak
 *
 * Revision 1.5  2001/01/14 17:52:32  pgm
 * use RandomAccessFile for multi thread safety
 *
 * Revision 1.4  2000/05/28 10:31:41  pgm
 * streaming works passibly
 *
 * Revision 1.3  2000/05/04 18:50:06  pgm
 * Debug class added
 *
 * Revision 1.2  2000/04/15 13:45:31  pgm
 * added headers
 *
 * Revision 1.1  1999/10/22 16:26:49  pgm
 * first entry to cvs
 *
 *
\*****************************************************************************/





import java.io.*;

public class Acq32Wrapper {
    private final String devname;

    public static void main( String[] args ){
	String devname;

	if ( args.length != 1 ){
	    devname = "/dev/acq32/acq32.1.m1";
	}else{
	    devname = args[0];
	}

	try {
	    Acq32Wrapper wrapper = new Acq32Wrapper( devname );

	    wrapper.run( System.in, System.out );
	}
	catch ( IOException e ) { 
	    System.err.println( e ); 
	}
    }

    
    RandomAccessFile io_stream;

    public Acq32Wrapper( String devname ) throws IOException
    {
	this.devname = devname;
	io_stream = new RandomAccessFile( devname, "rw" );
    }

    public void close() {
	Debug.println( 2, "Acq32Wrapper.close() " + devname );
	try {
	    io_stream.close();
	} catch ( IOException e ) {
	    System.err.println( "Acq32Wrapper.close() " + e );
	}
    }
    public void run( InputStream in, OutputStream out ) throws IOException {

	while ( true ){
	    byte[] buffer = new byte[256];

	    int bytes_read = in.read( buffer );

	    if ( bytes_read == -1 ){
		break;
	    }

	    out.write( sendCommand( buffer, bytes_read ) );
	}
    }

    public byte[] sendCommand( byte[] command, int ncommand ) 
	throws IOException {
	
	io_stream.write( command, 0, ncommand );

	byte[] rbuf   = new byte[256];	
	int bytes_read = io_stream.read( rbuf );

	if ( bytes_read == -1 ){
	    throw new IOException();
	}

	byte result[] = new byte[bytes_read];
	    
	for ( int ibyte = 0; ibyte != bytes_read; ++ibyte ){
	    result[ibyte] = rbuf[ibyte];
	}
	return result;
    }

    public void write( byte[] data, int ndata ) throws IOException {
	io_stream.write( data, 0, ndata );
    }
    public int read( byte[] data, int ndata ) throws IOException {
	return io_stream.read( data, 0, ndata );
    }
}




















