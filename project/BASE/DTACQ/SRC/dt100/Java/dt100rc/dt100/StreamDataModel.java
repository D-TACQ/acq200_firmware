/*****************************************************************************
 *
 * File:
 *
 * Created by IntelliJ IDEA.
 * User: pgm
 * Date: May 27, 2002
 * Time: 12:35:59 PM
 *
 * $RCSfile: StreamDataModel.java,v $
 *
 * Copyright (C) 2002 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: StreamDataModel.java,v 1.7.4.7 2004/09/03 08:42:26 pgm Exp $
 * $Log: StreamDataModel.java,v $
 * Revision 1.7.4.7  2004/09/03 08:42:26  pgm
 * B5217
 * Avoid channel duplication in Board st tree model and dt100 act on the SAME channels
 * then masking etc can work
 *
 * Revision 1.7.4.6  2004/09/03 08:40:50  pgm
 * B5217
 * Streaming single frames - plot from offset one to avoid plotting tag
 *
 * Revision 1.7.4.5  2004/06/26 19:59:09  pgm
 * serious grief failed to serialize stream Combos. fake it by introducing defaults MF Stats in but non-func
 *
 * Revision 1.7.4.4  2004/06/22 20:46:17  pgm
 * only run StreamCallbacks on Frame Accept.
 *
 * Revision 1.7.4.3  2004/06/12 16:54:06  pgm
 * burst streaming plot buggy, but generally good enough
 *
 * Revision 1.7.4.2  2004/06/11 16:54:45  pgm
 * MF display works, but can core dump also
 *
 * Revision 1.7.4.1  2004/06/11 10:03:10  pgm
 * DIO column in TAB now 8 bits not 16
 *
 * Revision 1.7  2002/05/27 10:25:11  pgm
 * std headers throughout
 *
 *
 *
\*****************************************************************************/

package dt100;

import javax.swing.table.*;
import debug.Debug;

public class StreamDataModel extends AbstractTableModel
// factory - raw/volts augment with tag
{
    static int iframe;
    
    StreamFrame [] streamFrames;
    int frameCount;
    int subframeCount;
    int maxchan;                // quick hack to speed plotting
    int this_frame;
    int first_channel;

    int frame(int row) {
        return row/subframeCount;
    }
    int row(int row){
        return row%subframeCount;
    }
    private void _init() {

    }
    protected StreamDataModel( StreamFrame streamFrame ) {
        this.streamFrames = new StreamFrame[1];
        this.frameCount = 1;
        this.subframeCount = streamFrame.getNsubframes();
        this.streamFrames[0] = streamFrame;
        this.maxchan = streamFrame.getNchannels();
        this.first_channel = 0;
        this.this_frame = ++iframe;
        Debug.println( "StreamDataModel() "+ this_frame );
    }

    private void initStreamDataModel(StreamFrame [] streamFrames, int maxframes, int maxchan, int first_channel ) {
        this.streamFrames = streamFrames;
        this.frameCount = maxframes;
        this.subframeCount = streamFrames[0].getNsubframes();
        this.maxchan = streamFrames[0].getNchannels();
        this.first_channel = first_channel;

        if (this.maxchan > maxchan){
            this.maxchan = maxchan;
        }
        this.this_frame = ++iframe;
        Debug.println( "StreamDataModel() "+ this_frame );
    }
    protected StreamDataModel( StreamFrame [] streamFrames, int maxframes, int maxchan, int first_channel ) {
        initStreamDataModel(streamFrames, maxframes, maxchan, first_channel );
    }

    
    protected StreamDataModel( StreamDataModel peer )
    {
        initStreamDataModel( peer.streamFrames, peer.frameCount, peer.maxchan, peer.first_channel );
    }
    
    public final static int WITH_INDEX = 1;
    public final static int WITH_TAG = 2;
    public final static int IS_MULTIFRAME = 4;

    public static StreamDataModel instantiate(StreamFrame[] streamFrames,
                                              int maxframes,
                                              int maxchan,
                                              int flags,
                                              boolean values_in_volts, int first_channel) {
	
        StreamDataModel model = values_in_volts? 
	            new VoltsStreamDataModel( streamFrames, maxframes, maxchan, first_channel ):
		        new StreamDataModel( streamFrames, maxframes, maxchan, first_channel );
			
		if ( (flags&WITH_TAG) != 0 ){
		    model = new TaggedStreamDataModel( model, flags );
		}else{
            model.first_channel = 1;
        }
		return model;
	}
    public int getColumnCount()
    {
        // This method is derived from interface javax.swing.table.TableModel
        // to do: code goes here
        return maxchan - first_channel;
    }

    public int getRowCount()
    {
        return frameCount*subframeCount;
    }
    public Class getColumnClass(int columnIndex) {
        return Integer.class;   
    }

    public Object getValueAt(int rowIndex, int columnIndex)
    {
        return new Integer(
	            streamFrames[frame(rowIndex)].getDataPoint( row(rowIndex), columnIndex+first_channel ) );
    }
    public String getColumnName( int c ) {
        return streamFrames[0].getChannelId( c+first_channel );
    }   
    public void finalize() throws Throwable {
        Debug.println( "StreamDataModel.finalize() "+ this_frame );
        super.finalize();
    }
}

