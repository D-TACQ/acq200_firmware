/*****************************************************************************
 *
 * File:
 *
 * $RCSfile: DtacqServer.java,v $
 * 
 * Copyright (C) 1999 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: DtacqServer.java,v 1.5 2000/05/28 10:31:41 pgm Exp $
 * $Log: DtacqServer.java,v $
 * Revision 1.5  2000/05/28 10:31:41  pgm
 * streaming works passibly
 *
 * Revision 1.4  2000/05/04 18:50:06  pgm
 * Debug class added
 *
 * Revision 1.3  2000/04/16 10:59:22  pgm
 * data upload works!
 *
 * Revision 1.2  2000/04/15 13:45:31  pgm
 * added headers
 *
 * Revision 1.1  1999/10/22 16:26:49  pgm
 * first entry to cvs
 *
 *
\*****************************************************************************/



/*****************************************************************************
 *
 * File:DtacqServer.java
 *
 * $RCSfile: DtacqServer.java,v $
 * 
 * Copyright (C) 1999 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description: defines DtacqServer class
 * listens on a given PORT, starts thread on accept
 *
 * $Id: DtacqServer.java,v 1.5 2000/05/28 10:31:41 pgm Exp $
 * $Log: DtacqServer.java,v $
 * Revision 1.5  2000/05/28 10:31:41  pgm
 * streaming works passibly
 *
 * Revision 1.4  2000/05/04 18:50:06  pgm
 * Debug class added
 *
 * Revision 1.3  2000/04/16 10:59:22  pgm
 * data upload works!
 *
 * Revision 1.2  2000/04/15 13:45:31  pgm
 * added headers
 *
 * Revision 1.1.1.1  2000/04/13 06:34:06  pgm
 * recreate in new tree
 *
 * Revision 1.1  2000/04/09 06:38:58  pgm
 * imported from NT
 *
 * Revision 1.1  1999/10/22 16:26:49  pgm
 * first entry to cvs
 *
 *
\*****************************************************************************/


import java.io.*;
import java.net.*;


class DtacqServerThread extends Thread {
    ServerSocket ssocket;
    InterpreterFactory interpreter_factory;
    int ithread;

    public DtacqServerThread( 
        int ithread,
        ServerSocket ssocket, InterpreterFactory interpreter_factory
	) throws IOException {

	    this.ssocket = ssocket;
	    this.interpreter_factory = interpreter_factory;
	    this.ithread = ithread;
    }

    public void run() {
	Socket tsocket = null;

	try {
	    while( true ) {
		tsocket = ssocket.accept();

		Debug.println( "Accepting:" + ithread );
		BufferedReader in = 
		    new BufferedReader(
		       	new InputStreamReader( tsocket.getInputStream() ) );
                   
	        Interpreter interpreter = 
		    interpreter_factory.create( tsocket.getOutputStream() );
		runTransaction( in, interpreter );
		tsocket.close();

		Debug.println( "Closing:" + ithread );
	    }
	} catch ( IOException e ) {
	    System.err.println( e );
	    try {
	        if ( tsocket != null ) tsocket.close();
		Debug.println( 1, "DtacqServerThread.run#close() OK" );
	    } catch ( IOException e2 ) {
		System.err.println ( "DtacqServerThread.run#close " + e2 );
	    }
	}
    }
    private void runTransaction( 
            BufferedReader in, Interpreter interpreter          ) {
	try {
	    while ( interpreter.processCommand( in.readLine() ) !=
		    Interpreter.FINISHED                            ){
		    Debug.println( 2, "DtacqServerThread.run() !FINISHED" );
	    }
	    Debug.println( 1, "DtacqServerThread FINISH detected" );
	} catch ( IOException e ) {
	    System.err.println( "DtacqServerThread.run " + e );
        }
    }
}

public class DtacqServer extends Thread implements DtacqComms {

    Thread thread;

    public DtacqServer( int port, InterpreterFactory interpreter_fact, 
		        int nthreads ) 
        throws IOException
    {
	    ServerSocket s = new ServerSocket( port );
          
	    Debug.println( 2, "Server started on port " + port );

	    for ( int it = 1; it <= nthreads; ++it ) {
		    Thread t = new DtacqServerThread( it, s, interpreter_fact );
		    t.start();
	    }

    }
}









