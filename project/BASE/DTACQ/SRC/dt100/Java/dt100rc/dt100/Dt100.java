/*****************************************************************************
 *
 * File:
 *
 * $RCSfile: Dt100.java,v $
 *
 * Copyright (C) 1999 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: Dt100.java,v 1.48.4.20 2004/09/13 16:46:39 pgm Exp $
 * $Log: Dt100.java,v $
 * Revision 1.48.4.20  2004/09/13 16:46:39  pgm
 * fixes striding
 *
 * Revision 1.48.4.19  2004/09/04 23:08:12  pgm
 * B5219
 * really really fix tree view
 *
 * Revision 1.48.4.18  2004/09/03 08:42:26  pgm
 * B5217
 * Avoid channel duplication in Board st tree model and dt100 act on the SAME channels
 * then masking etc can work
 *
 * Revision 1.48.4.17  2004/08/14 20:44:42  pgm
 * B5216
 * fix crazy "Board as Singleton" bug
 *
 * Revision 1.48.4.16  2004/06/26 22:57:23  pgm
 * Build 5210 MF stats look good.
 * Teed up for previous second control output.
 * (see DtacqClient.connectRaw()).
 *
 * Revision 1.48.4.15  2004/06/26 20:19:54  pgm
 * MF Stats file report works.
 *
 * Revision 1.48.4.14  2004/06/26 19:59:09  pgm
 * serious grief failed to serialize stream Combos. fake it by introducing defaults MF Stats in but non-func
 *
 * Revision 1.48.4.13  2004/06/22 20:46:17  pgm
 * only run StreamCallbacks on Frame Accept.
 *
 * Revision 1.48.4.12  2004/06/12 16:54:06  pgm
 * burst streaming plot buggy, but generally good enough
 *
 * Revision 1.48.4.11  2004/06/11 16:55:42  pgm
 * MF display works, but can core dump also
 *
 * Revision 1.48.4.10  2004/06/10 20:32:14  pgm
 * MF is go!
 *
 * Revision 1.48.4.9  2004/06/02 20:10:12  pgm
 * saves one directory per second files - looks good to me.
 * next stop, multi frame pulses
 *
 * Revision 1.48.4.8  2004/06/01 13:06:51  pgm
 * view and saveDataRange start and stride work.
 *
 * Revision 1.48.4.7  2004/05/31 09:12:47  pgm
 * *** empty log message ***
 *
 * Revision 1.48.4.6  2004/05/30 22:16:04  pgm
 * saves large memory lengths
 *
 * Revision 1.48.4.5  2004/05/08 20:38:39  pgm
 * *** empty log message ***
 *
 * Revision 1.48.4.4  2004/05/05 19:30:29  pgm
 * throttle threads gives better perf
 *
 * Revision 1.48.4.3  2004/04/18 21:14:03  pgm
 * works with acq196
 *
 * Revision 1.48.4.2  2003/12/20 15:12:29  pgm
 * hack correct channel mask on ACQ200
 *
 * Revision 1.48.4.1  2003/12/19 21:32:27  pgm
 * acq200, slot addressing
 *
 * Revision 1.48  2002/06/12 20:25:28  pgm
 * no control without commit, ChannelMask is per board
 *
 * Revision 1.47  2002/06/11 20:13:32  pgm
 * sub channel streaming
 *
 * Revision 1.46  2002/05/27 14:46:25  pgm
 * analog trigger runs
 *
 * Revision 1.45  2002/05/27 07:50:33  pgm
 * get boards more robust
 *
 * Revision 1.44  2002/05/25 12:24:21  pgm
 * StreamingAdapter pattern is good
 *
 * Revision 1.43  2002/05/24 11:21:50  pgm
 * *** empty log message ***
 *
 * Revision 1.42  2002/05/20 07:31:36  pgm
 * column class care
 *
 * Revision 1.41  2002/05/17 09:34:30  pgm
 * SOFT_CONTINUOUS gets prelen
 *
 * Revision 1.40  2002/05/08 09:53:59  pgm
 * table navigation, long tables are go
 *
 * Revision 1.39  2002/05/07 20:20:10  pgm
 * table with index decoration
 *
 * Revision 1.38  2002/04/21 12:39:51  pgm
 * connection quites thread on completion
 *
 * Revision 1.37  2002/04/20 11:09:04  pgm
 * fix sync problem in model add
 *
 * Revision 1.34  2002/04/18 12:53:43  pgm
 * table format is good
 *
 * Revision 1.33  2002/04/17 21:18:41  pgm
 * meg class Transaction refactoring lets hope it works
 *
 * Revision 1.32  2002/04/17 15:05:50  pgm
 * proper debugs in TableModel
 *
 * Revision 1.31  2002/04/16 12:38:44  pgm
 * resources rool OK
 *
 * Revision 1.30  2002/04/15 13:20:43  pgm
 * excellent status behaviour - ship it quick
 *
 * Revision 1.28  2002/04/14 15:26:33  pgm
 * works with mixed board sets
 *
 * Revision 1.27  2002/04/14 12:38:59  pgm
 * works with acq32, acq16, new format
 *
 * Revision 1.26  2002/03/25 22:26:53  pgm
 * new style /proc/pci
 *
 * Revision 1.25  2001/06/25 21:35:22  pgm
 * avoid mask overrun with acq16
 *
 * Revision 1.24  2001/06/22 15:11:25  pgm
 * works with multi boards ship it quick
 *
 * Revision 1.23  2001/06/21 21:50:26  pgm
 * should fix multi board issues
 *
 * Revision 1.22  2001/06/17 21:49:47  pgm
 * save to file - looking good
 *
 * Revision 1.21  2001/06/17 13:36:32  pgm
 * remote shutdown works
 *
 * Revision 1.20  2001/06/16 21:11:20  pgm
 * better tree displays
 *
 * Revision 1.19  2001/06/16 18:27:39  pgm
 * hacked in acq16 (12) settings
 *
 * Revision 1.16  2000/10/28 20:02:58  pgm
 * improved reliability, l&f, messaging
 *
 * Revision 1.15  2000/10/25 22:43:09  pgm
 * handles pre- post, Triggered
 *
 * Revision 1.14  2000/09/30 13:01:15  pgm
 * pre, post samples opts available
 *
 * Revision 1.13  2000/09/22 20:01:11  pgm
 * stop streaming works, errors or no
 *
 * Revision 1.12  2000/09/22 19:32:50  pgm
 * streaming stop works\!
 *
 * Revision 1.11  2000/09/20 22:34:07  pgm
 * streaming save to files done
 *
 * Revision 1.10  2000/09/20 21:44:44  pgm
 * first cut stream to raw
 *
 * Revision 1.9  2000/09/07 00:15:23  pgm
 * supports DVM, shows correct channel mapping
 *
 * Revision 1.8  2000/08/21 19:33:20  pgm
 * streaming works no that GUI update rate limits
 *
 * Revision 1.7  2000/07/29 16:17:50  pgm
 * View, Plot data in streaming mode
 *
 * Revision 1.6  2000/06/30 21:25:14  pgm
 * Soft Continuous added
 *
 * Revision 1.5  2000/06/11 21:59:56  pgm
 * fix nasty init problem from persistence
 *
 * Revision 1.4  2000/06/04 14:00:45  pgm
 * fix bug in persistent init
 *
 * Revision 1.3  2000/06/03 19:36:29  pgm
 * persistence works in most departments
 *
 * Revision 1.2  2000/06/03 08:44:17  pgm
 * SC ops (load,store,commit,edit) work
 *
 * Revision 1.1.1.1  2000/06/03 05:36:25  pgm
 * fromQ
 *
 * Revision 1.1.1.1  2000/05/28 16:06:35  pgm
 * uses packages
 *
 * Revision 1.18  2000/05/28 10:32:18  pgm
 * streaming works passibly
 *
 * Revision 1.17  2000/05/21 18:31:13  pgm
 * prototype signal conditioning panel (non-functioning)
 *
 * Revision 1.16  2000/05/21 14:21:53  pgm
 * stream panel added. also abortive experiment with packages
 *
 * Revision 1.15  2000/05/20 21:03:05  pgm
 * GridBag is good. Status resize, Clock gui etc
 *
 * Revision 1.14  2000/05/16 21:36:18  pgm
 * Status Reports 'r us
 *
 * Revision 1.13  2000/05/15 19:55:42  pgm
 * updated from harris
 *
 * Revision 1.12  2000/05/04 18:50:06  pgm
 * Debug class added
 *
 * Revision 1.10  2000/04/26 04:34:46  pgm
 * new style master slave config
 *
 * Revision 1.9  2000/04/24 20:15:57  pgm
 * getBoards, also better resize using GridBagLayout
 *
 * Revision 1.8  2000/04/19 19:28:30  pgm
 * save to root works
 *
 * Revision 1.7  2000/04/18 18:01:12  pgm
 * updates during capture
 *
 * Revision 1.6  2000/04/18 16:48:27  pgm
 * multithreading, first stab
 *
 * Revision 1.5  2000/04/18 09:45:41  pgm
 * repaired channel comms, primitive status rep
 *
 * Revision 1.4  2000/04/17 17:30:41  pgm
 * table and chart plot OK
 *
 * Revision 1.3  2000/04/16 10:59:23  pgm
 * data upload works!
 *
 * Revision 1.2  2000/04/13 19:50:37  pgm
 * added headers
 *
 * Revision 1.1  1999/10/22 16:26:49  pgm
 * first entry to cvs
 *
 *
 * References
 * [1] - Core Java II, Horstmann and Cornell
 \*****************************************************************************/