class VoltsStreamDataModel extends StreamDataModel {
    public VoltsStreamDataModel( StreamFrame [] streamFrames, int maxframes, int maxchan, int first_channel ) {
        super( streamFrames, maxframes, maxchan, first_channel );
    }
    public Object getValueAt(int rowIndex, int columnIndex)
    // worktodo - scaling !!
    {
        float f = streamFrames[frame(rowIndex)].getDataPoint( row(rowIndex), columnIndex+first_channel );
	
	    f *= 10;
	    f /= 32768;
	    
        return new Float( f );
    }
    public Class getColumnClass(int columnIndex) {
        return Float.class;   
    }
}

class TaggedStreamDataModel extends StreamDataModel {
    static final int ISUBFRAME      = 0;
    static final int ISAMPLE_NUMBER = 1;
    static final int INX            = 2;
    static final int ITRIGGER       = 3;
    static final int IDIO           = 4;
    static final int IEXTRA         = 5;
    static final int IRAWTAG        = 6;
    static final int IFIRSTSAMPLE   = 7;

    StreamDataModel peer;
    int flags;
    
    TaggedStreamDataModel( StreamDataModel peer, int flags ){
        super( peer );
        this.peer = peer;
        this.flags = flags;
    }
    int ifirstsample() {
        return (flags&WITH_TAG)!=0? IFIRSTSAMPLE: 1;   
    }
    int cpeer( int c ){
        return c - ifirstsample();   
    }
    public int getColumnCount()
    {
        // This method is derived from interface javax.swing.table.TableModel
        // to do: code goes here
        return super.getColumnCount() + ifirstsample();
    }
    public Class getColumnClass( int col ) {
        int cp = cpeer( col );

        if ( cp >= 0 ){
            return peer.getColumnClass( cp );
        }else{
            return Object.class;
        }
    }
        
    public Object getValueAt(int rowIndex, int columnIndex)
    {
        // This method is derived from interface javax.swing.table.TableModel
        // to do: code goes here
        int cp = cpeer( columnIndex );
        int fr = frame(rowIndex);
        int rw = row(rowIndex);

        if ( cp >= 0 ){
            return peer.getValueAt( rowIndex, cp );
        }
        switch( columnIndex ){
        case ISUBFRAME:
            return new String( fr + "." + rw );
        case ISAMPLE_NUMBER:
            return String.valueOf( streamFrames[fr].getSampleNumber()+rw );
        case INX:
            return streamFrames[fr].get_nX_bit( rw )? "1": "0";
        case ITRIGGER:
            return streamFrames[fr].triggerPresent( rw )? "T": "x";
        case IDIO:
            return HexString.leftFill( streamFrames[fr].getDIO( rw )&0x00ff, 2 );
        case IEXTRA:
            return HexString.leftFill( streamFrames[fr].getExtra()&0x0ffff, 4 );
        case IRAWTAG:
            return HexString.leftFill( streamFrames[fr].getTag(rw)&0x0ffff, 4 );
        default:
            return new Integer( 6666 );
        }
    }

        
    public String getColumnName( int c ) {
        int cp = cpeer( c );
        
        if ( cp >= 0 ){
            return peer.getColumnName( cp );
        }
        
        switch( c ){
        case ISUBFRAME:
            return "SFR";
        case ISAMPLE_NUMBER:
            return "SCC";
        case INX:
            return "nX";
        case ITRIGGER:
            return "TR?";
        case IDIO:
            return "DIO";
        case IEXTRA:
            return "EXTRA";
        case IRAWTAG:
            return "TAG";
        default:
            return "????";
        }
    }   
    
}

