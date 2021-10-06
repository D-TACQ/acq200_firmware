/*****************************************************************************
 *
 * File:
 *
 * Created by IntelliJ IDEA.
 * User: pgm
 * Date: May 27, 2002
 * Time: 12:35:59 PM
 *
 * $RCSfile: GetTransaction.java,v $
 *
 * Copyright (C) 2002 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: GetTransaction.java,v 1.3 2002/05/27 10:25:11 pgm Exp $
 * $Log: GetTransaction.java,v $
 * Revision 1.3  2002/05/27 10:25:11  pgm
 * std headers throughout
 *
 *
 *
\*****************************************************************************/

package dt100;

import java.io.*;
import debug.*;

public class GetTransaction extends Transaction implements Query {
    
    final String query;
    
    protected GetTransaction( ResultCallback callback, String query ) {
        super( "ACQ32:", "acqcmd" );
        setCallback( callback );
        this.query = query;
    }
    public void encode( PrintWriter output ) {
        String s = COMMAND_PREFIX + " " + query;
        output.println( s ); 
        Logger.println( "get:" + s );
    }
    protected void onDecode( String residue, DataInputStream input ) 
        throws DtacqException, IOException {
           ((ResultCallback)callback).setResult( residue );
    }
    public Object getValue() {
        return callback;    // &&&PGM a bit of a frig!!   
    }
    
    static public GetTransaction state( ResultCallback callback ) {
        return new GetTransaction( callback, Acq32BusProt.getState );   
    }
    static public GetTransaction dio( ResultCallback callback ) {
        return new GetTransaction( callback, Acq32BusProt.getDIO );  
    }
    static public GetTransaction voltsRange( ResultCallback callback ) {
        return new GetTransaction( callback, Acq32BusProt.getVoltsRange );  
    }
    static public GetTransaction numChannels( ResultCallback callback ) {
        return new GetTransaction( callback, Acq32BusProt.getNumChannels );
    }
    static public GetTransaction availableChannels( ResultCallback callback ) {
        return new GetTransaction( callback, Acq32BusProt.getAvailableChannels );
    }
    static public GetTransaction numSamples( ResultCallback callback ) {
        return new GetTransaction( callback, Acq32BusProt.getNumSamples );
    }
}
