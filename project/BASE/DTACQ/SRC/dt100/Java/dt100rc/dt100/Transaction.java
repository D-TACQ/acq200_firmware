/*****************************************************************************
 *
 * File:
 *
 * Created by IntelliJ IDEA.
 * User: pgm
 * Date: May 27, 2002
 * Time: 12:35:59 PM
 *
 * $RCSfile: Transaction.java,v $
 *
 * Copyright (C) 2002 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: Transaction.java,v 1.6.4.1 2004/06/26 19:59:09 pgm Exp $
 * $Log: Transaction.java,v $
 * Revision 1.6.4.1  2004/06/26 19:59:09  pgm
 * serious grief failed to serialize stream Combos. fake it by introducing defaults MF Stats in but non-func
 *
 * Revision 1.6  2002/05/27 10:25:11  pgm
 * std headers throughout
 *
 *
 *
\*****************************************************************************/

package dt100;

import java.io.*;
import java.awt.EventQueue;

import debug.*;

class DtacqUnexpectedResponseException extends DtacqException {
    public DtacqUnexpectedResponseException( String msg ) {
        super( msg );
    }
}
class DtacqNackException extends DtacqException {
    public DtacqNackException( String msg ) {
        super( msg );
        System.err.println( "DtacqNackException:" + msg );
    }
}

public abstract class Transaction
{
    private final String ack;
    protected final String COMMAND_PREFIX;
    
    protected Runnable callback;
    
    public Transaction( String ack ) {
        this.ack = ack;
        this.COMMAND_PREFIX = "";
    }
    public Transaction( String ack, String COMMAND_PREFIX ) {
       // was BoardTransaction
       this.ack = ack;
       this.COMMAND_PREFIX = COMMAND_PREFIX;
    }
    public void setCallback( Runnable callback ) {
        this.callback = callback;
    }
    
    public abstract void encode( PrintWriter output );
    
    public void decode( DataInputStream input )
            throws DtacqException, IOException {
        String response = "";
        
        try {
            do {
                // @@todo WORKTODO - replace with BufferedReader
                while ( (response = input.readLine()) == null ){
                    Thread.yield();
                }
                Logger.println( "response: len:" + response.length() + "" + response );
            } while ( response.length() <= 2 || response.length() > 250 );

            if ( response.startsWith( ack ) ){
                String residue = response.substring( ack.length() );
                    
                if ( !residue.startsWith( "ERROR" ) ){
                    onDecode( residue, input );  
                }else{
                    throw new DtacqNackException( residue );
                }
            }else{
                throw new DtacqUnexpectedResponseException( response );
            }
        }catch ( IOException e ){
            System.err.println( "Transaction.decode() Hit exception " + e );
            e.printStackTrace();
            throw e;
        }finally{
            if ( callback != null ){
                EventQueue.invokeLater( callback );         // callback better deal with null input:-)
            }
        }
    }
    protected void onDecode( String residue, DataInputStream input ) 
        throws DtacqException, IOException {
        
    }    
}