package dt100;


import java.awt.EventQueue;
import java.io.*;
import java.util.*;
import javax.swing.table.*;
import java.text.NumberFormat;

import debug.*;
import dtacqserver.*;

class DtacqBadResponseException extends DtacqException {
    public DtacqBadResponseException( String msg ) {
        super( msg );
    }
}

class CloseTransaction extends dt100.Transaction implements dt100.Command {

    public CloseTransaction() {
        super( "" );
    }
    public void encode( PrintWriter output ){
        Debug.println( "CloseTransaction() - WOOPSEE" );
    }
}

class ShutdownTransaction extends SystemTransaction {
    public void encode( PrintWriter output ) {
        String cmd = new String( COMMAND_PREFIX + " exec shutdown" );
        output.println( cmd );
    }
}





class GetMultilineTransaction extends GetTransaction {
    // this one's a little tricky as we have to accomodate multiple lines
    // so wrap decode() to stash input, then hoover up the rest in onDecode()

    DataInputStream input;

    protected GetMultilineTransaction( ResultCallback callback, String query ) {
        super( callback, query );
    }
    public void decode( DataInputStream input )
            throws DtacqException, IOException {
        super.decode( this.input = input );
    }
    protected void onDecode( String residue, DataInputStream input )
            throws DtacqException, IOException {

        try {
            Thread.sleep( 250 );
        } catch( InterruptedException e ) {

        }
        while( input.available() != 0 ){
            residue += "\n" + input.readLine();
        }
        ((ResultCallback)callback).setResult( residue );
    }
    public static GetTransaction fwRev( ResultCallback callback ) {
        return new GetMultilineTransaction( callback, Acq32BusProt.getFwRev );
    }
}


abstract class SystemTransaction extends Transaction {
    protected final String COMMAND_PREFIX = "dt100";

    SystemTransaction() {
        super( "DT100:" );
    }
}

class OpenTransaction extends SystemTransaction {
    String mode;
    String device;

    OpenTransaction( String mode, String device ) {
        this.mode = mode;
        this.device = device;
    }
    public void encode( PrintWriter output ) {
        String cmd = new String( COMMAND_PREFIX + " open " + mode + " " + device );
        output.println( cmd );
    }
}


class DataSink {
    public void processData( int offset, byte buffer[], int nbytes ) {}
    void close() {}
    public void declareNewDataSet( int iset ) {}
}

class Dt100FrameDataSink extends DataSink {

    NumberFormat nf = NumberFormat.getInstance();

    FileOutputStream fos;
    int idata_set;
    dtacqutils.TimedPathGenerator pathGenerator;
    String fn;
    String path;

    void generateFname() {
        fn = nf.format( idata_set ) + ".raw";
        path = pathGenerator.getPathName();
    }

    String fname() {
        return path + fn;
    }
    public Dt100FrameDataSink( String save_root ) {

        nf.setMinimumIntegerDigits( 6 );
        nf.setGroupingUsed( false );

        try {
            String first_try = save_root + "test";

            fos = new FileOutputStream( first_try );

            Debug.println( "Dt100FrameDataSink: save to " + first_try );

        }catch ( IOException e ) {
            System.out.println( "Dt100FrameDataSink: failed to create file in " + save_root );
            try {

                fos = new FileOutputStream( "test" );

                System.out.println( "Dt100FrameDataSink: output to " + "test" );
                save_root = "";
            }catch ( IOException e2 ){
                System.out.println( "Dt100FrameDataSink: could not create " + "test" );
            }
        }
        pathGenerator = new dtacqutils.TimedPathGenerator(save_root);
    }
    public void declareNewDataSet( int iset )
    {
        int old_set = idata_set;
        idata_set = iset;

        if ( fos != null ){
            try {
                fos.close();
            } catch ( IOException e ){
                System.out.println( "Dt100FrameDataSink: failed to close fos" );
            }

        }
        String old_path = path;

        try {
            if (pathGenerator.onNewSecond()){
                generateFname();
                ResultCallback cb = Dt100.instance().getStreamingDatasetEndCallback();
                if (cb != null){
                    cb.setResult(old_set + " " + old_path);
                    EventQueue.invokeLater(cb);
                }   
                ResultCallback cb2 = Dt100.instance().getStreamingDatasetStartCallback();
                if (cb2 != null){
                    cb2.setResult(iset + " " + fname());
                    EventQueue.invokeLater(cb2);
                }                
            }else{
                generateFname();
            }
            fos = new FileOutputStream( fname() );

            Debug.println( 1, "Dt100FrameDataSink: output to " + fname() );

        }catch ( IOException e2 ){
            System.out.println( "Dt100FrameDataSink: could not create " + fname() );
        }
    }

