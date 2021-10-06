/*****************************************************************************
 *
 * File:
 *
 * Created by IntelliJ IDEA.
 * User: pgm
 * Date: May 27, 2002
 * Time: 12:35:59 PM
 *
 * $RCSfile: ResultCallback.java,v $
 *
 * Copyright (C) 2002 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: ResultCallback.java,v 1.3 2002/05/27 10:25:11 pgm Exp $
 * $Log: ResultCallback.java,v $
 * Revision 1.3  2002/05/27 10:25:11  pgm
 * std headers throughout
 *
 *
 *
\*****************************************************************************/

package dt100;

public abstract class ResultCallback implements Runnable {
    
    protected String result;      // "<status code > <residue>"
    String residue;
    int status;
    
    public void setResult( String result ) {
        this.result = result;
        
        int ispace;
        
        if ( (ispace = result.indexOf( " " )) > 0 ) {
            try {
                status = Integer.parseInt( result.substring( 0, ispace ) );
            } catch( java.lang.NumberFormatException e ) {
                status = 0;
            }
            
            residue = result.substring( ispace+1 );
        }
        
    }
    public int getStatus() {
        return status;        
    }
    public String getResultString() {
        return residue;
    }
}