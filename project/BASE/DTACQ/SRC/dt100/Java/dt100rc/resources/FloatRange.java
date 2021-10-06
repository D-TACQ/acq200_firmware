/*****************************************************************************
 *
 * File:
 *
 * Created by IntelliJ IDEA.
 * User: pgm
 * Date: May 27, 2002
 * Time: 7:58:44 PM
 *
 * $RCSfile: FloatRange.java,v $
 * 
 * Copyright (C) 2002 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: FloatRange.java,v 1.1 2002/05/27 19:41:06 pgm Exp $
 * $Log: FloatRange.java,v $
 * Revision 1.1  2002/05/27 19:41:06  pgm
 * numeric combos R us
 *
 *
 *
\*****************************************************************************/
package resources;

import java.io.Serializable;

public class FloatRange implements Serializable, Range {
    public final float rmin;
    public final float rmax;

    public float clip( float rx ){
        if ( rx > rmax ){
            return rmax;
        }else if ( rx < rmin ){
            return rmin;
        }else{
            return rx;
        }
    }
    public FloatRange( float rmin, float rmax ) {
        this.rmin = rmin;
        this.rmax = rmax;
    }
}