    public void processData( int offset, byte buffer[], int nbytes ) {
        String top_of_buffer = "FrameDataSink " + offset;

        debug.Debug.println( 3, top_of_buffer );

        try {
            fos.write( buffer, offset, nbytes );
        } catch ( IOException e ){
            System.out.println( "Dt100FrameDataSink: write failed" );
        }
    }
}
class DataFetchTransaction extends SystemTransaction implements Command {

    DataSink sink;
    int start, stop, stride;
    boolean is_final;

    public DataFetchTransaction( DataSink sink, int start, int stop, int stride ) {
        this.sink = sink;
        this.start = start;
        this.stop = stop;
        this.stride = stride;
        is_final = true;
    }

    public void encode( PrintWriter output ) {
        String command = COMMAND_PREFIX + " " +
                "read " +
                start + " " +
                stop + " " +
                stride;

        Logger.println(command);
        output.println(command);
    }
    protected void finalize() {
        Debug.println( 5, "DataFetchTransaction.finalize()" );
    }

    final int MAXBUF = 0x100000;

    protected void onDecode( String residue, DataInputStream input )
            throws DtacqException, IOException {
        CommandLine rl = new CommandLine( residue );

        Iterator it = rl.iterator();

        int nsamples;
        boolean isAscii;

        try {
            if ( it.hasNext() ) {
                nsamples = Integer.valueOf( it.next().toString() ).intValue();
            }else{
                throw new DtacqBadResponseException( "#samples expected" );
            }
            if ( it.hasNext() ){
                isAscii = it.next().equals( "lines" );
            }else{
                throw new DtacqBadResponseException( "keyword expected" );
            }

            // now input the data
            int nbytes = nsamples;
            byte buf[];

            for ( int ibyte = 0; ibyte != nbytes; ){

                buf = new byte[nbytes>MAXBUF? MAXBUF: nbytes];

                int bytes_left = nbytes - ibyte;
                int max_read = bytes_left>0x10000? 0x10000: bytes_left;
                int nread = input.read( buf, 0, max_read );

                sink.processData( ibyte, buf, nread );

                if ( ibyte == 0 && Debug.getDebug_level() > 4 ){
                    for ( int jbyte = 0; jbyte != 16; ++jbyte ){
                        Debug.print( 5, String.valueOf( (int)buf[jbyte] ) );
                    }
                    Debug.println( 5, "" );
                }

                ibyte += nread;

                if ( callback != null ){
                    ((AbstractChannelCallback)callback).setNumSamples( ibyte/2 );
                    EventQueue.invokeLater( callback );
                }
            }
        } finally {
            if (is_final) {
                if (sink != null) {
                    sink.close();
                }
                if (callback != null) {
                    ((AbstractChannelCallback) callback).clearUploading();
                }
            }
        }
    }

    public void setFinal(boolean b) {
        is_final = b;
    }
}

class Connection extends Thread implements Serializable
// must have at least one transaction tee'd up or it will just terminate
{
    ReadWriteStream rws;
    final String host;
    final String device;
    Vector transactions = new Vector();
    private boolean connection_finished = false;
    protected boolean request_max_rxbuffer;

    Connection( String host, String device ) throws IOException {
        this.host = host;
        this.device = device;
        openChannel( device );
        setPriority( MIN_PRIORITY+1 );
    }
    protected void openChannel( String device )
            throws DtacqException, IOException
            // sometimes you might not need to talk to a device
    {}

    protected void closeChannel() {
        rws.getOutput().println( "bye" );
        rws.getOutput().flush();

        try {
            Debug.println( 2, "Remote closedown:"+device+
                    " ?" + rws.getInput().readLine() );    //
        } catch ( IOException e ){
            Debug.println( 2, "far side has closed, shut down this side" );
        } finally {
            onClose();
            DtacqClient.close( rws );
        }
    }
    protected void onClose() {
        connection_finished = true;
    }

    private void exec( Transaction transaction )
            throws DtacqException, IOException, InterruptedException  {

        // hack your way out anything using RTTI.
        // better to use a regular transaction with callback to close down

        if ( transaction instanceof CloseTransaction ){
            closeChannel();
        }else{
            try {
                transaction.encode( rws.getOutput() );
                transaction.decode( rws.getInput() );
            } catch ( DtacqException e ) {
                closeChannel();
                Debug.println( "Connection.exec() " + e + " closing down" );
            }
        }
    }

    public void run() {
        try {
            DtacqClient.setRequest_max_rxbuffer(request_max_rxbuffer);
            rws = DtacqClient.connect( host, device );
        } catch ( DtacqException de ) {
            System.err.println( "Connection: ERROR: " + de );
            Debug.println( "Connection.run() dropping out" );
            return;
        } catch ( IOException ioe ) {
            System.err.println( "Connection: ERROR: " + ioe );
            Debug.println( "Connection.run() dropping out" );
            return;
        }

        try {
            while( !connection_finished && !interrupted() ){
                exec( fetch() );
            }
        } catch ( Exception e ) {
            System.err.println( "Connection.run() hit exception " + e );
            e.printStackTrace();
        }
        Debug.println( "Connection.run() "+host+" "+device+" complete" );
    }

    synchronized Transaction fetch() throws InterruptedException {
        Transaction latest;

        while( transactions.isEmpty() ){
            wait();
        }
        synchronized(transactions){
            latest = (Transaction)transactions.get(0);
            transactions.remove( 0 );
        }

        return latest;
    }

    synchronized public void post( Transaction transaction ) {
        if ( !isAlive() ){
            start();
        }
        synchronized(transactions){
            transactions.add( transaction );
        }
        notify();
    }
}

class MasterConnection extends Connection {
    static String makeDevice( String master, String slaves ) {
        return new String( "/dev/acq32/acq32." + master + ".m" + slaves );
    }

    protected void openChannel( String device )
            throws DtacqException, IOException {
        post( new OpenTransaction( "master", device ) );
    }

    public MasterConnection( String host, String master, String slaves )
            throws DtacqException, IOException {

        super( host, makeDevice( master, slaves ) );
    }
}

class ShellConnection extends Connection {
    static String makeDevice() {
        return new String( "1" );
    }

    protected void openChannel( String device )
            throws DtacqException, IOException {
        post( new OpenTransaction( "shell", device ) );
    }

    public ShellConnection( String host )
            throws DtacqException, IOException {

        super( host, makeDevice() );
    }
}

