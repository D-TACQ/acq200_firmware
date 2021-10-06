/*****************************************************************************
 *
 * File:
 *
 * Created by IntelliJ IDEA.
 * User: pgm
 * Date: May 27, 2002
 * Time: 12:35:59 PM
 *
 * $RCSfile: HexString.java,v $
 *
 * Copyright (C) 2002 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: HexString.java,v 1.2 2002/05/27 10:25:11 pgm Exp $
 * $Log: HexString.java,v $
 * Revision 1.2  2002/05/27 10:25:11  pgm
 * std headers throughout
 *
 *
 *
\*****************************************************************************/

package dt100;

public class HexString {
    // emulate sprintf( "%0Nx" )
    public static String leftFill( int x, int width ){
        String istr = Integer.toString( x, 16 );
        String fill = "";
        
        for ( int ifill = width - istr.length(); ifill > 0; --ifill ){ 
            fill += "0";
        }
        return fill + istr;        
    }
}
