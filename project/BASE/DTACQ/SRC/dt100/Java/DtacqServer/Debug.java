public class Debug
{
    static int debug_level;
    
    public static void println( String line ) {
        if ( debug_level > 0 ) {
            System.out.println( line );
        }
    }
    public static void println( int lvl, String line ) {
        if ( debug_level >= lvl ) {
            System.out.println( line );
        }
    }
    public static void print( String line ) {
        if ( debug_level > 0 ) {
            System.out.print( line );
        }
    }
    public static void print( int lvl, String line ) {
        if ( debug_level >= lvl ) {
            System.out.print( line );
        }
    }
    public static int setDebugLevel( int lvl ) {
        int rv = debug_level;
        
        debug_level = lvl;

        System.out.println( "Debug.setDebugLevel( " + lvl + " )" );

        return rv;
    }
}
