import java.io.*;

// Exec - execute a command

class KeyValue {
    public String key;
    public String value;

    public KeyValue( String key, String value ) {
	    this.key = key; this.value = value;
    }
}

class CommandLookupTable {
    /*
    class KeyValue {
	public String key;
	public String value;

	public KeyValue( String key, String value ) {
	    this.key = key; this.value = value;
	}
    }
    */
    static KeyValue commands[] = {
	    new KeyValue( "who", "sh whoami.sh" ),
	    new KeyValue( "ls",  "sh ls.sh" ),
	    new KeyValue( "shutdown", "sh shutdown.sh" )
    };
    
    static String lookup( String key ) {
	for ( int ikey = 0; ikey != commands.length; ++ikey ){
	    if ( key.startsWith( commands[ikey].key ) ){
		return commands[ikey].value;
	    }
	}
	return null;
    }
}

public class Exec {

    static public int run( String command ) {
	String full_command;

	if ( (full_command = CommandLookupTable.lookup( command )) != null ){
	    try {
		Runtime.getRuntime().exec( full_command );
		return 0;
	    }catch( IOException e ) {
		System.out.println( "exec( " + full_command + " ) failed" );
		return -1;
	    }
	}else{
	    System.out.println( "command " + command + " not available" );
	    return -1;
	}
    }
    public static void main( String[] args ) {
	for ( int iarg = 0; iarg != args.length; ++iarg ){
	    System.out.println( "run( " + args[iarg] + " )" );
	    int error = run( args[iarg] );
	    System.out.println( error==-1? "FAILED": "SUCCESS" );
	}
    }
}

