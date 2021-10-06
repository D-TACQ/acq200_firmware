/*****************************************************************************
 *
 * File:
 *
 * $RCSfile: Interpreter.java,v $
 * 
 * Copyright (C) 1999 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: Interpreter.java,v 1.7 2002/04/20 12:37:27 pgm Exp $
 * $Log: Interpreter.java,v $
 * Revision 1.7  2002/04/20 12:37:27  pgm
 * fixes file handle leak
 *
 * Revision 1.6  2002/04/20 09:07:10  pgm
 * close stream on BYE, hack CommandLine
 *
 * Revision 1.5  2000/07/01 06:47:29  pgm
 * keeping cvs happy
 *
 * Revision 1.4  2000/05/04 18:50:06  pgm
 * Debug class added
 *
 * Revision 1.3  2000/04/16 10:59:23  pgm
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



import java.io.*;
import java.util.*;



public abstract class Interpreter {

    protected OutputStream out;
    protected PrintWriter print;

    static public final boolean FINISHED = false;
    
    public Interpreter ( OutputStream out ) {
	    this.out = out;
	    print = new PrintWriter(
		            new BufferedWriter( 
			            new OutputStreamWriter( out )
			        ), true
		        );
		signOn();
    }
	
    protected void signOn() {
        print.println( "Interpreter" );   
    }
    public boolean processCommand( String command ) 
        throws IOException {
    // returns FINISHED when finished
    
	    if ( command.equals( "bye" ) ){
		onClose();
	        Debug.println( 1, "bye byeee" );
	        return FINISHED;
	    }else{
	        return onCommand( command );
	    }
    }

    protected abstract void onClose();
    protected boolean onCommand( String command ) {
	// default impl, subclass should override
    	
	    System.out.println( "Interpreter.onCommand " + command );
	    print.println( "Interpreter.onCommand " + command );
	    return !FINISHED;
    }

    protected boolean onCommand( CommandLine cl ) {
	    return !FINISHED;
    }
}
