class DataConnection extends Connection {
    static class ActiveCount {
        static final int MAXNACTIVE = 4;
        int nactive;
        private boolean _p() {
            if (nactive < MAXNACTIVE){
                nactive++;
                return true;
            }else{
                return false;
            }
        }
        private void _v() {
            if (nactive > 0){
                nactive--;
            }
        }
        void v() {
            synchronized(this){
                _v();
            }
        }
        boolean p() {
            boolean pass;
            synchronized(this){
                pass = _p();
            }
            return pass;
        }
    }
    static ActiveCount nactive = new ActiveCount();

    static String makeDevice( String board, String channel ) {
        return new String(
                "/dev/acq32/acq32." +
                board + "." +
                (channel.length() < 2? "0": "") +
                channel                         );
    }


    protected void openChannel( String device )
            throws DtacqException, IOException  {
        post( new OpenTransaction( Dt100.data_fetch_connection_mode, device ) );
    }
    public DataConnection( String host, String board, String channel )
            throws DtacqException, IOException {
        super( host, makeDevice( board, channel ) );
        request_max_rxbuffer = true;
    }

    protected void enterGate(boolean enter) {
        if (enter){
            while(!nactive.p()){
                try {
                    sleep(100);
                }catch(Exception e){

                }
            }
        }else{
            nactive.v();
        }
    }

    public void run() {
        enterGate(true);
        super.run();
        enterGate(false);
    }
}

class SaveableState implements Serializable {

    public static final int MAXBOARDS = 16; // will be OK for CompactPCI ++
//    public static final int MAXBOARDS = 1; // @@todo pgm debug channel model
    public String hostname;
    public int nboards;
    public int acq32_count;
    public int acq16_count;
    public int acq200_count;

    String boardDetails = "not available";

    String rootDirectory = ".";
    boolean saveToFile = false;
    private Board master;
    private Board boards[] = new Board[MAXBOARDS+1]; // INDEX FROM 1

    public SaveableState() {
        int iboard;

        for ( iboard = 1; iboard <= MAXBOARDS; ++iboard ){
            boards[iboard] = new Board( iboard, null, 96 );
        }
        setMaster( null );
    }
    public Board getMaster() {
        if ( master == null ){
            Debug.println( "SaveableState master is null" );
        }
        return master;
    }
    public Board setMaster( Board master ) {
        if ( master == null ){
            Debug.println( "SaveableState master set null" );
        }
        return this.master = master;
    }

    public boolean isAcq32System() { return acq32_count > 0; }
    public boolean isAcq16System() { return acq16_count > 0; }

    public int getNumBoards() { return nboards; }

    Board getBoard( int ib ) {
        if ( ib < 1 ) { System.out.println( "State.getBoard() index from 1\n" ); return null; }
        if ( ib > nboards ) { System.out.println( "State.getBoard() index oob\n" ); return null; }
        return boards[ib];
    }

    public Collection getItems( boolean active, boolean inactive ){
        Collection c = new ArrayList();

        int iboard = 1;

        try {
            for ( ; iboard <= nboards; ++iboard ) {

                if ( active && boards[iboard].isActive() ||
                     inactive && !boards[iboard].isActive()  ){
                    c.add( boards[iboard] );
                }
            }
        }catch(ArrayIndexOutOfBoundsException e){
            System.err.println("Out of bounds " + e + "\nnboards " + nboards + "len "+ boards.length);
            throw e;
        }
        return c;
    }
};


public class Dt100 implements Serializable
{
    public boolean is_connected;
    boolean is_running = false;


    SaveableState state;

    // implementation of Serializable

    private void writeObject(java.io.ObjectOutputStream out)
            throws IOException {
        clearChannelData();
        //out.defaultWriteObject(); // Black Magic TIJ ed1 p 503
        out.writeObject( state );
    }
    private void readObject(java.io.ObjectInputStream in)
            throws IOException, ClassNotFoundException {
        //in.defaultReadObject();
        state = (SaveableState)in.readObject();
        clearChannelData();
        initStatelessFields();

        theInstance = this;     // force "Singleton" behaviour
    }


    Connection masterConnection;

    // constructor

    private static Dt100 theInstance;

    static String data_fetch_connection_mode = "data";

    public void setDataFetchConnectionMode(int mode){
        if (mode==1){
            data_fetch_connection_mode = "data1";
        }else{
            data_fetch_connection_mode = "data";
        }
    }

    private Dt100() {
        state = new SaveableState();
        initStatelessFields();
    }

    public static Dt100 instance() {
        if ( theInstance == null ){
            return theInstance = new Dt100();
        }else{
            return theInstance;
        }
    }

    public void setHostname( String hostname ) { this.state.hostname = hostname; }
    public String getHostname() { return state.hostname; }

    class LocalCallback implements Runnable {
        Runnable client_cb;

        LocalCallback( Runnable client ) { client_cb = client; }

        public void run() {         // subclasses must call this if overridden
            client_cb.run();
        }
    }
    public void clearChannelData()
            // tidy up before out serialization
    {
        for ( int iboard = 1; iboard <= state.nboards; ++iboard ){
            state.getBoard( iboard ).clearChannels();
        }
    }

    public void identifyBoards( Runnable callback ) {
        Transaction transaction = new GetBoardsTransaction();

        transaction.setCallback( callback );

        try {
            Connection connection = new Connection( state.hostname, "dt100" );
            connection.post( transaction );
            connection.post( new CloseTransaction() );
        } catch ( DtacqException de ) {
            Debug.println( 2, "execMasterTransaction() " + de );
        } catch ( IOException ie ) {
            Debug.println( 2, "execMasterTransaction() " + ie );
        }
    }
    public void getBoardFirmwareRevision( ResultCallback cb, Board board ) {
        try {
            Connection connection =
                    new MasterConnection(
                            state.hostname,
                            Integer.toString( board.getSlot() ),
                            Integer.toString( board.getSlot() )
                    );
// piggy back some additional queries first
// the costly part is time to open connection
            connection.post( GetTransaction.voltsRange(
                    Board.newVoltsRangeCallback( board )
            ) );
            connection.post( GetTransaction.availableChannels(
                    Board.newAvailableChannelsCallback( board )
            ) );
            connection.post( GetMultilineTransaction.fwRev( cb ) );
            connection.post( new CloseTransaction() );
        } catch ( DtacqException de ) {
            cb.setResult( "ERROR: Board " + board + " not responding" );
            cb.run();
            Debug.println( 2, "execMasterTransaction() " + de );
        } catch ( IOException ie ) {
            cb.setResult( "ERROR: Board " + board + " not available" );
            cb.run();
            Debug.println( 2, "execMasterTransaction() " + ie );
        }
    }
    public void setSampleStart( ResultCallback cb, int sample_start) {
        try {
            Connection connection =  new ShellConnection(state.hostname);

            connection.post( GetMultilineTransaction.fwRev( cb ) );
            connection.post( new CloseTransaction() );
        } catch ( DtacqException de ) {
            cb.setResult( "ERROR: Board " + " not responding" );
            cb.run();
            Debug.println( 2, "execMasterTransaction() " + de );
        } catch ( IOException ie ) {
            cb.setResult( "ERROR: Board " + " not available" );
            cb.run();
            Debug.println( 2, "execMasterTransaction() " + ie );
        }
    }

