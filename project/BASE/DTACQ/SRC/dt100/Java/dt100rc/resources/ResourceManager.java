/*****************************************************************************
 *
 * File:
 *
 * Created by IntelliJ IDEA.
 * User: pgm
 * Date: May 27, 2002
 * Time: 12:35:59 PM
 *
 * $RCSfile: ResourceManager.java,v $
 *
 * Copyright (C) 2002 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: ResourceManager.java,v 1.2 2002/05/27 10:25:11 pgm Exp $
 * $Log: ResourceManager.java,v $
 * Revision 1.2  2002/05/27 10:25:11  pgm
 * std headers throughout
 *
 *
 *
\*****************************************************************************/


package resources;

public class ResourceManager implements ResourceHandler
{
    static ResourceManager _instance;
    
    ResourceBlock rb;
    ResourceHandler handler;
    
    ResourceManager( ResourceBlock rb, ResourceHandler handler ) {
        this.rb = rb;
        this.handler = handler;
    }
    
    public void executeBlock() {
        rb.actOnResources( handler );
    }
    public void dumpHandler() {
        System.out.println( "dumpHandler()" );   
    }
    public void dumpBlock() {
        rb.printResources();
    }
    public static ResourceManager create( String resource_file ) {
        ResourceBlock rb;
        
     
        try {
            rb = new DefaultResourceBlock( resource_file );
        } catch ( java.io.FileNotFoundException e ) {
            System.out.println( "ResourceManager.create, unable to file file " + resource_file );
            rb = new ResourceBlock();
        } catch ( Exception e ) {
            System.out.println( "ResourceManager.create, exception " + e );
            rb = new ResourceBlock();
        }
        _instance = new ResourceManager( rb, DefaultResourceHandler.instance() );
        
        return _instance;
    }
    // ResourceHandler interface
    public void register( Object object, String name ){
        handler.register( object, name );
    }
    public boolean isBlacklisted( java.awt.Component control ) { 
        return handler.isBlacklisted( control ); 
    }
    public void setEnabled( java.awt.Component control, boolean enable ) {
        handler.setEnabled( control, enable );  
    }
    
    public void call( String name, String method, String value )
        throws Exception {
        handler.call( name, method, value );
    }   
}