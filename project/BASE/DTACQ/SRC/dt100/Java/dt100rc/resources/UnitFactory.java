/*****************************************************************************
 *
 * File:
 *
 * Created by IntelliJ IDEA.
 * User: pgm
 * Date: May 27, 2002
 * Time: 9:22:23 PM
 *
 * $RCSfile: UnitFactory.java,v $
 * 
 * Copyright (C) 2002 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: UnitFactory.java,v 1.2 2002/05/28 10:17:02 pgm Exp $
 * $Log: UnitFactory.java,v $
 * Revision 1.2  2002/05/28 10:17:02  pgm
 * *** empty log message ***
 *
 * Revision 1.1  2002/05/27 19:41:06  pgm
 * numeric combos R us
 *
 *
 *
\*****************************************************************************/
package resources;


// WORKTODO - let units auto register themselves, and supply own RangeClass
// given that we only have two types atm, this is OTT.

public class UnitFactory {
    static public Object newUnit( Range r, String s ) {
        if ( r instanceof VoltsRange ){
            return new Voltage( (VoltsRange)r, s );
        }else if ( r instanceof IntegerRange ){
            return new SampleCount( (IntegerRange)r, s );
        }
        return "WARNING - no conversion possible";
    }
}
