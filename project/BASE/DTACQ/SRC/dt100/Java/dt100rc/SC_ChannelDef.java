/*****************************************************************************
 *
 * File:
 *
 * Created by IntelliJ IDEA.
 * User: pgm
 * Date: May 27, 2002
 * Time: 12:35:59 PM
 *
 * $RCSfile: SC_ChannelDef.java,v $
 *
 * Copyright (C) 2002 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: SC_ChannelDef.java,v 1.2 2002/05/27 10:25:10 pgm Exp $
 * $Log: SC_ChannelDef.java,v $
 * Revision 1.2  2002/05/27 10:25:10  pgm
 * std headers throughout
 *
 *
 *
\*****************************************************************************/

import java.io.Serializable;


public class SC_ChannelDef implements Serializable {
    int channel;
    int g1;
    int g2;
    int v_ex;
        
    public SC_ChannelDef( int channel ) {
            this.channel = channel;
            g1 = 1;
            g2 = 1;
            v_ex = 1;
    }
    public int getChannel() { return channel; }
    public int getG1()      { return g1; }
    public int getG2()      { return g2; }
    public int getV_EX()    { return v_ex; }
        
    public int get( int icol ) {
        switch( icol ){
            case 0: return getChannel();
            case 1: return getG1();
            case 2: return getG2();
            case 3: return getV_EX();
            default:
                System.err.println( "ASSERT" );
                return 0;
        }
    }
    public int set( int icol, String value ) {
            
        int ivalue = Integer.parseInt(value);
            
        switch( icol ){
            case 1: return setG1( ivalue );
            case 2: return setG2( ivalue );
            case 3: return setV_EX( ivalue );
            case 0:
            default:
                System.err.println( "ASSERT" );
                return 0;
        }
    }
        
    public int setG1( int g1 )      { return this.g1 = g1; }
    public int setG2( int g2 )      { return this.g2 = g2; }
    public int setV_EX( int v_ex )  { return this.v_ex = v_ex; }
        
    public String toString() { return " "+channel+" "+g1+" "+g2+" "+v_ex; }
}
    