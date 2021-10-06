/*****************************************************************************
 *
 * File:
 *
 * Created by IntelliJ IDEA.
 * User: pgm
 * Date: May 27, 2002
 * Time: 12:35:59 PM
 *
 * $RCSfile: DefaultResourceHandler.java,v $
 *
 * Copyright (C) 2002 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: DefaultResourceHandler.java,v 1.2 2002/05/27 10:25:11 pgm Exp $
 * $Log: DefaultResourceHandler.java,v $
 * Revision 1.2  2002/05/27 10:25:11  pgm
 * std headers throughout
 *
 *
 *
\*****************************************************************************/

package resources;

import java.util.*;
import java.lang.*;
import java.lang.reflect.*;

class ResourceTester {
    int foo;
    
    public void setFoo( String s ) { this.foo = Integer.parseInt( s ); }
    public void setAsInt( int i )  { this.foo = i; }
    public void clear( boolean doit ) { if ( doit ) this.foo = 0; }
    public void set( boolean setit  ) { if ( setit ) this.foo = 1; }
    public String toString() { return String.valueOf( foo ); }
}

class ObjectNotFoundException extends Exception {
    public ObjectNotFoundException( String s ) {
        super( s );
    }
}
class MethodNotFoundException extends Exception {
    public MethodNotFoundException( String s ) {
        super( s );
    }
}
class PramTypeNotSupportedException extends Exception {
    public PramTypeNotSupportedException( Class c ) {
        super( c.getName() ); 
    }
}
class BadNumArgsException extends Exception {
    public BadNumArgsException( int i ) {
        super( String.valueOf( i ) );   
    }
}

public class DefaultResourceHandler implements ResourceHandler
{
    TreeMap map;
    HashMap blacklist;
    DefaultResourceHandler() {
        map = new TreeMap();
        blacklist = new HashMap();
    }
    static ResourceHandler _instance;
    
    public void register( Object object, String name ) {
        map.put( name, object );
    }
    public boolean isBlacklisted( java.awt.Component control ) {
        return blacklist.containsKey( control );   
    }
    public void setEnabled( java.awt.Component control, boolean enable ) {
        if ( !blacklist.containsKey( control ) ){
            control.setEnabled( enable );
        }
    }
    public void call( String name, String method, String value )
        throws Exception {
        if ( map.containsKey( name ) ){
            Object o = map.get( name );
            Class c = o.getClass();
            Method[] m = c.getMethods();
                                
            if ( method.compareTo( "INHIBIT" ) == 0 ){
                blacklist.put( o, "INHIBIT" );
                return;
            }
            for ( int ii = 0; ii != m.length; ++ii ){
                if ( method.compareTo( m[ii].getName() ) == 0 ){
                    Class[] prams_def = m[ii].getParameterTypes();
                    Object[] args = new Object[1];
                  
                    if ( prams_def.length != 1 ){
                        throw new BadNumArgsException( prams_def.length );
                    }else if ( prams_def[0].isInstance( value ) ){
                        args[0] = value;
                    }else if ( prams_def[0].getName().compareTo( "int" ) == 0 ) {
                        args[0] = new Integer( value );
                    }else if ( prams_def[0].getName().compareTo( "boolean" ) == 0 ) {
                        args[0] = new Boolean( value );
                    }else{
                        throw new PramTypeNotSupportedException(prams_def[0]);
                    }
                    m[ii].invoke( o, args );
                    return;
                }
            }
            throw new MethodNotFoundException( method );
        }else{
            throw new ObjectNotFoundException( name );
        }
    }
    public static ResourceHandler instance() {
        if ( _instance == null ){
            _instance = new DefaultResourceHandler();
        }
        return _instance;
    }
    
    static void testcall( Object o, String oname, String method, String value ) 
        throws Exception 
    {
        System.out.println( "Try "+oname+"."+method+"( "+value+" )" );
        DefaultResourceHandler.instance().call( oname, method, value );      
        System.out.println( "Success " +oname+" holds "+o.toString() );
    }
    public static void main( String[] args ) throws Exception {
        System.out.println( "pgmwashere" );   
        
        ResourceTester test1 = new ResourceTester();
        DefaultResourceHandler.instance().register( test1, "test1" );
        ResourceTester test2 = new ResourceTester();
        DefaultResourceHandler.instance().register( test2, "test2" );
       
        testcall( test1, "test1", "setFoo", "37" );
        testcall( test2, "test2", "setFoo", "1234" );
        testcall( test1, "test1", "setAsInt", "22" );
        testcall( test1, "test1", "clear",    "false" );
        testcall( test1, "test1", "clear",    "true" );
        testcall( test1, "test1", "set",    "0" );
        testcall( test1, "test1", "set",    "1" );
    }
}