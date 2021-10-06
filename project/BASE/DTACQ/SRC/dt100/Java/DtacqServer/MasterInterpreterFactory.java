/*****************************************************************************
 *
 * File:
 *
 * $RCSfile: MasterInterpreterFactory.java,v $
 * 
 * Copyright (C) 1999 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: MasterInterpreterFactory.java,v 1.16.4.1 2003/12/19 21:31:42 pgm Exp $
 * $Log: MasterInterpreterFactory.java,v $
 * Revision 1.16.4.1  2003/12/19 21:31:42  pgm
 * works with acq200
 *
 * Revision 1.16  2002/04/20 12:37:27  pgm
 * fixes file handle leak
 *
 * Revision 1.15  2002/04/20 09:07:10  pgm
 * close stream on BYE, hack CommandLine
 *
 * Revision 1.14  2001/06/17 13:48:38  pgm
 * remote shutdown enabled
 *
 * Revision 1.13  2001/01/14 17:52:32  pgm
 * use RandomAccessFile for multi thread safety
 *
 * Revision 1.12  2000/09/22 08:09:38  pgm
 * should handle stream shutdown cleanly
 *
 * Revision 1.11  2000/09/18 13:28:55  pgm
 * *** empty log message ***
 *
 * Revision 1.10  2000/08/22 06:41:14  pgm
 * switchable monitor
 *
 * Revision 1.9  2000/08/21 19:36:03  pgm
 * Stream Monitor added to dt100d
 *
 * Revision 1.8  2000/07/01 06:47:29  pgm
 * keeping cvs happy
 *
 * Revision 1.7  2000/05/28 10:31:42  pgm
 * streaming works passibly
 *
 * Revision 1.6  2000/05/04 18:50:06  pgm
 * Debug class added
 *
 * Revision 1.5  2000/04/24 20:15:57  pgm
 * getBoards, also better resize using GridBagLayout
 *
 * Revision 1.4  2000/04/16 10:59:23  pgm
 * data upload works!
 *
 * Revision 1.3  2000/04/15 14:11:09  pgm
 * prettify in VC
 *
 * Revision 1.2  2000/04/15 13:45:31  pgm
 * added headers
 *
 * Revision 1.1  1999/10/22 16:26:49  pgm
 * first entry to cvs
 *
 *
\*****************************************************************************/



import java.io.*;
import java.util.*;

// MasterInterpreterFactory.java - interprets master commands


class HandlerException extends Exception {
	HandlerException(){
		super( "HandlerException" );
	}
}

class StreamMonitor extends Thread {
	int sleep_secs;
	int nread_calls, nbuf_min, nbuf_max, nbuf_total, nerrors;

	public synchronized void update( int nbuf ) {

		nread_calls++;

		if ( nbuf < 0 ){
			nerrors ++;
		}else{
			if ( nbuf > nbuf_max ) nbuf_max = nbuf;
			if ( nbuf < nbuf_min ) nbuf_min = nbuf;

			nbuf_total += nbuf;
		}
	}
	synchronized void print() {
	
		int mean = nread_calls > 0? nbuf_total/nread_calls: 0;

		System.out.println( "" + 
				    nread_calls + " " + 
				    nerrors     + " " +
				    nbuf_min    + " " +
				    nbuf_max    + " " +
				    nbuf_total  + " " +
				    mean        + " "       );

		nread_calls = nbuf_max = nbuf_total = nerrors = 0;
		nbuf_min = 99999;
	}
	StreamMonitor() {
		super();
		sleep_secs = 2;
	}
	public void run() {
		for ( ;; ){
			try {
				sleep( sleep_secs*1000 );
				print();
			}catch( Exception e ){
				;
			}
		}
	}

	static StreamMonitor _inst = null;

	public static StreamMonitor instance() {
		if ( _inst == null ){
			_inst = new StreamMonitor();
		}
		return _inst;
	}

}
class BoardLevelHandler extends Interpreter {

	String device;
	Acq32Wrapper deviceWrapper; 

	public BoardLevelHandler( OutputStream out, String device )
		throws IOException {

		super( out );
		deviceWrapper = new Acq32Wrapper( this.device = device );
	}