    public String getBoardSummary() {
        String summary = "The system comprises:\n";
        String joiner = "";

        if ( state.acq32_count != 0 ){
            summary += String.valueOf( state.acq32_count ) + " ACQ32 boards\n";
            joiner = " and\n";
        }
        if ( state.acq16_count != 0 ){
            summary += joiner;
            summary += String.valueOf( state.acq16_count ) + " ACQ16 boards\n";
        }
        if ( state.acq200_count != 0 ){
            summary += joiner;
            summary += String.valueOf( state.acq200_count ) + " ACQ200 boards\n";
        }
        if ( state.acq32_count == 0 && state.acq16_count == 0 && state.acq200_count == 0 ){
            summary += "No boards recognised\n";
        }
        return summary;
    }


    public String getBoardDetails() {
        return state.boardDetails;
    }
    public void setNboards( int nboards ) { this.state.nboards = nboards; }
    public int getNboards() { return state.nboards; }

    public boolean isAcq32System() {
        return state.isAcq32System();
    }
    public boolean isAcq16System() {
        return state.isAcq16System();
    }
    public boolean isAcq200System() {
        return state.acq200_count >= 1 && state.acq32_count == 0 && state.acq16_count == 0;
    }

    public void setMaster( int iboard ) {
        Board master = state.getBoard( iboard );
        master.setActive( true );

        for ( int ii = 1; ii <= state.getNumBoards(); ++ii ){
            Board current = state.getBoard( ii );
            if ( current.isActive() ){
                current.setMaster( master );
            }
        }
        state.setMaster( master );
    }

    public Board getMaster() {
        return state.getMaster();
    }

    public void setSlave( int iboard ) {
        Board aboard = state.getBoard( iboard );

        if ( state.getMaster() == aboard ){
            aboard.setMaster( null );
            state.setMaster( null );
        }
        aboard.setActive( true );
    }
    public void setIsolated( int iboard ) {
        Board aboard = state.getBoard( iboard );

        if ( state.getMaster() == aboard ){
            state.setMaster( null );
        }
        aboard.setActive( false );
        aboard.setMaster( null );
    }

    Collection getItems( boolean active, boolean inactive ) {
        return state.getItems( active, inactive );
    }
    public Collection getActiveBoards() {
        return getItems( true, false );
    }
    public Collection getInactiveBoards() {
        return getItems( false, true );
    }
    public Collection getBoards() {
        return getItems( true, true );
    }

    public int nActiveChannels() {
        Iterator it = getActiveBoards().iterator();
        int nactive = 0;

        while ( it.hasNext() ){
            Board board = (Board)it.next();

            nactive += board.nActiveChannels();
        }
        return nactive;
    }
    public String getSlavesString() {
        String slave_string = "";
        Iterator it = getActiveBoards().iterator();

        while ( it.hasNext() ){
            slave_string += ((Board)(it.next())).getSlot();
        }
        return slave_string;
    }

    public void connect( String hostname ) throws IOException {
        DtacqClient.connect( hostname, "dummy" );
        is_connected = true;
    }


    public boolean isConnected() { return is_connected; }

    Connection makeMasterConnection()
            throws DtacqException, IOException {
        if ( masterConnection == null ){
            masterConnection =
                    new MasterConnection(
                            state.hostname,
                            Integer.toString( state.getMaster()!=null?state.getMaster().getSlot():1 ), //&&&WORKTODO
                            getSlavesString()
                    );
        }
        return masterConnection;
    }
    public void clearMasterConnection() {
        if ( masterConnection != null ){
            masterConnection.closeChannel();
            masterConnection = null;
        }
    }

    public void execMasterTransaction( Transaction transaction ) {
        try {
            makeMasterConnection().post( transaction );
        } catch ( DtacqException de ) {
            Debug.println( 2, "execMasterTransaction() " + de );
        } catch ( IOException ie ) {
            Debug.println( 2, "execMasterTransaction() " + ie );
        }
    }
    public void execTransaction( Board board, Transaction transaction ){
        try {
            (new MasterConnection(
                    state.hostname,
                    Integer.toString(board.getSlot()),
                    Integer.toString(board.getSlot()) )).post( transaction );
        } catch ( DtacqException de ) {
            Debug.println( 2, "execMasterTransaction() " + de );
        } catch ( IOException ie ) {
            Debug.println( 2, "execMasterTransaction() " + ie );
        }
    }
    public void getState( ResultCallback callback ) {
        execMasterTransaction( GetTransaction.state( callback ) );
    }
    public void getNumSamples( ResultCallback callback ) {
        execMasterTransaction( GetTransaction.numSamples( callback ) );
    }
    public void setModeSoftTransient( int nsamples ) {
        execMasterTransaction(
                SetTransaction.mode( "SOFT_TRANSIENT", nsamples ) );
    }
    public void setModeGatedTransient( int nsamples ) {
        execMasterTransaction(
                SetTransaction.mode( "GATED_TRANSIENT", nsamples ) );
    }
    public void setModeGatedContinuous( int presamples, int postsamples ) {
        if ( presamples == -1 ){
            execMasterTransaction(
                    SetTransaction.mode( "GATED_CONTINUOUS", presamples ) );
        }else{
            execMasterTransaction(
                    SetTransaction.modeGatedContinuous( presamples, postsamples ) );
        }
    }
    public void setModeTriggeredContinuous( int presamples, int postsamples ) {
        execMasterTransaction(
                SetTransaction.modeTriggeredContinuous( presamples, postsamples ) );
    }

