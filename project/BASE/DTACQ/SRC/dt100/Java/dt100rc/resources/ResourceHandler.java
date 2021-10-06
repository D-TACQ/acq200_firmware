/*****************************************************************************
 *
 * File:
 *
 * Created by IntelliJ IDEA.
 * User: pgm
 * Date: May 27, 2002
 * Time: 12:35:59 PM
 *
 * $RCSfile: ResourceHandler.java,v $
 *
 * Copyright (C) 2002 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: ResourceHandler.java,v 1.3 2002/05/27 10:25:11 pgm Exp $
 * $Log: ResourceHandler.java,v $
 * Revision 1.3  2002/05/27 10:25:11  pgm
 * std headers throughout
 *
 *
 *
\*****************************************************************************/

package resources;


public interface ResourceHandler
{
    public void register( Object object, String name );
    public boolean isBlacklisted( java.awt.Component control );
    public void setEnabled( java.awt.Component control, boolean enable );
    public void call( String name, String method, String value )
        throws Exception;
}