	protected void signOn() {
		; // suppress sign on message
	}
	protected void onClose() {
		deviceWrapper.close();
	}
	protected boolean onCommand( CommandLine cl ) {
		String command = cl.toString();

		Debug.println( 2, "onCommand:" + command + " dev:" + device );

		command += "\n" ;        // need to keep device driver happy


		int len = command.length();
		byte bytes[] = new byte[len+1];

		command.getBytes( 0, len, bytes, 0 );

		try {
			byte reply [] = 
				deviceWrapper.sendCommand( bytes, len );
			Debug.println(  "< (len:" + reply.length +") \"" +
					new PrintableString( reply, 20 ) +"\"");
			out.write( reply );
		} catch ( IOException e ){
			System.err.println( "BoardLevelHandler.onCommand() " + e );
		}
		return !FINISHED;
	}
	protected boolean onCommandNoReply( CommandLine cl ) {
		String command = cl.toString();

		Debug.println( 2, "onCommand:" + command + " dev:" + device );

		command += "\n" ;             // need to keep device driver happy


		int len = command.length();
		byte bytes[] = new byte[len+1];

		command.getBytes( 0, len, bytes, 0 );

		try {
			deviceWrapper.write( bytes, len );
		} catch ( IOException e ){
			System.err.println( "BoardLevelHandler.onCommand() " + e );
		}
		return !FINISHED;
	}
	static final int NBUF = 0x10000;

	static String TAGSTR( byte raw[], int nprint ) {
		String rv = "TAGSTR:";
		int imax = nprint;
	
		if ( imax > raw.length ) imax = raw.length;

		for ( int iraw = 0; iraw != imax; ++iraw ){

			String hits = Integer.toString( raw[iraw^1]&0x00ff, 16 );
			if ( hits.length() < 2 ){
				rv += "0";
			}
			rv +=  hits;
			// Little endian is great. not.
		}

		rv += ":";
		return rv;
	}
	byte raw[] = new byte[NBUF];

	public void readStream() throws HandlerException, IOException {
		try {
			int nbuf = deviceWrapper.read( raw, NBUF );

			StreamMonitor.instance().update( nbuf );

			if ( nbuf == -1 ){
				return;             // no data yet
			}

			out.write( raw, 0, nbuf );
			Debug.println( 1, "nbuf:" + nbuf );
			Debug.println( 2, "tag:" + TAGSTR( raw, 2 ) );
			Debug.println( 3, "readStream() tag;" + 
				       TAGSTR( raw, 16 ) + " deliver " + nbuf );
		} catch ( IOException e ) {
			Debug.println( "readStream() took exception " + e );
			throw e;
		}
	}
	public void read( int start_sample, int stop_sample, int stride ) {
		// start..stop inclusive
		// binary   NB one sample == 2 bytes
   
		byte raw[] = new byte[NBUF];
		int start_byte = start_sample * 2;
		int stop_byte  = (stop_sample+1) * 2; // inclusive
		int nbytes_left = stop_byte - start_byte;;
		int nbytes_read = 0;
		int nbuf = 0;
        
		// WORKTODO - start, stop stride not handled properly

		Debug.println( 2,
			       "BoardLevelHandler.read() start read:" + 
			       (stop_byte-start_byte) );
            
		try {
	        
	       
			Debug.println( 2, "BoardLevelHandler.read() nleft:" + nbytes_left );
	        
			for ( ; nbytes_left > 0; nbytes_left = stop_byte - start_byte ){		        
				nbytes_read = nbytes_left>NBUF? NBUF: nbytes_left;	        
				nbuf = deviceWrapper.read( raw, nbytes_read );
				if ( nbuf == -1 ){
					break;
				}
				out.write( raw, 0, nbuf );
				start_byte += nbuf;
			}
		    
		} catch ( IOException e ) {
			System.err.println( "BoardLevelHandler.read():" + e );
	
			e.printStackTrace();
			System.err.println( 
					   "nbytes_left:" + nbytes_left + 
					   " start_byte:" + start_byte +
					   " nbuf:" + nbuf );
		}

		/* ASCII : WORKTODO
		   byte raw[] = new byte[(stop-start)/stride];

		   // WORKTODO

		   for ( int isample = start; isample < stop; isample += stride ){
		   print.println( isample );
		   }
		   ASCII */

		finally {
			Debug.println( 2, "BoardLevelHandler.read() done:");   
		}
	}
}

class StreamerThread extends Thread {
	BoardLevelHandler board_level_handler;
	boolean please_stop;

	StreamerThread( BoardLevelHandler blh ) {
		board_level_handler = blh;
	}
	public void run () {
		// now hack in a loop to stream
		// WORKTODO ... NO EXIT POSSIBLE

		try {
			for ( int iloop = 0; !please_stop; ++iloop ){
				yield(); // green threads
				Debug.println( 6, "streaming loop " + iloop );

				try {
					board_level_handler.readStream();
				} catch( HandlerException e ){
					Debug.println( "ABORT:" + e );
					break;
				} catch( IOException ie ) {
					Debug.println( "FINISH:" + ie );
					break;
				}
			}
		}finally{
			Debug.println( "StreamerThread quitting\n" );
		}
	} 
	public void requestStop() { please_stop = true; }
}