    public void setModeSoftContinuous( int prelen ) {
        execMasterTransaction(
                SetTransaction.mode( "SOFT_CONTINUOUS", prelen ) );
    }
    public void setArm( Runnable callback ) {
        Transaction armer = new SetTransaction( Acq32BusProt.setArm ) {
            protected void onDecode(
                    String residue, DataInputStream input
                    ) throws DtacqException, IOException {
                is_running = true;
            }
        };
        armer.setCallback( callback );
        execMasterTransaction( armer );
    }
    public void setAbort( Runnable callback ) {
        Transaction aborter = SetTransaction.abort();
        aborter.setCallback( callback );
        execMasterTransaction( aborter );
        is_running = false;
    }
    public void remoteShutdown( Runnable callback ) {
        Transaction shutdown_transaction = new ShutdownTransaction();

        shutdown_transaction.setCallback( callback );
        execMasterTransaction( shutdown_transaction );
    }
    public void reset() { is_running = false; }
    public boolean isRunning() { return is_running; }
    public void disableSaveToFile( ) {
        state.saveToFile = false;
    }
    public void enableSaveToFile( String root ) {
        state.rootDirectory = root;
        state.saveToFile = true;
    }
    public void setChannelMask() {
        for ( Iterator slaves = getActiveBoards().iterator(); slaves.hasNext(); ){

            Board board = (Board)slaves.next();
            String the_mask = "";

           Debug.println("st100.setChannelMask() " + board + " hash " + board.hashCode());

            for ( int ichannel = 0; ichannel != board.getChannelCount(); ++ichannel ) {
                the_mask += board.getChannel( ichannel ).isMasked()? "0": "1";
            }

            execTransaction( board, SetTransaction.channelMask( the_mask ) );
        }
    }
    public void setInternalClock( int hertz ) {
        // hertz==0 => use external clock
        execMasterTransaction( SetTransaction.internalClock( hertz ) );
    }
    public void setSampleTagging( boolean enable ) {
        execMasterTransaction( SetTransaction.sampleTagging( enable ) );
    }
    public void setDIO( String dio ) {
        execMasterTransaction( SetTransaction.dio( dio ) );
    }
    public void getDIO( ResultCallback callback ) {
        execMasterTransaction( GetTransaction.dio( callback ) );
    }
    public void setChannel(  int ichannel, int g1, int g2, int v_ex ) {
        execMasterTransaction( SetTransaction.channel( ichannel, g1, g2, v_ex ) );
    }

    Connection the_streaming_connection;
    DataStreamingTransaction dst;

    private ResultCallback streamingDatasetStartCallback;
    private ResultCallback streamingDatasetEndCallback;


    public void startStreaming( Board board, int stride, int nchannels, boolean mf, String save_root ) {

        cancelStreaming();

        StreamingCallback callback =
                StreamingCallbackManager.instance().createCallbackInstance(mf? "mf": "sf");


        try {
            the_streaming_connection =
                    new DataConnection( state.hostname, String.valueOf( board.getSlot() ), "XX" );


            DataSink the_sink;

            if ( save_root != null ){
                the_sink = new Dt100FrameDataSink( save_root );
            }else{
                the_sink = new DataSink();
            }

            dst = new DataStreamingTransaction( the_sink, board, stride, mf, nchannels );

            dst.setCallback( callback );
            the_streaming_connection.post( dst );
        } catch ( DtacqException de ) {
            System.err.println( "Dt100.getData() caused " + de );
        }catch ( IOException ie ) {
            System.err.println( "Dt100.getData() caused " + ie );
        }
    }

    public void cancelStreaming() {

        if ( the_streaming_connection != null ){

            dst.stop();
            dst = null;
            the_streaming_connection.post(
                    new DataStreamingTransaction( null, getMaster(), 0, false, 0 ) );
            the_streaming_connection.post( new CloseTransaction() );
            the_streaming_connection = null;
        }
    }
    class Dt100DataSink extends DataSink {
        class RawStore {
            public final byte buffer[];
            public final int nbytes;

            public RawStore( byte buffer[], int nbytes ) {
                this.buffer = buffer;
                this.nbytes = nbytes;
            }
        }
        ArrayList buffers = new ArrayList();

        private byte[] makeSingleRawBuffer() {
            ListIterator it = buffers.listIterator();
            int totlen = 0;

            while( it.hasNext() ){
                totlen += ((RawStore)it.next()).nbytes;
            }

            byte[] bigbuffer = new byte[totlen];

            it = buffers.listIterator();
            int isrc = 0;

            while ( it.hasNext() ){
                RawStore rs = (RawStore)it.next();
                System.arraycopy( rs.buffer, 0, bigbuffer, isrc, rs.nbytes );
                isrc += rs.nbytes;
            }
            return bigbuffer;
        }

        FileOutputStream fs;

        Channel channel;
        boolean held_in_ram;

        public Dt100DataSink( Channel channel, boolean saves_to_file, boolean held_in_ram) {
            this.channel = channel;
            if ( saves_to_file && state.saveToFile ) {
                enableSaveToFile( state.rootDirectory );
            }
            this.held_in_ram = held_in_ram;
        }
        private void enableSaveToFile( String root ) {
            try{
                fs = new FileOutputStream( root + channel.makeIDString1() );
            } catch( IOException e ){
                System.err.println( "Dt100DataSink failed to create: " + e );
            }
        }
        public void processData( int offset, byte buffer[], int nbytes )
        {
            if (held_in_ram){
                buffers.add( new RawStore( buffer, nbytes ) );
            }
            if ( fs != null ){
                try {
                    fs.write( buffer, 0, nbytes );
                } catch( IOException e ){
                    ;
                }
            }
        }
        public void close() {
            if (held_in_ram) {
                theTableModel.addColumn(new ChannelData(channel, makeSingleRawBuffer()));
            }
            if ( fs != null ){
                try {
                    fs.close();
                } catch ( IOException e ) {
                    ;
                }
            }
        }
    }

    class GetBoardsTransaction extends SystemTransaction {

        class BoardLineDetail {
            String the_line;

            private void parse() {
                //  physical=0 logical=6 model=acq200

                if (the_line.indexOf( "=" ) < 0 ){
                    return;
                }

                String pairs[] = the_line.split( " " );
                int physical = -1;
                int logical = 1;

                for ( int ipair = 0; ipair != pairs.length; ++ipair ){
                    String kvp[] = pairs[ipair].split( "=" );
                    String key = kvp[0];
                    String val = kvp[1];

                    if ( key.startsWith( "physical" ) ){
                        physical = Integer.parseInt(val);
                    }else if ( key.startsWith( "logical" ) ){
                        logical = Integer.parseInt(val);
                    }else if (key.startsWith( "model" ) ){
                        ; // do nothing
                    }
                }
                if ( physical != -1 ){
                    System.out.println(
                            "physical"+physical+
                            "setSlot("+logical+")");

                    state.getBoard(physical+1).setSlot(logical);
                }
            }
            public BoardLineDetail( String the_line ) {
                this.the_line = the_line;
                parse();
            }
            public boolean isAcq32() {
                return the_line.startsWith( "ACQ32" );
            }
            public boolean isAcq16() {
                return the_line.startsWith( "ACQ16" );
            }
            public boolean isAcq200() {
                return the_line.startsWith( "ACQ200" );
            }
            public int getNumChannels() {
                if (isBoard()) {

                    /*
                    return Integer.parseInt( the_line.substring( 9, 11 ).trim() );
                    */
                    /* should really take result of getAvailableChannels WORKTODO */
                    return 37;
                } else {
                    return 0;
                }
            }
            public boolean isBoard() {
                return isAcq32() || isAcq16() || isAcq200();
            }
        }
        public void encode( PrintWriter output ) {
            output.println( COMMAND_PREFIX + " " + "getBoards" );
        }


