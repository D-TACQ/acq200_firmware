/*****************************************************************************
 *
 * File:
 *
 * $RCSfile: Channel.java,v $
 * 
 * Copyright (C) 1999 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: Channel.java,v 1.10.4.3 2004/09/03 08:42:26 pgm Exp $
 * $Log: Channel.java,v $
 * Revision 1.10.4.3  2004/09/03 08:42:26  pgm
 * B5217
 * Avoid channel duplication in Board st tree model and dt100 act on the SAME channels
 * then masking etc can work
 *
 * Revision 1.10.4.2  2004/05/05 19:30:29  pgm
 * throttle threads gives better perf
 *
 * Revision 1.10.4.1  2003/12/19 21:32:21  pgm
 * acq200, slot addressing
 *
 * Revision 1.10  2002/05/27 14:46:25  pgm
 * analog trigger runs
 *
 * Revision 1.9  2002/05/24 11:21:50  pgm
 * *** empty log message ***
 *
 * Revision 1.8  2002/04/18 12:53:43  pgm
 * table format is good
 *
 * Revision 1.7  2002/04/15 13:20:43  pgm
 * excellent status behaviour - ship it quick
 *
 * Revision 1.5  2001/06/17 21:49:47  pgm
 * save to file - looking good
 *
 * Revision 1.4  2001/06/16 21:11:20  pgm
 * better tree displays
 *
 * Revision 1.3  2000/10/28 20:02:58  pgm
 * improved reliability, l&f, messaging
 *
 * Revision 1.2  2000/09/07 00:15:23  pgm
 * supports DVM, shows correct channel mapping
 *
 * Revision 1.1.1.1  2000/06/03 05:36:25  pgm
 * fromQ
 *
 * Revision 1.1.1.1  2000/05/28 16:06:35  pgm
 * uses packages
 *
 * Revision 1.5  2000/05/21 14:21:53  pgm
 * stream panel added. also abortive experiment with packages
 *
 * Revision 1.4  2000/05/20 21:03:05  pgm
 * GridBag is good. Status resize, Clock gui etc
 *
 * Revision 1.3  2000/04/18 09:45:41  pgm
 * repaired channel comms, primitive status rep
 *
 * Revision 1.2  2000/04/13 19:50:37  pgm
 * added headers
 *
 * Revision 1.1  1999/10/22 16:26:49  pgm
 * first entry to cvs
 *
 *
\*****************************************************************************/

package dt100;

import debug.*;

import java.io.*;
import java.util.*;
//import dt100.*;

public class Channel implements Serializable
{
    int channel;
    Board board;
    Channel pair_member;
    boolean masked;     // masked (disabled) when true
    int nsamples;
    boolean uploading;

    AnalogTrigger atrig;

    public String toString() {
        String title = new String( "ch" );
        if ( channel < 10 ){
            title += "0";
        }
        title += channel;
        return title;
    }

    public AnalogTrigger getAnalogTrigger() { return atrig; }
    public void setAnalogTrigger( AnalogTrigger atrig ) {
        this.atrig = atrig;
    }

    public static int fromString( String title ) {
        String snum = title.substring( 2 )  ;
        
        return Integer.parseInt( snum );
    }
    public Board board() {
        return board;
    }
    public int channel() {
        return channel;
    }
    public int getNumSamples() { return nsamples; }
    
    public Channel( Board board, int channel ) {
        this.board = board;
        this.channel = channel;
        masked = false;
        nsamples = -1;              // no samples yet
        atrig = AnalogTrigger.getNullTrigger();
    }
    
    public void setPairMember( Channel channel ) {
        pair_member = channel;  
    }
    
    public boolean isMasked() {
        return masked;
    }
    public void setMasked( boolean mask ) { 
        masked = mask; 
        // DONT use the pair_member.setMask() as this is recursive!
        if ( pair_member != null ) pair_member.masked = mask;
    }
    public void setNumSamples( int nsamples ) {
        this.nsamples = nsamples;
    }
    public void clearNumSamples() {
        nsamples = -1;
        clearUploading();
    }
    public void setUploading() { 
        uploading = true; 
    }
    void clearUploading() {
        uploading = false;      // only package members can do this ???
    }
    public boolean isUploading() {
        return uploading; 
    }
    public int makeID() {
        return (board.getSlot())*100 + channel;
    }

    public String makeIDString() {
        String tnum = String.valueOf( makeID() );   
        while( tnum.length() < 3 ){
            tnum = "0" + tnum;                      // loads a channels YEAH!
        }    
        return "Ch" + board.getSlot() + "." + (channel<10?"0":"") + channel;
    }
    public String makeIDString1() {
        String tnum = String.valueOf( makeID() );   
        while( tnum.length() < 3 ){
            tnum = "0" + tnum;                      // loads a channels YEAH!
        }    
        return "." + board.getSlot() + "." + (channel<10?"0":"") + channel;
    }
    public int raw( float volts ) {

        return (int)( volts/10*32768 );                  // WORKTODO
    }
}

