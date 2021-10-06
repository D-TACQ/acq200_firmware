function chV = toVolts( ch )
     if ( nargin < 1 )
         usage( "toVolts( ch )" );
     endif

     chV = ch*10/32768
endfunction
