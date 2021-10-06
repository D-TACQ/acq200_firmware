/*****************************************************************************
 *
 * File:
 *
 * $RCSfile: CommandLine.java,v $
 * 
 * Copyright (C) 1999 D-TACQ Solutions Ltd
 * not to be used without owner's permission
 *
 * Description:
 *
 * $Id: CommandLine.java,v 1.5 2002/04/20 09:07:10 pgm Exp $
 * $Log: CommandLine.java,v $
 * Revision 1.5  2002/04/20 09:07:10  pgm
 * close stream on BYE, hack CommandLine
 *
 * Revision 1.4  2002/04/20 08:50:12  pgm
 * nodos
 *
 * Revision 1.2  2000/04/15 13:45:31  pgm
 * added headers
 *
 * Revision 1.1  1999/10/22 16:26:49  pgm
 * first entry to cvs
 *
 *
\*****************************************************************************/

//package dtacqserver;

//package pgmutil;

// CommandLine.java - parser for space separated args

import java.io.*;
import java.util.*;


public class CommandLine {

    String the_command;
    List list = new ArrayList();

    void buildFrom( String the_command ){
	this.the_command = the_command;

	// parse the command

	int istart, istop;

	for ( istart = istop = 0; 
	      istart < the_command.length(); 
	      istop += 1                      ){

	    if ( the_command.charAt( istart ) == ' ' ){
		;
	    }else if ( istop >= the_command.length() ){
		list.add( the_command.substring( istart ) );
	    }else if ( the_command.charAt( istop ) == ' ' ){
		list.add( the_command.substring( istart, istop ) );
	    }else{
		continue;
	    }

	    istart = ++istop;
	}
    }

    public CommandLine( String the_command ) {
	buildFrom( the_command );
    }

    public Iterator iterator() { return list.iterator(); }

    public CommandLine( Iterator it ) {
	String rump = "";

	while( it.hasNext() ){
	    rump += it.next() + " ";
	}

	buildFrom( rump );
    }
    public CommandLine( String the_command, Iterator it, String whatsthis ) {
	// this is a pretty speculative reconstruciton of some LOST CODE 
	// WORKTODO
	while( it.hasNext() ){
	    the_command += " " + it.next();
	}
	buildFrom( the_command );
    }
    public String toString() {
	String str = "";

	for ( Iterator it = list.iterator(); it.hasNext(); ){
	    str += it.next() + " ";
	}
	return str;
    }
    public static void main( String args[] ) throws IOException {

	    BufferedReader in = new BufferedReader(
			            new InputStreamReader( System.in ) );

	    String s;

	    while( (s = in.readLine()) != null ) {
	        CommandLine cl = new CommandLine( s );
	        int iarg = 0;

	        for ( Iterator it = cl.iterator(); it.hasNext(); ){
		        System.out.println( ++iarg + " " + it.next() );
	        }
	    }
    }
}
