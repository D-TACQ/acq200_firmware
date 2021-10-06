/*****************************************************************************
 *
 * File:
 *
 * Created by IntelliJ IDEA.
 * User: pgm
 * Date: May 27, 2002
 * Time: 9:32:07 PM
 *
 * $RCSfile: SampleCount.java,v $
 * 
 * Copyright (C) 2002 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: SampleCount.java,v 1.4.4.1 2004/06/27 12:04:55 pgm Exp $
 * $Log: SampleCount.java,v $
 * Revision 1.4.4.1  2004/06/27 12:04:55  pgm
 * StreamingControlPanel - first cut does OneSecond action
 *
 * Revision 1.4  2002/06/07 18:56:49  pgm
 * remove recurring exception on poll
 *
 * Revision 1.3  2002/05/28 10:16:52  pgm
 * reverse count
 *
 * Revision 1.2  2002/05/28 07:46:33  pgm
 * numeric ranges implemented
 *
 * Revision 1.1  2002/05/27 19:41:06  pgm
 * numeric combos R us
 *
 *
 *
\*****************************************************************************/
package resources;

import debug.Debug;

import java.io.Serializable;

public class SampleCount implements Comparable, Serializable {
    int sampleCount;
    String asString;

    public int compareTo(Object o) {
        // sort reverse order
        SampleCount ov = (SampleCount)o;

        if ( ov.sampleCount == sampleCount ){
            return 0;
        }else if ( ov.sampleCount < sampleCount ){
            return 1;
        }else{
            return -1;
        }
    }
    public String toString() {
        return asString;
    }

    public SampleCount( IntegerRange range, String s ){
        try {
            if ( s.length() == 0 ){
                sampleCount = range.clip( 0 );
                asString = "0";
            }else{
                sampleCount = range.clip( getCount( s ) );
                asString = s;
            }
        } catch ( NumberFormatException e ){
            Debug.println( "SampleCount.NFE \""+s+"\"" );
            sampleCount = range.rmin;
            asString = String.valueOf( range.rmin );
        }
    }

    public static SampleCount[] makeArray( String[] strings ){
        SampleCount[] sampleCounts = new SampleCount[strings.length];

        for ( int ii = strings.length-1; ii >= 0; --ii ){
            sampleCounts[ii] = new SampleCount( IntegerRange.maxRange, strings[ii] );
        }

        return sampleCounts;
    }

    public static int getCount( String item ){
        try {
            String trimmed = item.trim();
            String expanded;
            int ix;
            int mult;

            if ( trimmed.length() == 0 ){
                trimmed = "0";
            }
            if ( trimmed.equalsIgnoreCase( "max" ) ){
                return -1;
            }else if ( (ix = trimmed.indexOf( "k" )) > -1 ||
                       (ix = trimmed.indexOf( "K" )) > -1    ){
                expanded = trimmed.substring( 0, ix );
                mult = 1024;
            } else if ( (ix = trimmed.indexOf( "m" )) > -1 ||
                        (ix = trimmed.indexOf( "M" )) > -1     ){
                expanded = trimmed.substring( 0, ix );
                mult = 0x100000;
            } else {
                expanded = trimmed;
                mult = 1;
            }

            Debug.println(2, "SampleCount.getCount( "+item+" ) "+expanded+" * "+mult );
            int num_samples = Integer.parseInt( expanded.trim() ) * mult;
            return num_samples>0? num_samples: 0;
        } catch ( NumberFormatException e ){
            System.err.println( "sampleCount.getCount NFE "+item );
            return 0;
        }
    }
}
