/*****************************************************************************
 *
 * File:
 *
 * $RCSfile: DtacqClient.java,v $
 * 
 * Copyright (C) 1999 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: DtacqClient.java,v 1.7 2002/05/30 19:49:32 pgm Exp $
 * $Log: DtacqClient.java,v $
 * Revision 1.7  2002/05/30 19:49:32  pgm
 * sockets clean up
 *
 * Revision 1.3  2000/04/18 09:45:40  pgm
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



// DtacqClient.java

import java.net.*;
import java.io.*;
import java.util.*;
import java.text.*;

class DtacqClientImpl extends DtacqClient implements Serializable {
    private Socket socket;
    private DataInputStream in;
    private PrintWriter out;

    public PrintWriter getOutput() { return out; }
    public DataInputStream getInput() { return in; }

    public DtacqClientImpl(
                InetAddress addr,
                String hostname, 
                String stream_id    ) throws IOException {
                    
        super( hostname, stream_id );
        
        try {
            socket = 
            new Socket(addr, DtacqComms.PORT);
        } catch(IOException e) {
            System.out.println( "ERROR:failed to create socket " + e );
            throw e;
        }
        try {    
            in = new DataInputStream(
                    new BufferedInputStream( socket.getInputStream())
                );
            // Enable auto-flush:
            out = new PrintWriter(
                    new BufferedWriter( new OutputStreamWriter(
                                                socket.getOutputStream())
                    ), true
                );
                
            String sign_on = in.readLine();
            //System.out.println( "Signing on:" + sign_on );
        } catch(IOException e) {
            System.out.println( "ERROR: failed to open streams " + e );
            
            
            try {
                cleanup();
            } catch(IOException e2) {
                System.out.println( "ERROR: failed to close streams " + e2 );           
            }
            throw e;                // throw first error
        }
    }
    
    public void cleanup() throws IOException {
        socket.close();
    }
    // Otherwise the socket will be closed by
    // the run() method of the thread.
}

public abstract class DtacqClient extends ReadWriteStream {
    // Factory class: can't be instantiated directly, only thru connect()
  
    static Map connection_map = new HashMap();
          
    final String hostname;
    final String stream_id;
    
    static String makeKey( String hostname, String stream_id ) {
        return new String( hostname + ":" + stream_id );
    }
    String getKey() {
        return makeKey( hostname, stream_id );
    }
  
    protected DtacqClient( String hostname, String stream_id ) {
        this.hostname = hostname;
        this.stream_id = stream_id;
    }
    public static ReadWriteStream connect( 
                    String hostname, String stream_id ) 
        throws IOException 
    {
        
        String key = makeKey( hostname, stream_id );
        
        DtacqClientImpl the_client;
        
        if ( connection_map.containsKey( key ) ){
            
            the_client = (DtacqClientImpl)connection_map.get( key );
        }else {
            InetAddress addr;
            
            try {
                addr = InetAddress.getByName( hostname );
            } catch ( UnknownHostException e ) {
                System.out.println( "ERROR: Host:" + hostname + " " + e );
                throw e;
            }
            the_client = new DtacqClientImpl( addr, hostname, stream_id  ); 
            connection_map.put( key, the_client );
        }            
       
        return the_client;       
    }
    public static void close( ReadWriteStream client ) {
        DtacqClient dtacqClient = (DtacqClient)client;
        connection_map.remove( dtacqClient.getKey() );
	    client.close();
    }
    
    public static void main(String args[]) 
        throws IOException, InterruptedException {
            
          
        String hostname = args.length >= 1? args[0]: "harris";
        String stream_id = args.length>= 2? args[1]: "acq32.1.m1";
        
        ReadWriteStream rws = connect( hostname, stream_id );
       
        BufferedReader in = 
            new BufferedReader( new InputStreamReader( System.in ) );
            
        while(true) {
            String command = new String();
            String response = new String();
            
            command = in.readLine();
            
            if ( command == null ){
                break;
            }else if ( command.startsWith( "#" ) ){
                System.out.println( command );
            }else{
                rws.getOutput().println( command );
                response = rws.getInput().readLine();
                System.out.println( response );
                
                if ( command.equals( "bye" ) ){
                    break;
                }
            }
        }
    }
}
