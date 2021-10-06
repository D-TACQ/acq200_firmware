public class PrintableString
{
    String the_string;
    
    PrintableString( byte buf[], int maxlen ) {
        int len;
        
        if ( maxlen > buf.length ) {
            len = buf.length;
        }else{
            len = maxlen;
        }
        String bufstr = new String( buf );
        
        bufstr = bufstr.substring( 0, len-1 );
        
        char chars[] = bufstr.toCharArray();
        
        for ( int ichar = 0; ichar != chars.length; ++ichar ){
            
            // valid for ASCII only
            
            if ( !(chars[ichar] >= ' ' && chars[ichar] <= '~') ){
                chars[ichar] = '.';
            }
        }
        the_string = new String( chars );
    }
    public String toString() {
        return the_string;  
    }
}