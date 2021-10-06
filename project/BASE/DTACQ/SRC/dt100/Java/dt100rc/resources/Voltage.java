/*****************************************************************************
 *
 * File:
 *
 * Created by IntelliJ IDEA.
 * User: pgm
 * Date: May 27, 2002
 * Time: 7:57:58 PM
 *
 * $RCSfile: Voltage.java,v $
 * 
 * Copyright (C) 2002 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: Voltage.java,v 1.1 2002/05/27 19:41:06 pgm Exp $
 * $Log: Voltage.java,v $
 * Revision 1.1  2002/05/27 19:41:06  pgm
 * numeric combos R us
 *
 *
 *
\*****************************************************************************/
package resources;

import debug.Debug;

import java.io.Serializable;

public class Voltage implements Comparable, Serializable {
    float volts;

    public int compareTo(Object o) {
        Voltage ov = (Voltage)o;

        if ( ov.volts == volts ){
            return 0;
        }else if ( ov.volts < volts ){
            return -1;
        }else{
            return 1;
        }
    }
    public String toString() {
        String s = volts >= 0.0f? "+":"";
        return s + volts + " V";
    }

    public Voltage( FloatRange range, String s ){
        try {
            volts = range.clip( Float.parseFloat( s.replace( 'V', ' ' ).trim() ) );
        } catch ( NumberFormatException e ){
            Debug.println( "Voltage.NFE "+s );
            volts = 0.0f;
        }
    }

    public static Voltage[] makeArray( String[] strings ){
        FloatRange range = new FloatRange( -1000f, +1000f );
        Voltage[] voltages = new Voltage[strings.length];

        for ( int ii = strings.length-1; ii >= 0; --ii ){
            voltages[ii] = new Voltage( range, strings[ii] );
        }

        return voltages;
    }
}