        int decodeResidue( String residue ) {
            int nlines = 0;

            try {
                /* new style : lines=5 boards=1 */
                String pairs[] = residue.split( " " );
                String pair1[] = pairs[0].split( "=" );
                String pair2[] = pairs[1].split( "=" );

                if (pair1[0].equals( "lines" )){
                    nlines = Integer.parseInt(pair1[1]);
                    if (pair2[0].equals( "boards" )){
                        state.nboards = Integer.parseInt(pair2[1]);
                    }
                }
            } catch ( Exception e ){
                /* oldstyle:lines = boards + 1 */
                nlines = Integer.parseInt( residue );
                state.nboards = nlines - 1;

            }

            return nlines;
        }
        protected void onDecode( String residue, DataInputStream input )
                throws DtacqException, IOException {

            // residue holds # boards
            int nlines = decodeResidue( residue );


            state.boardDetails = "";
            state.acq32_count = 0;
            state.acq16_count = 0;
            state.acq200_count = 0;

            try {
                BufferedReader din =
                        new BufferedReader( new InputStreamReader(input) );

                int iboard = 1; // state board list indexes from one

                for (int iline = 0; iline != nlines; ++iline ){
                    String the_line = din.readLine();


                    if ( the_line == null ){
                        break;                  // shouldn't happen !!
                    }
                    BoardLineDetail detail = new BoardLineDetail( the_line );
                    state.boardDetails += the_line + "\n";

                    String model = "";

                    if ( detail.isBoard() ){
                        if ( detail.isAcq32() ){
                            state.acq32_count++;
                            model = "acq32";
                        }else if ( detail.isAcq16() ){
                            state.acq16_count++;
                            model = "acq16";
                        }else if ( detail.isAcq200() ){
                            state.acq200_count++;
                            model = "acq200";
                        }// else assert( NFG )
                        state.getBoard( iboard ).setChannelCount( model, detail.getNumChannels() );
                        ++iboard;
                    }
                }
            } catch (IOException e ){
                System.err.println( "GetBoardsTransaction, confused: " + e );
            }

        }
    }

    public void clearTableModel() {
        theTableModel.clear();
    }

    static final int MAXDATA_FETCH = 0x40000;

    public void getData(  dt100.DataFetchDetail detail ){
        try {
            Connection connection =
                    new DataConnection(
                            state.hostname,
                            String.valueOf( detail.channel.board.getSlot() ),
                            String.valueOf( detail.channel.channel() )
                    );


            if ( detail.format.length() > 0 ){
                connection.post( dt100.SetTransaction.format( detail.format ) );
            }
            DataSink the_sink = new Dt100DataSink( detail.channel, detail.saves_to_file, detail.held_in_ram);

            /*
            * add one transaction per MAXDATA_FETCH
            */

            int start = detail.start;
            while (start < detail.stop){
                int stop1 = start + MAXDATA_FETCH/2*detail.stride;

                if (stop1 < 0){
                    stop1 = Integer.MAX_VALUE;
                }
                if (stop1 > detail.stop){
                    stop1 = detail.stop;
                }
                DataFetchTransaction dft =
                        new DataFetchTransaction( the_sink, start, stop1, detail.stride );
                dft.setCallback( detail.callback );
                if (stop1 < detail.stop){
                    dft.setFinal(false);
                }
                connection.post( dft );
                start = stop1;
            }
            connection.post( new CloseTransaction() );
        } catch ( DtacqException de ) {
            System.err.println( "Dt100.getData() caused " + de );
        }catch ( IOException ie ) {
            System.err.println( "Dt100.getData() caused " + ie );
        }
    }

    class ChannelData implements Comparable {
        private final int ichan;
        private final byte[] data;
        private final String title;
        private final Channel channel;

        public ChannelData( Channel channel, byte[] data ) {
            this.ichan = channel.makeID();
            this.title = channel.makeIDString();
            this.data = data;
            this.channel = channel;
        }
        public int compareTo( Object other ) {
            try {
                ChannelData otherChannel = (ChannelData)other;
                return ichan - otherChannel.ichan;
            }catch ( ClassCastException e ) {
                System.out.println ( "ChannelData.compareTo " + e );
                return 1;
            }
        }
        public Channel getChannel() { return channel; }
        public Integer getValueAt( int index ) {
            return new Integer( getIntValueAt( index ) );
        }
        public int getIntValueAt( int index ) {
            index *= 2;

            short s1 = (short)data[index++];
            short s2 = (short)data[index];

            s2 <<= 8;
            s2 += s1&0x0ff;

            return s2;
        }
        public Integer getChannelNumber () {
            return new Integer(ichan);
        }
        public String getName() {
            return title;        // format with leading zero
        }
        public int length() {
            return this.data.length/2;
        }
    }
    class Dt100TableModel extends AbstractTableModel {

        SortedSet channel_set = new TreeSet( (Comparator)null );    // key by channel number
        ChannelData[] cd_array;                 // index by entry in map
        int length;

//        Object[] getTheData() { return the_data; }  // for use by friends

        public int getColumnCount() {
            Debug.print( 5, "getColumnCount() " + cd_array.length );

            return cd_array.length;
        }
        public Class getColumnClass( int columnIndex ) {
            return Integer.class;
        }

        public int getRowCount() {
            return length;
        }
        public Object getValueAt( int row, int col ) {
            Debug.println( 5, "getValueAt( "+row+", "+col+" )" );
            Debug.println( 5, "getValueAt() " +
                    cd_array[col].getName()+" " +
                    cd_array[col].getValueAt( row ) );

            return cd_array[col].getValueAt( row );
        }
        public int getIntValueAt( int row, int col ) {
            Debug.println( 5, "getIntValueAt( "+row+", "+col+" )" );
            Debug.println( 5, "getIntValueAt() " +
                    cd_array[col].getName() +" " +
                    cd_array[col].getValueAt( row ) );
            return cd_array[col].getIntValueAt( row );
        }
        public void clear() {
            channel_set.clear();
            cd_array = null;
            length = 0;
        }
        public synchronized void addColumn( ChannelData cd ) {
            channel_set.add( cd );
            length = cd.length();           // WORKTODO ... check max min ??

            cd_array = (ChannelData[])channel_set.toArray(
                    new ChannelData[0] ); // [1] p 123 Thanx!
        }
        public String getColumnName( int c ) {
            return cd_array[c].getName();
        }

        public Channel getChannel( int c ) {
            return cd_array[c].getChannel();
        }
    }

    class Dt100VoltsTableModel extends Dt100TableModel {
        // each column is the data set for a channel

        Dt100TableModel raw_model;
        private final float vmax;
        private final int MAXCODE=32768;

        public Dt100VoltsTableModel( Dt100TableModel raw_model ) {
            this.raw_model = raw_model;

            // WARNING this assumes that all channels in the same set have the same voltage
            // coding. Most of the time that's true.
            // evaluating this on a per cell basis will be expensive
            // a better way may be to have model-per-board underneath
            vmax = raw_model.getChannel( 0 ).board().getAIPrams().vmax;
        }

