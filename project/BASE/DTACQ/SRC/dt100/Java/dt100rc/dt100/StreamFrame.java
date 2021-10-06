/*****************************************************************************
 *
 * File:
 *
 * Created by IntelliJ IDEA.
 * User: pgm
 * Date: May 27, 2002
 * Time: 12:35:59 PM
 *
 * $RCSfile: StreamFrame.java,v $
 *
 * Copyright (C) 2002 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: StreamFrame.java,v 1.8.4.3 2004/06/22 20:46:17 pgm Exp $
 * $Log: StreamFrame.java,v $
 * Revision 1.8.4.3  2004/06/22 20:46:17  pgm
 * only run StreamCallbacks on Frame Accept.
 *
 * Revision 1.8.4.2  2004/06/10 20:32:14  pgm
 * MF is go!
 *
 * Revision 1.8.4.1  2004/05/01 13:54:13  pgm
 * *** empty log message ***
 *
 * Revision 1.8  2002/06/11 20:13:32  pgm
 * sub channel streaming
 *
 * Revision 1.7  2002/05/27 10:25:11  pgm
 * std headers throughout
 *
 *
 *
\*****************************************************************************/

package dt100;

import debug.*;
import dtacqserver.*;

class SubframeOutOfSyncException extends DtacqException {
    SubframeOutOfSyncException() {
        super( "SubframeOutOfSyncException" );
    }
}

public class StreamFrame
// encapsulation of the dt100 streaming frame structure
{
    long sample_number;
    boolean trigger_present;
    int dio;
    int extra;
    int nchannels;
    int isubframe;
    Board board;
    private boolean frame_accepted;    // @@todo StreamingCallback signals frame accept via this field

    static boolean turbo;
    
    public static void setTurbo( boolean on ) {
        turbo = on;   
    }
    public void reset() {
        isubframe = 0;
        sample_number = 0;
        dio = 0;
        extra = 0;
        trigger_present = false;
    }
    
    public static final int NSUBFRAMES = 64;
    public static final int MAXNX = 48;
    
    short frame_data[][];
      
    static boolean nX_bit( byte tag_h ) {
        return (tag_h&0x80) != 0;
    }
    static boolean T_bit( byte tag_h ) {
        return (tag_h&0x40) != 0;
    }
    static int subframe( byte tag_h ) {
        return tag_h&0x3f;
    }
    static int dio_field(byte tag_l ) {
        return tag_l&0x00ff;
    }
    static boolean isEvenSubframe( byte tag_h ) {
        return (subframe(tag_h)&0x1) == 0;
    }
    static boolean isOddSubframe( byte tag_h ){
        return (subframe(tag_h)&0x1) == 1;
    }
    static byte tagH( short wtag ) {
        return (byte)(wtag>>8);   
    }
    static byte tagL( short wtag ) {
        return (byte)(wtag&0x0ff);
    }

    StreamFrame( Board board, int nchannels ) {
        this.board = board;
        this.nchannels = nchannels;
        
        frame_data = new short[NSUBFRAMES][nchannels+1];
        reset();
    }   
    
    int subFrameLen() {     // return subframe length in bytes
        return 2*(nchannels+1);
    }
    public int length() {
        return subFrameLen() * NSUBFRAMES;
    }
    
    static String sTag( byte tag_h, byte tag_l ) {
        return Integer.toString(tag_h&0x00ff,16) + Integer.toString(tag_l&0x00ff,16);
    }
    public int appendSubframe( byte[] raw, int istart, int maxbytes ) {
        // returns #bytes consumed
        
        if ( maxbytes < subFrameLen() ){
            return 0;
        }
         
        byte tag_h = raw[istart+1];
        byte tag_l = raw[istart];
        
      
        //String debug_str = new String( "appendSubframe(): " + isubframe + " Tag:" + sTag( tag_h, tag_l ) );
        
        if ( subframe( tag_h ) == isubframe ){

            //Debug.println( 4, "SUCCESS!" + debug_str );

            frame_data[isubframe][0] = (short)
                (((tag_h&0x0ff)<<8) | (tag_l&0x0ff)  );   // store be format
            
            // copy across the data
            
            int iraw = istart + nchannels*2+1;
            
            if ( !turbo ){
                for ( int iword = nchannels; iword != 0; ){
                    frame_data[isubframe][iword--] = (short)
                        (((raw[iraw--]&0x0ff)<<8) | (raw[iraw--]&0x0ff));
                }
            }
                
            if ( isubframe < MAXNX ) {
                
                if ( isubframe == 5 ){
                    dio = dio_field( tag_l );  // works on incoming LE data
                }else if ( isubframe == 6 ) {
                    dio |= dio_field( tag_l )  << 8;
                }
                if ( nX_bit( tag_h ) ){
                    sample_number |= 1 << isubframe;
                }
            }else {
                if ( nX_bit( tag_h ) ){
                    extra |= 1 << (isubframe-MAXNX);
                }
            }
            if ( T_bit( tag_h ) ){
                trigger_present = true;
            }
            
            ++isubframe; // ready for next time;
            return subFrameLen();
        }else if ( isubframe == 0 ){
          //  Debug.println( 4, "WAIT..." + debug_str );

            return subFrameLen();
        }else{
        
            String debug_str = new String( "appendSubframe(): " + isubframe + " Tag:" + sTag( tag_h, tag_l ) );
            Debug.println( "StreamFrame:REJECT :" + debug_str );
            
            reset();
            return -1;
        }
    }
    
    public boolean isComplete() {
        return isubframe == NSUBFRAMES;
    }
 
    public long getSampleNumber() {
        return sample_number;
    }
    public boolean triggerPresent() {
        return trigger_present;
    }
    public boolean triggerPresent( int isample ) {
        return T_bit( tagH( getTag( isample ) ) );
    }
    
    public int getDIO()     { return dio; }
    public int getDIO( int isample ) {
        return dio_field( tagL(getTag(isample)));   
    }
    public boolean get_nX_bit( int isample ){
        return nX_bit( tagH( getTag( isample ) ) );   
    }
    public int getExtra()   { return extra; }
    public int getNchannels() { return nchannels; }
    public int getNsubframes() { return NSUBFRAMES; }
    public short getTag( int iSubframe ) {
        return frame_data[iSubframe][0];
    }
    public int getDataPoint( int iSubframe, int iChannel ) {
        return frame_data[iSubframe][iChannel];
    }
    
    public String getChannelId( int ichannel ) {
        return board.getActiveChannel(ichannel).toString();   
    }

    public boolean isFrame_accepted() {
        return frame_accepted;
    }

    public void setFrame_accepted(boolean frame_accepted) {
        this.frame_accepted = frame_accepted;
    }
}