class MasterInterpreter extends Interpreter {

	// work to do : make the specialization!

	StreamerThread streamerThread;

	public MasterInterpreter( OutputStream out ) {
		super( out );
	}
	protected void signOn() {
		print.println( "MasterInterpreter" );   
	}

	BoardLevelHandler board_level_handler;

	protected void onClose() {
		if ( board_level_handler != null ){
			board_level_handler.onClose();
		}
	}
	protected boolean onCommand( String command ) {
		// default impl, hacked out impl of simple command grammar
		// could be better!

		Debug.println( ">" + command );
		Debug.println( 2, "MasterInterpreter.onCommand " + command );

		CommandLine cl = new CommandLine( command );
		Iterator it = cl.iterator();

		if ( !it.hasNext() ){
			return ack();                // are you there?
		}else{
			command = it.next().toString();

			if ( command == null ) {
				return ack();
			}else if ( command.equals( "acqcmd" ) ) {
                
				if ( board_level_handler != null ){
					return board_level_handler.onCommand( 
									     new CommandLine( it ) );
				}else{
					return nack( "no open board level device" );
				}
			}else if ( command.equals( "dt100" ) ){
				return onCommand( new CommandLine( it ) );
			}else{
				return nack( "command not understood:" + command );
			}
		}
	}

	boolean ack() {
		String sack = "DT100:";

		Debug.println( "<" + sack );
		print.println( sack );
		return !FINISHED;
	}
	boolean ack( String prams ) {

		String sack =  "DT100:" + prams;
		Debug.println(  "<" + sack );
		print.println( sack);
		return !FINISHED;
	}
	boolean nack( String msg ) {
		String snack = "DT100:ERROR " + msg;

		Debug.println(  "<" + snack );
		print.println( snack  );
		return !FINISHED;
	}

	String lines[] = new String[40];
	int nboards = 0;
	int iline = 0;

	class BoardCounter {
		String board_list;
		public BoardCounter( String board_list ){
			this.board_list = board_list;
		}
		public int act() {
			try {
				FileInputStream fis = new FileInputStream( board_list );
				BufferedReader din = 
					new BufferedReader( new InputStreamReader(fis) );


				while ( true ) {
					String theLine = din.readLine();
		
					if ( theLine == null ){
						break;
					}

					lines[iline++] = theLine;

					if ( theLine.startsWith( "#" ) ){
						continue;
					}
					if ( theLine.startsWith( "model" ) ){
						continue;
					}
					if ( theLine.indexOf( "NO DEVICES FOUND" ) != -1 ){
						continue;
					}
					nboards++;
				}
				return 0;
			} catch( IOException e ) {
				return -1;
			}
		}
	}
	class BoardMap {
		/* handles mapping between physical and logical number */
		String map_file;
		public BoardMap( String map_file ) { this.map_file = map_file; }
		public int act() {

			System.out.println( "BoardMap.act()" );
			try {
				FileInputStream fis = new FileInputStream( map_file );
				BufferedReader din = 
					new BufferedReader( new InputStreamReader(fis) );


				while ( true ) {
					String theLine = din.readLine();

					System.out.println( "BoardMap.act() " + theLine );

					if ( theLine != null ){
						lines[iline++] = theLine;
					}else{
						break;
					}
				}
				return 0;
			} catch( IOException e ) {
				System.err.println( "ERROR " + map_file + " " + e );
				return -1;
			}
		}
	}
	private boolean getBoards() {
		int ecount = 0;
		/**
		 * read /proc/acq32, counting non comment lines
		 * output #boards, raw output
		 */   
		if ((new BoardCounter( "/proc/acq32" )).act() != 0){
			ecount ++;
		}
		if ((new BoardCounter( "/proc/acq200" )).act() != 0){
			ecount ++;
		}
		(new BoardMap( "/dev/acq32/boardmap" )).act();

		if ( ecount > 1 ){
			return nack( "ACQ32 driver not initialized" );
		}

		ack( "lines="+String.valueOf(iline)+" boards="+String.valueOf(nboards) );

		for ( int ii = 0; ii != iline; ++ii ){
			print.println( lines[ii] );
		}

		return !FINISHED;
	}
	protected boolean onCommand( CommandLine cl ) {

		Debug.println( 2, "MasterInterpreter.onCommand(cl) " + cl );
		Iterator it = cl.iterator();
    	
		String subcommand = it.next().toString();

		Debug.println( 2, "processing subcommand\""+subcommand+"\"" );

		if ( subcommand == null ) {
			return ack();

			// WORKTODO ... NO CHECKING WATCHIT
		}else if ( subcommand.equals( "read" ) ){

			int start, stop, stride;

			if ( it.hasNext() ){
				start = Integer.parseInt( (String)it.next() );
			}else{
				return nack( "no start value" );
			}
			if ( it.hasNext() ){
				stop = Integer.parseInt( (String)it.next() );
			}else{
				return nack( "no stop value" );
			}
			if ( it.hasNext() ){
				stride = Integer.parseInt( (String)it.next() );
			}else{
				return nack( "no stride" );
			}
			Debug.println( 2, "start: "+start+" stop: "+stop+" stride: "+stride );
			ack( String.valueOf( 2*(stop-start+1)/stride ) + " bytes" );
			//ack( String.valueOf( (stop-start)/stride ) + " lines" );
			board_level_handler.read( start, stop, stride );
			return !FINISHED;

		}else if ( subcommand.equals( "stream" ) ){

			Debug.println( 2, "Acting on stream command" );

			CommandLine cl2 = new CommandLine( cl.toString() );
			Iterator it2 = cl2.iterator();

			it2.next();
			if ( it2.hasNext() ){
				String stride = it2.next().toString();

				if ( stride.equals( "0" ) ){
					streamerThread.requestStop();
					streamerThread = null;
					board_level_handler.onCommandNoReply( 
									     new CommandLine( subcommand, it, "" ) ); 
					return !FINISHED;
				}
			}
			// else .. attempt to start streaming
			board_level_handler.onCommandNoReply( 
							     new CommandLine( subcommand, it, "" ) ); 

			streamerThread = 
				new StreamerThread( board_level_handler );

			streamerThread.start();
		    
			return !FINISHED;
		}else if ( subcommand.equals( "open" ) ){


			String mode = it.next().toString();

			Debug.println( 2,
				       "MasterInterpreter.onCommand(cl): mode: " + 
				       mode + " WORKTODO" );
			String device = it.next().toString();
    	
			if ( device != null ){
    	        
				try {
					board_level_handler = 
						new BoardLevelHandler( out, device );
					return ack();
				} catch( IOException e ) {
					return nack( "failed to open " + device + " " + e );
				}
			}else{
				return nack( "no device to open" );
			}
		}else if ( subcommand.equals( "getBoards" ) ){
			return getBoards();
		}else if ( subcommand.equals( "exec" ) ){
			if ( Exec.run( it.next().toString() ) == 0 ){
				return ack();
			}else{
				return nack( "failed to exec command" );
			}
		}else{
			return nack( "DT100:ERROR bad arg " + subcommand );
		}
	}
}


