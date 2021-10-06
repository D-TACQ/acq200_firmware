/*****************************************************************************
 *
 * File:
 *
 * Created by IntelliJ IDEA.
 * User: pgm
 * Date: May 27, 2002
 * Time: 12:35:59 PM
 *
 * $RCSfile: DefaultResourceBlock.java,v $
 *
 * Copyright (C) 2002 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: DefaultResourceBlock.java,v 1.4 2002/05/27 10:25:11 pgm Exp $
 * $Log: DefaultResourceBlock.java,v $
 * Revision 1.4  2002/05/27 10:25:11  pgm
 * std headers throughout
 *
 *
 *
\*****************************************************************************/

package resources;

import java.util.*;
import java.io.*;

public class DefaultResourceBlock extends ResourceBlock
{
    Set data;
    static boolean verbose;
    
    class Resource {
        String instance_name;
        String method_name;
        String arg;
        String line;
        
        void init( String instance_name, String method_name, String arg ) {
            this.instance_name = instance_name; 
            this.method_name = method_name;
            this.arg = arg;            
        }
        public Resource( String instance_name, String method_name, String arg ) {
            init( instance_name, method_name, arg );
        }
        public Resource( String line ) {
            StringTokenizer st = new StringTokenizer( line, ".() " );   
            String [] args = new String[3];
        
            for( int itok = 0; itok != 3 && st.hasMoreTokens(); ++itok ){
                String delims = itok==0||itok==1? ".() ": "\"()";
                args[itok] = st.nextToken( delims );
            }
            init( args[0], args[1], args[2] );
        }
        public String getInstanceName() { return instance_name; }
        public String getMethodName()   { return method_name; }
        public String getArg()          { return arg; }
        public String getLine()         { return line; }
        
        public String toString() {
            return new String( "I:"+getInstanceName()+ 
                               " M:"+ getMethodName()+ 
                               " A:"+getArg() );   
        }
    }
    void vprint( String s ) {
        if ( verbose ) System.out.println( s );
    }
    void buildResource( String line ) {
        data.add( new Resource( line ) );                
    }
    void swallow( BufferedReader br ) throws IOException {
        String line;
        
        while ( (line = br.readLine()) != null ){
            vprint( line );
            if ( line.length() > 0 && !line.startsWith( "#" ) ){
                buildResource( line );      
            }
        }
    }

    public void actOnResources( ResourceHandler rh ) {
        Iterator it = data.iterator();
        
        while( it.hasNext() ){
            Resource r = (Resource)it.next();       
            try {
                rh.call( r.getInstanceName(), r.getMethodName(), r.getArg() );  
            }catch ( Exception e ) {
                System.out.println( "WARNING:resource action failed " + r + 
                                    " Exception " + e);   
            }
        }
    }
    public void printResources() {
        Iterator it = data.iterator();
        
        while( it.hasNext() ){
            Resource r = (Resource)it.next();            
            System.out.println( r );  
        }
    }
    public DefaultResourceBlock( String fname )
                throws FileNotFoundException, IOException {
        data = new HashSet();
        
        try {
            BufferedReader br = new BufferedReader(
                                new FileReader( fname ) );
            swallow( br );
        } catch (FileNotFoundException e ) {
            System.out.println( "ResourceBlock: file " + fname + " " + e );
            throw e;
        }
    }
    
    public static void main( String[] args ) throws Exception {
        verbose = true;
        ResourceBlock rb = new DefaultResourceBlock( args.length>0? args[0]:"resource.test" );            
        
        rb.printResources();
    }
}