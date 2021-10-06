/*****************************************************************************
 *
 * File:
 *
 * Created by IntelliJ IDEA.
 * User: pgm
 * Date: May 27, 2002
 * Time: 9:32:23 PM
 *
 * $RCSfile: IntegerRange.java,v $
 * 
 * Copyright (C) 2002 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: IntegerRange.java,v 1.2 2002/05/28 07:46:32 pgm Exp $
 * $Log: IntegerRange.java,v $
 * Revision 1.2  2002/05/28 07:46:32  pgm
 * numeric ranges implemented
 *
 * Revision 1.1  2002/05/27 19:41:06  pgm
 * numeric combos R us
 *
 *
 *
\*****************************************************************************/
package resources;

import java.io.Serializable;

public class IntegerRange implements Range, Serializable {
    public final int rmin;
    public final int rmax;

    public int clip( int rx ){
        if ( rx > rmax ){
            return rmax;
        }else if ( rx < rmin ){
            return rmin;
        }else{
            return rx;
        }
    }
    public IntegerRange( int rmin, int rmax ) {
        this.rmin = rmin;
        this.rmax = rmax;
    }

    static final IntegerRange maxRange = new IntegerRange( Integer.MIN_VALUE, Integer.MAX_VALUE );
}
