package dt100;

import debug.Logger;

import java.io.PrintWriter;
import java.io.DataInputStream;
import java.io.IOException;

/**
 * Created by IntelliJ IDEA.
 * User: pgm
 * Date: Jun 1, 2004
 * Time: 8:39:22 AM
 * To change this template use File | Settings | File Templates.
 */
public class ShellTransaction extends Transaction {
    final String query;

    protected ShellTransaction( ResultCallback callback, String query ) {
        super( "", "" );
        setCallback( callback );
        this.query = query;
    }
    protected ShellTransaction( String query ) {
        super( "", "" );
        setCallback( null );
        this.query = query;
    }
    public void encode( PrintWriter output ) {
        String s = COMMAND_PREFIX + " " + query;
        output.println( query );
        Logger.println( "shell:" + s );
    }
    protected void onDecode( String residue, DataInputStream input )
        throws DtacqException, IOException {
            if (callback != null){
                ((ResultCallback)callback).setResult( residue );
            }
    }
    public Object getValue() {
        return callback;    // @todo PGM a bit of a frig!!
    }


    public static ShellTransaction setSampleReadStart(int sample_read_start ) {
        return new ShellTransaction( "set.sample_read_start " +  sample_read_start );
    }
}
