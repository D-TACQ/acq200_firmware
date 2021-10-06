/*****************************************************************************
 *
 * File:
 *
 * Created by IntelliJ IDEA.
 * User: pgm
 * Date: May 27, 2002
 * Time: 12:35:59 PM
 *
 * $RCSfile: ResourceBlock.java,v $
 *
 * Copyright (C) 2002 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: ResourceBlock.java,v 1.2 2002/05/27 10:25:11 pgm Exp $
 * $Log: ResourceBlock.java,v $
 * Revision 1.2  2002/05/27 10:25:11  pgm
 * std headers throughout
 *
 *
 *
\*****************************************************************************/


package resources;

import java.util.*;
import java.io.*;

public class ResourceBlock
{

    public void actOnResources( ResourceHandler rh ) {}
    public void printResources() {
        System.out.println( "null resource block" );   
    }
    public ResourceBlock() {}
    public ResourceBlock( String fname )
                throws FileNotFoundException, IOException {}
    
    public static void main( String[] args ) throws Exception {
        ResourceBlock rb = new ResourceBlock( args.length>0? args[0]:"resource.test" );            
        
        rb.printResources();
    }
}