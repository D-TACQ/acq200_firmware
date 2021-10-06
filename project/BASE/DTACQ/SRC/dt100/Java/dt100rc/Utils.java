/*****************************************************************************
 *
 * File:
 *
 * Created by IntelliJ IDEA.
 * User: pgm
 * Date: May 27, 2002
 * Time: 12:35:59 PM
 *
 * $RCSfile: Utils.java,v $
 *
 * Copyright (C) 2002 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: Utils.java,v 1.2 2002/05/27 10:25:10 pgm Exp $
 * $Log: Utils.java,v $
 * Revision 1.2  2002/05/27 10:25:10  pgm
 * std headers throughout
 *
 *
 *
\*****************************************************************************/

class Utils {
     static int expandUnitsKM( String km_string ) throws NumberFormatException {

        // converts string with nnnn [kM] integer

        km_string.trim();
        int multiplier = 1;
        int multiplier_index;
        int km;

        if ( (multiplier_index = km_string.indexOf( "k" )) >= 0 ){
            multiplier = 1000;
            km_string = km_string.substring( 0, multiplier_index );
        }else if ( (multiplier_index = km_string.indexOf( "M" )) >= 0 ){
            multiplier = 1000000;
            km_string = km_string.substring( 0, multiplier_index );
        }
        km_string = km_string.trim();
        km = Integer.parseInt( km_string ) * multiplier;
        return km;
    }   
}