        public int getColumnCount() {
            return raw_model.getColumnCount();
        }
        public Class getColumnClass( int columnIndex ) {
            return Float.class;
        }
        public int getRowCount() {
            return raw_model.getRowCount();
        }
        public Object getValueAt( int row, int col ) {
            // or model per board use raw_model[row].getIntValueAt( startrow[row],col )
            // hmmm, saves function calls, uses lots of indexing
            return new Float( vmax * raw_model.getIntValueAt(row,col)/MAXCODE );
        }
        public void clear() {
            raw_model.clear();
        }
        public String getColumnName( int c ) {
            return raw_model.getColumnName( c );
        }
    }
    class Dt100IndexedTableModel extends Dt100TableModel {

        Dt100TableModel raw_model;

        public Dt100IndexedTableModel( Dt100TableModel raw_model ) {
            this.raw_model = raw_model;
        }

        public int getColumnCount() {
            return raw_model.getColumnCount()+1;
        }
        public Class getColumnClass( int columnIndex ) {
            return columnIndex==0? Integer.class: raw_model.getColumnClass( columnIndex-1 );
        }
        public int getRowCount() {
            return raw_model.getRowCount();
        }
        public Object getValueAt( int row, int col ) {
            // or model per board use raw_model[row].getIntValueAt( startrow[row],col )
            // hmmm, saves function calls, uses lots of indexing
            if ( col==0 ){
                return new Integer( row );
            }else{
                return raw_model.getValueAt( row, col-1 );
            }
        }
        public void clear() {
            raw_model.clear();
        }
        public String getColumnName( int c ) {
            if ( c == 0 ){
                return "Sample";
            }else{
                return raw_model.getColumnName( c-1 );
            }
        }
    }


    Dt100TableModel theTableModel;

    public TableModel getTableModel( boolean withIndex) {
        if ( withIndex ){
            return new Dt100IndexedTableModel( theTableModel );
        }else{
            return theTableModel;
        }
    }

    public TableModel getVoltsTableModel( boolean withIndex ) {
        if ( withIndex ){
            return new Dt100IndexedTableModel(
                    new Dt100VoltsTableModel( theTableModel ) );
        }else{
            return new Dt100VoltsTableModel( theTableModel );
        }
    }

    void initStatelessFields() {
        theTableModel = new Dt100TableModel();
    }

    public ResultCallback getStreamingDatasetStartCallback() {
        return streamingDatasetStartCallback;
    }

    public void setStreamingDatasetStartCallback(ResultCallback streamingDatasetStartCallback) {
        this.streamingDatasetStartCallback = streamingDatasetStartCallback;
    }

    public ResultCallback getStreamingDatasetEndCallback() {
        return streamingDatasetEndCallback;
    }

    public void setStreamingDatasetEndCallback(ResultCallback streamingDatasetEndCallback) {
        this.streamingDatasetEndCallback = streamingDatasetEndCallback;
    }

}

class StreamBufferManager {

    byte buf[];
    final int MAXBUF;
    DataInputStream input;

    int iput;   // increment after put
    int iget;   // increment after get

    public StreamBufferManager( DataInputStream input, byte buf[], int maxbuf ) {
        this.input = input;
        this.buf = buf;
        MAXBUF = maxbuf;
    }
    boolean isEmpty() {
        return iget == iput;
    }
    boolean hasSpace( int nblock ) {
        return iput+nblock < MAXBUF;
    }
    boolean hasBlock( int nblock ) {
        return iget+nblock <= iput;
    }
    void shuffle() {
        int ito;

        for ( ito = 0; !isEmpty(); ){
            buf[ito++] = buf[iget++];
        }
        iget = 0;
        iput = ito;
    }

    int getBlock( int nblock ) throws IOException {
        // return index of next block of data

        Debug.println( 6, "getBlock( " + nblock + " )" );

        if ( !hasBlock( nblock ) ){
            if ( !hasSpace( nblock ) ){
                shuffle();
            }
            // ASSERT( hasSpace( nblock ) )
            while ( !hasBlock( nblock ) ){

                int nread = input.read( buf, iput, MAXBUF-iput );

                if ( nread <= 0 ){
                    Thread.yield();
                    continue;
                }else{
                    iput += nread;
                }
            }
        }
// ASSERT( hasBlock() is true )

        int istart = iget;
        iget += nblock;

        Debug.println( 6, "getBlock() returns " + istart );

        return istart;
    }
}

class DataStreamingTransaction extends SystemTransaction implements Command {

    DataSink sink;
    Board board;
    int nchannels;
    int stride;
    boolean calc_mean;
    boolean please_stop;

    public DataStreamingTransaction( DataSink sink, Board board, int stride, boolean calc_mean, int nchannels ) {
        this.sink = sink;
        this.board = board;
        this.nchannels = nchannels;
        this.stride = stride;
        this.calc_mean = calc_mean;
    }
    public void encode( PrintWriter output ) {

        String command =
                COMMAND_PREFIX + " " +
                "stream " +
                stride + " " +
                (calc_mean? "1": "0") + " " +
                nchannels/2;

        Debug.println("DataStreamingTransaction.encode()" + command);

        output.println(command);
    }
    protected void finalize() {
        Debug.println( 2, "DataStreamingTransaction.finalize()" );
    }

    void stop() {
        please_stop = true;
    }
    final int MAXBUF = 0x10000;

    public void decode( DataInputStream input )
            throws DtacqException, IOException {

        if ( stride == 0 ) return;

        byte buf[] = new byte[MAXBUF];
        StreamBufferManager bufman = new StreamBufferManager( input, buf, MAXBUF );
        int rowLen = (nchannels+1)*2;

        StreamingCallback scb = (StreamingCallback)callback;
        int iframe = 0;

        try {

            while ( !please_stop ){

                Debug.println( 2, "DataStreamingTransaction.decode()" );
                StreamFrame streamFrame = new StreamFrame( board, nchannels );

                while( !streamFrame.isComplete() ){
                    int istart = bufman.getBlock( rowLen );

                    if ( istart < 0 ){
                        throw new DtacqException();
                    }

                    sink.processData( istart, buf, rowLen );

                    if ( streamFrame.appendSubframe( buf, istart, rowLen ) < 0 ){
                        scb.incrementErrors( 1 );
                        if ( please_stop ){
                            break;
                        }
                    }else{
                        scb.incrementPasses( 1 );
                    }
                }

                int inew_frame =  scb.setTheFrame( streamFrame );
                if (inew_frame != iframe && streamFrame.isFrame_accepted()){
                    sink.declareNewDataSet( iframe = inew_frame );

                    if ( scb.makeReadyToRun() ){
                        EventQueue.invokeLater( callback );
                    }
                }
            }

            Debug.println( 2, "DataStreamingTransaction.decode() STOP" );
        } finally {
            if ( sink != null ){
                sink.close();
            }
        }
    }
}