public class MasterInterpreterFactory implements InterpreterFactory {

	// work to do ... control access to master
	public Interpreter create( OutputStream out ) {
		return new MasterInterpreter( out );
	}

	public static final String rev_id = "MasterInterpreterFactory $Revision: 1.16.4.1 $";

	public static void main( String[] args ) throws IOException {

		System.out.println( rev_id );

		if ( args.length >= 1 && args[0].equals( "BoardLevelTest" ) ){
			Debug.println( 2, "BoardLevelTest" );

			String device = "/dev/acq32/acq32.1.m1";

			Interpreter blh = new BoardLevelHandler( System.out, device );

			BufferedReader in = new BufferedReader(
							       new InputStreamReader( System.in ) );

			String s;

			while( (s = in.readLine()) != null ) {
				CommandLine cl = new CommandLine( s );

				blh.onCommand( cl );
			}

			return;
		}else if ( args.length >= 1 && args[0].equals( "local" ) ){

			Interpreter interpreter = new MasterInterpreter( System.out );
    	    
			BufferedReader input = new BufferedReader(
								  new InputStreamReader( System.in ) );
			String s;

			while ( ( s = input.readLine()) != null ){
				interpreter.onCommand( s );
			}
		}else{
			int nthreads = 10;

			try {
				if ( args.length >= 1 && Integer.parseInt(args[0]) > 0 ){
					Debug.setDebugLevel( Integer.parseInt(args[0]) );
				}
				if ( args.length >=2  && Integer.parseInt(args[1]) > 0 ){
					nthreads = Integer.parseInt(args[1]);
				}
			}catch( Exception e ){
				; // probably a numeric
			}
			if ( args.length >= 1 && args[0].equals( "monitor" ) ){

				try {
					StreamMonitor.instance().start();
				}catch( IllegalStateException e ){
					System.out.println( "??? PGM: ??? " + e );
				}
			}


        	    
			System.out.println( "DtacqServer rides again" );
    	    
			DtacqServer server = 
				new DtacqServer( 
						DtacqComms.PORT, new MasterInterpreterFactory(), 
						nthreads
						);
		}
	}
}






